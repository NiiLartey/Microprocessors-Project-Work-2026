# Home-Intrusion-System
Smart Arduino security system with keypad authentication, motion detection, and real-time alarm response.
## Arduino Security System

This project is a smart embedded security system built using Arduino. It combines multiple sensors and input methods to monitor, authenticate, and respond to potential intrusions.

### Features
- Keypad-based PIN authentication
- Ultrasonic sensor for user detection
- PIR motion sensor for intrusion detection
- Multi-state system (Idle, Authentication, Access Granted, Alert, Alarm)
- RGB LED for visual status indication
- Dual buzzer system for alerts and alarms

### How It Works
- The system remains idle until a user is detected within range.
- The user is prompted to enter a PIN using the keypad.
- Correct PIN → Access granted  
- Wrong PIN (multiple attempts) → Alert mode  
- Motion detected in alert mode → Alarm triggered  

### Components Used
- Arduino board
- 4x4 Keypad
- PIR Motion Sensor
- Ultrasonic Sensor (HC-SR04)
- RGB LED
- Buzzers

### Applications
- Home security systems
- Smart door access control
- Embedded systems learning projects
