# Vehicle Anti-Theft System (GPS Geofencing)

**Group 8 | Project Topic #25 | KNUST | March 2026**

This project demonstrates a functional GPS-based Vehicle Anti-Theft System designed on the Arduino Uno R3 platform. The system utilizes geofencing logic to monitor a vehicle's coordinates against a predefined rectangular boundary enclosing the KNUST campus. When a breach is detected, the system triggers a multi-channel alert, including a high-frequency piezo siren sweep and a remote owner notification displayed on a secondary LCD.

To overcome simulation limitations, we implemented sensor simulation using a 20-step coordinate array to represent live geographic data. This repository contains the full source code, electrical schematics, and technical documentation required for the Microprocessors project submission.

---

## Group Members

- Kwame Adom Asuma-Perprem (Index: 7266023)
- Francis Arthur (Index: 7264823)
- Hammond Jude Emmanuel Nii Armah (Index: 7272423)
- Bruce Lamptey David Aseda (Index: 7268623)
- Hardi Ruhaina (Index: 7272523)
- Asante-Opoku Francis (Index: 7265323)
- Dafeamekpor Michel Selorm (Index: 7269023)
- Kejanga Ajigeweh (Index: 7273423)
- Arloo Priscilla Afiba (Index: 7264523)

---

## Folder Contents

This folder contains the 6 required project files for the COE_group8 submission:

- **Coe_group8_code.ino** - Final Arduino source code featuring the geofence logic and siren sweep function.
- **Coe_group8_project_report.pdf** - Comprehensive technical report including methodology, results, and future improvements.
- **Coe_group8_circuitview.png** - High-resolution screenshot of the physical breadboard layout in Tinkercad.
- **Coe_group8_Circuit.brd** - The physical circuit design export file.
- **Coe_group8_schematic_view.png** - Technical electrical schematic of the system.
- **Coe_gorup8_schematic.pdf** - Detailed PDF export of the system's schematic connections.

---

## How to Run the Simulation

1. Open Tinkercad Circuits and create a new project.
2. Wire the Arduino Uno R3 to two 16x2 I2C LCDs and a Piezo Buzzer on Digital Pin 8 as shown in Coe_group8_circuitview.png.
3. Set the I2C addresses for the LCDs: 0x27 for the primary status display and 0x26 for the owner notification display.
4. In the Tinkercad Code editor, select Libraries and include the LiquidCrystal I2C library.
5. Copy the code from Coe_group8_code.ino and paste it into the editor.
6. Click Start Simulation to observe the 20-step sequence, where Step 13 triggers the breach alert.

---
