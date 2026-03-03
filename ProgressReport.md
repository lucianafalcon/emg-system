EMG Interface for Muscle Activity Monitoring  
Progress Report  
Author: Luciana Falcon  
Date: 04/12/2025  

2nd Semester 2025  

Below is the progress report of the final project based on the defined requirements.

---

## Requirements Status

| Status | Description |
|--------|------------|
| 🟢 | Implemented |
| 🟡 | In progress |
| 🔴 | Will not be implemented |

---

## Functional Requirements

| Code | Requirement | Status |
|------|------------|--------|
| RF1 | EMG signal acquisition using AD8232 | 🟢 |
| RF2 | ADC digitization at ≥ 1 kHz | 🟢 |
| RF3 | Digital processing (RMS, filtering, threshold) | 🟢 |
| RF4 | Visualization on OLED display | 🟢 |
| RF5 | Data transmission via Bluetooth | 🟢 |
| RF6 | Buzzer activation based on threshold | 🟢 |
| RF7 | Start / stop control using button | 🟢 |

---

## Non-Functional Requirements

| Code | Non-Functional Requirement | Status |
|------|----------------------------|--------|
| RNF1 | Power consumption below 20 mA | 🟢 |
| RNF2 | Continuous Bluetooth communication | 🟢 |
| RNF3 | Display update every 100 ms | 🟢 |
| RNF4 | Latency below 50 ms | 🟢 |
| RNF5 | Super-loop firmware architecture | 🟢 |

---

**Note:**  
The system design, firmware architecture, and timing analysis were completed. The embedded code implementation was successfully validated without integration or debugging issues.
