# Week 1 Log (Peter Njuguna)

**Period:** 21st – 27th September 2025  
**Project:** TAFITI Nanosatellite Bench Model  
**Location:** JKUAT – TAFITI Project Lab  

---

### 🧭 Activities
- Created the **Bill of Materials (BOM)** for the project.  
- Tested **LoRa communication** using **ESP32** and **RFM96** modules.  
- Implemented **transmission with acknowledgment** and **automatic retransmission** features.  
- Attempted **image transmission via LoRa**.  
- **Interfaced GSM module** with ESP32 and created a **web app** for real-time data visualization.  

---

### 🏆 Achievements So Far
- Successfully **interfaced RFM95 with ESP32-S3**, enabling transmission of a test message (“hello”) with an incrementing counter.  
- Implemented **automatic retransmission with acknowledgment** — transmitter resent the message up to three times if no ACK was received within a defined timeout period.  
- Successfully **hosted a webpage using ESP32**, allowing remote **LED control** (on/off and brightness adjustment).  
- **Connected a GPS module** to the ESP32-S3 for location data acquisition.  
- Conducted initial trials on **image transmission via LoRa** (transmission successful, reception under review).  

---

### ⚙️ Challenges Faced
- **Image transmission issue:** The transmitted image could not be opened or had an invalid format upon reception.  
- **GPS limitation:** While the GPS module connected successfully, **no satellites were detected**, making it impossible to determine precise location data.  

---

### 🔧 Plan for the Week Ahead
- Implement **image processing using Raspberry Pi**.  
- Attempt to **send processed image via LoRa**.  
- **Interface LoRa with Raspberry Pi**, and establish reliable **data transfer between ESP32 and Pi** for payload communication.  
