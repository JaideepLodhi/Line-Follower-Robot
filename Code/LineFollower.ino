#define AIN1 4
#define AIN2 3
#define PWMA 9
#define BIN1 6
#define BIN2 7
#define PWMB 10
#define STBY 5

#define button1 11
#define button2 12
#define button3 8

const uint8_t SensorCount = 6;
const uint8_t sensorPins[SensorCount] = {A0, A1, A2, A3, A4, A5};
uint16_t sensorValues[SensorCount];
int threshold[SensorCount];
int minValues[SensorCount], maxValues[SensorCount];

float Kp = 0.8, Ki = 0, Kd = 0.002;
uint8_t multiP = 1, multiI = 1, multiD = 1;
int P, D, I, previousError, error;
int lsp, rsp;
int lfspeed = 230;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(STBY, OUTPUT);
    digitalWrite(STBY, HIGH);
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
  
}

void loop() {
  while(digitalRead(button2)!=1){
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    callibration();
    digitalWrite(LED_BUILTIN, LOW);
  }
  while(digitalRead(button1)!=1){
    digitalWrite(LED_BUILTIN, HIGH);
    while(1){
      readSensors();
      positionControl();
    }
  }
}
void callibration(){
  Serial.println("Calibrating Sensors... Move the robot over the line!");
      // Initialize min and max values
    for (uint8_t i = 0; i < SensorCount; i++) {
        minValues[i] = 1023;  // Max possible value
        maxValues[i] = 0;     // Min possible value
    }

    unsigned long startTime = millis();  // Start time for 5-sec calibration

    while (millis() - startTime < 5000) {  // Run for 5 seconds
        for (uint8_t i = 0; i < SensorCount; i++) {
            int reading = analogRead(sensorPins[i]);
            if (reading < minValues[i]) minValues[i] = reading;
            if (reading > maxValues[i]) maxValues[i] = reading;
        }
        delay(5);  // Small delay to avoid excessive reads
        motor_drive(80, -80);
    }
    motor_drive(0, 0);

    // Set threshold as midpoint of min and max readings
    for (uint8_t i = 0; i < SensorCount; i++) {
        threshold[i] = (minValues[i] + maxValues[i]) / 2;
        Serial.print(threshold[i]);
        Serial.print("  ");
    }

    Serial.println("\nCalibration Complete. Ready to start!");
    delay(1000);
}

void positionControl() {
    int position = calculatePosition();
    Serial.println(position);
    error = 2500 - position;
    if (allSensorsHigh()) {
        motor_drive(previousError > 0 ? -230 : 230, previousError > ? 230 : -230);
    } else {
        PID_Linefollow(error);
    }
}

void PID_Linefollow(int error) {
    P = error;
    I += error;
    D = error - previousError;
    previousError = error;
    
    float Pvalue = (Kp / pow(10, multiP)) * P;
    float Ivalue = (Ki / pow(10, multiI)) * I;
    float Dvalue = (Kd / pow(10, multiD)) * D;

    float PIDvalue = Pvalue + Ivalue + Dvalue;
    
    lsp = constrain(lfspeed - PIDvalue, -230, 230);
    rsp = constrain(lfspeed + PIDvalue, -230, 230);
    
    motor_drive(lsp, rsp);
}

void motor_drive(int left, int right) {
    analogWrite(PWMA, abs(left));
    digitalWrite(AIN1, left > 0 ? HIGH : LOW);
    digitalWrite(AIN2, left > 0 ? LOW : HIGH);

    analogWrite(PWMB, abs(right));
    digitalWrite(BIN1, right > 0 ? HIGH : LOW);
    digitalWrite(BIN2, right > 0 ? LOW : HIGH);
}

void readSensors() {
    for (uint8_t i = 0; i < SensorCount; i++) {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

bool allSensorsHigh() {
    for (uint8_t i = 0; i < SensorCount; i++) {
        if (sensorValues[i] > threshold[i]) return false;
    }
    return true;
}

uint16_t calculatePosition() {
    uint32_t weightedSum = 0;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < SensorCount; i++) {
        weightedSum += (uint32_t)sensorValues[i] * (i * 1000);
        sum += sensorValues[i];
    }
    return sum > 0 ? weightedSum / sum : 2500;
}
