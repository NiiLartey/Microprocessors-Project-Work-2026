# 🌿 Smart Autonomous Irrigation System (Group 9)
**KNUST COE 381 - Microprocessors**

Project Overview
This project is an end-to-end IoT solution designed for precision agriculture. It utilizes an **ESP32** to monitor soil moisture and reservoir levels, providing autonomous irrigation while ensuring hardware longevity through a software-defined **Safety State**.

 Key Functionalities
* **Autonomous Irrigation:** Automatically triggers the water pump when moisture drops below 30%.
* **Hardware Protection (Safety Lock):** System kills pump power if the reservoir level is < 20% to prevent cavitation.
* **Cloud Supervision:** Real-time telemetry and manual override via **Blynk IoT Cloud**.
* **Weather Awareness:** Integrated logic to bypass irrigation during rain events.

Technical Specifications
* **Microcontroller:** ESP32 (30-pin/38-pin)
* **Connectivity:** WiFi (802.11 b/g/n)
* **Sensors:** * HC-SR04 Ultrasonic (Range: 2cm - 400cm)
    * Analog Soil Moisture Sensor (Mapped 0-4095)
* **Actuators:** 5V Single-Channel Relay Module
* **Platform:** Blynk IoT (Virtual Pins V0-V4)

 Repository Structure
* `/Code`: Contains the `.ino` firmware file.
* `/Schematics`: Circuit diagrams and pinout maps.
* `/Media`: Screenshots of the Blynk Dashboard and Simulation.

 Group 9 Members
1. Nasirud-Deen - Lead Architect & Firmware
2. **Adaf** & **Michael** - Cloud & UI
3. **Fred** & **Farida** - Hardware & Power
4. **Hakim** & **Mujahid** - Logic & QA
5. **Sylvester** & **Ifraj** - Documentation & Coordination