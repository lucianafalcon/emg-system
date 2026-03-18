# EMG Embedded System  
**Real-time muscle activity monitoring using STM32  + AD8232 Module + ESP32 + Web Interface**

---

## Overview

This project implements a **complete embedded system for EMG (Electromyography) signal acquisition and processing**, using non-invasive surface electrodes.

The system captures muscle activity, processes it in real time, and streams the results both locally (LCD) and remotely (web interface via WiFi).

---

## System Architecture

<div align="center">
<img width="1243" height="828" alt="511713972-35658773-ff54-48d2-b060-35d2d7419d01" src="https://github.com/user-attachments/assets/6549db3b-ea5b-4614-aa76-6dab692ff90b" />
</div>

<div align="center">
Figure: Block diagram of the EMG interface system.
</div>

---

## Key Features

- **2-channel EMG acquisition**
- **Real-time signal processing** (moving average + hysteresis)
- **Low latency detection (< 50 ms)**
- **Wireless data streaming** via ESP32 (WiFi + WebSocket)
- **Local visualization** on 16x2 LCD
- **Threshold alert system** (buzzer)
- **Guided calibration routine**
- **Ultra low CPU usage (~7.29%)**

---

## Demo

## Live Web Monitoring

<div align="center">
<img width="917" height="457" alt="558470288-84d7e047-b59f-49cb-90c2-d057e97e424a" src="https://github.com/user-attachments/assets/94813765-adb5-4be6-9760-5da97df9b7c4" />
</div>

<div align="center">
Figure: "EMG Live Monitor" web page showing both channels in real time. The ascending peak in EMG 1 corresponds to a correctly detected muscle contraction. The descending peaks in both channels are WebSocket reconnection artifacts, not EMG signal artifacts.
</div>

---

## Hardware

- STM32 NUCLEO-F103RB  
- AD8232 EMG sensor (x2)  
- ESP32 (WiFi module)  
- LCD 16x2 display  
- Electrodes (surface EMG)  
- Buzzer + keypad  

---

## Software & Technologies

- **C (bare-metal)** – STM32 firmware  
- **UART communication** – STM32 ↔ ESP32  
- **Arduino framework** – ESP32  
- **WebSocket protocol** – real-time streaming  
- **HTML + JavaScript** – live monitoring dashboard  

---

## Signal Processing

- ADC sampling ≥ 1 kHz  
- Sliding window average (8 samples)  
- Noise reduction via smoothing  
- Threshold detection with hysteresis  
- User-calibrated activation levels  

---

## Performance

- **CPU Load:** ~7.29%  
- **Power Consumption:** ~14.5 mA  
- **Detection Latency:** < 50 ms  

---

## Repository Structure
/tdse-tp3_04-interactive_menu-main → STM32 firmware   
/esp32_emg_wifi.ino → ESP32 firmware    
/emg_dashboard.html → Web interface   
/Final_Report_EMG_System.md → Full report and documentation   

---

## Applications

- Human-machine interfaces  
- Rehabilitation systems  
- Prosthetic control  
- Muscle activity monitoring  
- Embedded biomedical devices  

---

## Author

**Luciana B. Falcon**  
Electronic Engineering (Embedded Systems)



