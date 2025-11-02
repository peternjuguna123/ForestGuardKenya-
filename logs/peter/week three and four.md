"""# Week 3 & 4 (28th Sept – 11th Oct 2025)

## Objectives
- To achieve reliable image transmission between different devices using LoRa.
- To begin PCB design and layout familiarization using EasyEDA.

---

## Activities Undertaken
- Focused primarily on **image transmission experiments** using LoRa and different microcontroller setups.  
- Tested several transmission configurations:  
  - **ESP32 → Arduino:** Transmission initiated successfully, but the acknowledgment (ACK) signal response was delayed due to the ESP32’s higher processing speed compared to the Arduino.  
  - **ESP32 → ESP32:** Attempted direct communication; however, serially writing received data to the PC proved impossible, leading to transmission failure.  
  - **Raspberry Pi → ESP32:** Communication attempted, but no ACK signal was received.  
  - **Raspberry Pi → Arduino:** Communication established, though manual reset was required after each transmission.  
- Started **PCB design** using **EasyEDA**, focusing on learning how to create and assign **custom footprint modules**.  
- Continued to review approaches for **On-Board Computer (OBC)** simulation in Proteus.  

---

## Achievements
- Understood LoRa synchronization and timing differences between various microcontrollers.  
- Identified a more efficient approach: saving received LoRa data directly to an **SD card** for later retrieval.  
- Successfully initiated PCB design workflow and familiarized with component footprint creation in EasyEDA.  

---

## Challenges
- Inconsistent image transfer and lack of ACK signal between devices.  
- Limitations in serial data writing when interfacing ESP32 with the PC.  
- Learning curve in PCB design tools and custom footprint generation.  

---

## Next Steps
- Implement and test **ESP-to-ESP LoRa image transmission** with SD card data storage.  
- Advance PCB design and refine footprint creation in EasyEDA.  
- Begin **OBC simulation** in Proteus once PCB foundation is ready.  
"""

