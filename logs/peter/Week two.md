# Week 2 Log (Peter Njuguna)

**Period:** 28th September – 4th October 2025  
**Project:** TAFITI Nanosatellite Bench Model   

---

### 🧭 Activities
- Integrated **GPS**, **ESP32**, and **LoRa** modules for synchronized data communication.  
- Combined **ESP32**, **DHT11**, and **LoRa** modules to collect and visualize sensor data via a web app.  
- **Interfaced Raspberry Pi 3** with a **camera module** for automatic image capture and storage.  
- Performed **image processing on Raspberry Pi**, including **image compression** and **encryption**.  
- Implemented **onboard payload data collection and transmission** systems for environmental monitoring.  

---

### 🏆 Achievements So Far
- Successfully collected data using the **DHT11 sensor**, hosted a **webpage**, and **visualized real-time data in graphical form**.  
- Transmitted the collected sensor data via **LoRa**, and hosted another **receiver-side webpage** for comparing transmitted and received data.  
- **Interfaced Raspberry Pi 3 with a PC camera** — achieved live video streaming, image capture, timestamping, and storage of the media.  
- Implemented **image compression** and **encryption** on Raspberry Pi, including the **generation and storage of a secret encryption key**.  
- Connected **DHT11 sensor to Raspberry Pi 3**, with data transmitted from the Pi → ESP32 → RFM95 → Receiver ESP32 through **UART and LoRa links**.  

---

### ⚙️ Challenges Faced
- Initially faced **difficulties with encryption implementation** and **key generation**, but successfully resolved them after debugging.  
- The **PC camera produced blurred images** at first, making it hard to stream and focus. The issue was resolved by using **RealVNC (graphical interface)** instead of SSH for camera configuration.  

---

### 🔧 Plan for the Week Ahead
- **Transmit processed images and encryption key via LoRa** for secure payload communication.  
- Implement **ground station command execution** to control onboard functions remotely.  
- Begin **simulation and design of the On-Board Computer (OBC)** connections using **Proteus** and/or **EasyEDA**.  
