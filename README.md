# SmartBike Telemetry — Backend / Firmware

This repository contains the backend and firmware components for the SmartBike telemetry system. It focuses on device-side code (firmware) and backend integration points (network transport, telemetry format). The frontend/mobile application is not included in this repository.

**Quick links**
- **Firmware/Arduino sketch:** [SmartBike_Telemetry/SmartBike_Telemetry.ino](SmartBike_Telemetry/SmartBike_Telemetry.ino)

**Project status:** Prototype — firmware and backend code present. Frontend is maintained separately.

**Overview**
- **Purpose:** Provide firmware that reads sensors (BLE, GPS, IMU), formats telemetry messages, and transmits them over the network to a broker or backend service.
- **Scope of this repository:** Device firmware, sensor managers, and network transport. It does not contain the UI/frontend application.

**Repository structure**
```
SmartBike_Telemetry/
├─ SmartBike_Telemetry.ino       # Main Arduino/ESP firmware sketch
├─ BleManager.h                  # BLE handling (scanning/peripheral)
├─ GpsManager.h                  # GPS/GNSS parsing and helpers
├─ ImuManager.h                  # IMU initialization and filtering
├─ NetworkManager.h              # Network transport (MQTT/WebSocket/UDP)
├─ DisplayManager.h              # On-device display management (if applicable)
└─ README.md                      # This file (backend-focused)
```

Note: If a `flutter_application_1/` folder exists in the workspace, it is not part of the official backend deliverable and is not supported from this repository.

**Hardware / Requirements**
- Microcontroller with BLE/GPS/IMU support (e.g., ESP32 or equivalent).
- GNSS/GPS module and IMU (accelerometer/gyro) as expected by the firmware headers.

**Firmware — quick start**
1. Open `SmartBike_Telemetry/SmartBike_Telemetry.ino` in the Arduino IDE or PlatformIO.
2. Install required Arduino libraries referenced in the headers.
3. Configure pins and network credentials in the firmware headers (e.g., `NetworkManager.h`).
4. Select the target board and upload.

Notes:
- `NetworkManager.h` defines the transport and destination (MQTT broker, WebSocket URL, or UDP endpoint). Update it to point to your backend.
- Message format and topic/topic schema are implemented by the firmware; inspect the telemetry formatting code before integrating with your server.

**Testing and simulation**
- Use a simple MQTT publisher or UDP sender to simulate telemetry messages while developing server-side components.

**Contributing**
- Open issues for firmware bugs or feature requests.
- For code changes, fork and submit a pull request with a description and test notes.

---

If you'd like, I can:
- remove the `flutter_application_1/` folder from this repo and add a note referencing a separate frontend repo,
- add an example MQTT simulator script to `tools/`, or
- add a `platformio.ini` for building with PlatformIO.
Tell me which and I'll implement it.
# SmartBike Telemetry System

This repository contains the firmware and companion Flutter app for a SmartBike telemetry system. The project collects sensor data (BLE, GPS, IMU), transmits it over the network, and displays live telemetry and ride summaries in a Flutter app.

**Quick links**
- **Firmware/Arduino:** [SmartBike_Telemetry/SmartBike_Telemetry.ino](SmartBike_Telemetry/SmartBike_Telemetry.ino)
- **Flutter app:** [SmartBike_Telemetry/flutter_application_1](SmartBike_Telemetry/flutter_application_1)

**Project status:** Prototype — firmware and mobile/desktop app available. Improvements welcome.

**Overview**
- **Purpose:** Collect and stream bike telemetry (speed, cadence, GPS location, IMU data) to a client app for visualization and logging.
- **Components:** Arduino/ESP-style firmware (C++/Arduino), and a Flutter application (multi-platform) that subscribes to telemetry (via MQTT or network API).

**Repository structure**
- **SmartBike_Telemetry/**: Firmware headers and Arduino sketch plus the Flutter application.
  - `SmartBike_Telemetry.ino`: Main firmware sketch.
  - `BleManager.h`, `GpsManager.h`, `ImuManager.h`, `NetworkManager.h`, `DisplayManager.h`: Firmware modules.
  - `flutter_application_1/`: Flutter frontend (live map, dashboard, charts).

**Project structure (detailed)**
```
SmartBike_Telemetry/
├─ SmartBike_Telemetry.ino              # Main Arduino sketch
├─ BleManager.h                         # BLE handling (scanning/peripheral)
├─ GpsManager.h                         # GPS/GNSS parsing and helpers
├─ ImuManager.h                         # IMU initialization and filtering
├─ NetworkManager.h                     # Network transport (MQTT/WebSocket/UDP)
├─ DisplayManager.h                     # On-device display management (if any)
└─ README.md                             # (this file)
```

**Hardware / Requirements**
- Microcontroller with BLE/GPS/IMU support (ESP32, Arduino with shields, or similar).
- GNSS/GPS module and IMU (accelerometer/gyro) as used in the firmware headers.
- Host machine with Flutter SDK for running the app (Windows, macOS, Linux, Web, Android, iOS as supported).

**Firmware (Arduino) — quick start**
- Open [SmartBike_Telemetry/SmartBike_Telemetry.ino](SmartBike_Telemetry/SmartBike_Telemetry.ino) in the Arduino IDE or PlatformIO.
- Install any required libraries referenced by the headers (check the top of each header for includes and library names).
- Select the target board/port and upload.

Notes:
- `NetworkManager.h` contains the network transport used by the firmware — it may use MQTT, WebSocket, or a simple UDP/TCP stream. Inspect that file to configure broker/host details.

**Flutter app — quick start**
1. Install Flutter SDK: https://flutter.dev/docs/get-started/install
2. From a terminal run:

```bash
cd SmartBike_Telemetry/flutter_application_1
flutter pub get
flutter run
```

- To run on Windows desktop: `flutter run -d windows` (Windows tooling required).
- To run in a browser: `flutter run -d chrome` from the same folder (web folder present).

Configuration:
- The app contains `lib/mqtt_service.dart` which configures the MQTT broker and topics. Update its constants or use the app's settings UI (if available) to point to your telemetry source.

**Development notes**
- Firmware modules are modularized in the headers; adapt pin assignments, sensor initialization, and network credentials to your hardware.
- The Flutter app organizes UI under `lib/pages` and widgets under `lib/widgets` for live map and charts.

**Testing**
- You can simulate incoming telemetry by running a small MQTT publisher that publishes messages matching the app's expected topics/format. The app's `mqtt_service.dart` reveals the topic and message structure.

**Contributing**
- Open issues for bugs or feature requests.
- For code contributions, fork, create a branch, and submit a pull request with a clear description of changes.

