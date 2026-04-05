# Crash-Car-Detector-Rakshak-
Project Rakshak is an ultra-low-cost, IoT automotive safety prototype from the CVMU Hackathon. It uses a dual-sensor loop to detect crashes and filter false alarms. If unresponsive, a custom Android app act as a Dead Man's Switch, instantly pushing live GPS coordinates to a cloud telemetry dashboard to save lives during the Golden Hour.



Project Rakshak 🚗📡
An IoT-Driven Automotive Crash Detection & Emergency SOS System

📌 Overview
Project Rakshak is an ultra-low-cost, aftermarket automotive safety prototype designed to autonomously detect severe vehicle impacts and dispatch live emergency coordinates during the critical "Golden Hour" of a crash. The system integrates a dual-sensor hardware loop with a custom Android smartphone gateway to filter out false alarms and execute a "Dead Man's Switch" protocol if the driver is unresponsive.

This project was developed and pitched as a Grand Finalist prototype at the CVMU Hackathon 4.0.

⚡ Key Features
False Alarm Prevention: Utilizes a dual-sensor approach (Accelerometer + Vibration) paired with a 10-second manual cancel button to filter out speed bumps, hard braking, or minor bumps.

The "Dead Man's Switch": If a crash is detected and the driver does not press the physical cancel button within 10 seconds, the hardware forces the companion Android app to silently execute an emergency API call and launch GPS routing.

Live Telemetry Dashboard: All incidents are recorded on a custom web dashboard, logging precise latitude/longitude coordinates and tracking whether an event was a critical emergency or a resolved false alarm.

Smartphone Gateway Integration: Uses Bluetooth to tether the Arduino hardware to an Android device, acting as a proxy cellular module for API triggers.

🛠️ Hardware Components Used
Arduino (Core Microcontroller)

MPU-6050 Accelerometer & Gyroscope (G-Force impact detection)

SW-420 Vibration Sensor (Physical shockwave detection)

HC-05 Bluetooth Module (Hardware-to-App communication)

NEO-6M GPS Module (Location tracking)

Active Buzzer & Push Button (Cabin alarm and manual override)

💻 Software & Platforms
C++ / Arduino IDE: For sensor logic, G-force calculations, and the 10-second trigger loop.

MIT App Inventor: For building the custom Android gateway app that catches Bluetooth payloads and executes background HTTP GET requests.

ThingSpeak API: Used as the cloud server to receive and store the field6=1 danger flags.

HTML/CSS/JavaScript: For the live telemetry dashboard UI.

⚙️ How the Logic Loop Works
Impact Detection: The MPU-6050 and SW-420 constantly monitor for a simultaneous G-force spike (>1.8G) and physical vibration shock.

The Golden Hour Countdown: Upon detection, the motor cuts off, and the cabin buzzer sounds for 10 seconds.

Safe Mode: If the driver presses the push button, a SAFE flag is sent, the alarm clears, and no emergency alerts are triggered.

Danger Mode: If 10 seconds pass with no response, the Arduino blasts a FORCE_MAP command to the Android app. The app automatically pushes the data payload to ThingSpeak and forces Google Maps to open the coordinates.

🚀 Future Roadmap (Phase 2)
Transition from smartphone-tethering to a standalone SIM7600 4G LTE module.

Implement an isolated backup power supply for post-crash reliability.

Upgrade detection algorithms to include vehicle rollover states using the MPU-6050 Z-axis data.
