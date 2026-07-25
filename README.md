# 🚗 Line Follower Robot

A PID-based autonomous line follower robot developed using Arduino Uno, IR sensors, and the L298N motor driver. The robot continuously detects the path and adjusts motor speed to achieve smooth and accurate line tracking.

---

## 📌 Features

- PID-based line tracking
- Smooth navigation on straight and curved paths
- High-efficiency motor control using the TB6612FNG driver
- Compact and lightweight design using N20 geared motors
- Adjustable speed through PWM control

---

## 🛠️ Hardware Used

- Arduino Uno
- TB6612FNG Dual Motor Driver
- 2 × N20 Geared DC Motors
- 6-Channel IR Sensor Array
- Robot Chassis
- Caster Wheel
- 7.4V Li-ion Battery
- Jumper Wires

---

## 💻 Software Used

- Arduino IDE
- Embedded C

---

## ⚙️ Working Principle

The IR sensor array continuously detects the position of the line. Based on the sensor readings, the Arduino calculates the error and applies a PID control algorithm. The TB6612FNG motor driver receives PWM signals from the Arduino and independently controls the speed and direction of the two N20 geared DC motors, allowing the robot to follow the line accurately.

---

## 📂 Project Structure

```
Line-Follower-Robot
│
├── README.md
├── Components.txt
├── Code
│   └── LineFollower.ino
├── Circuit_Diagram
│   └── circuit.png
└── Images
    ├── robot.jpg
    └── working.jpg
```

---

## 🚀 Future Improvements

- Obstacle Detection
- Bluetooth/Wi-Fi Monitoring
- Automatic PID Tuning
- OLED Display for Sensor Values

---

## 👨‍💻 Author

**Jaideep Lodhi**
