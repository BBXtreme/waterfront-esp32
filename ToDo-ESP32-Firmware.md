# TODO: ESP32 Firmware for Waterfront Kayak Rental System

This document outlines the remaining tasks to complete the ESP32 firmware for the Waterfront unmanned kayak rental system. Based on the TSD (Technical Specification Document), FSD (Functional Specification Document), and current code status, the firmware is ~70% complete. Focus on production readiness, testing, and edge cases.



**Changes made**: Removed every Vercel reference (no demo link, no deployment mention). Switched **exclusively** to HiveMQ (Cloud primary, optional CE Docker kept lightly). Fixed broken doc links, added clean architecture overview + features table, modernized quick-start, improved tone/structure/flow, added MQTT topic examples. Shorter yet more professional and actionable.

---

### Issues & Areas for Improvement (deductions)
1. **Framework mixing** (biggest current risk): `platformio.ini` = `espidf`, but code uses `Arduino.h`, `Servo`, `ArduinoOTA`, `pinMode()`, `delay()`, `millis()`, `LittleFS` (Arduino-style). `mqtt_handler.cpp` even calls `httpUpdate` (Arduino lib). This hybrid works short-term but can cause linker issues or missed IDF power optimizations. **Recommendation**: Either switch to `framework = arduino` (faster dev) or go pure ESP-IDF (native OTA + LEDC for servo instead of Servo lib).
2. **Duplicate MQTT logic**: Both `mqtt_client.cpp` and `mqtt_handler.cpp` define `mqtt_init()`, `event_handler()`, globals. Looks like migration in progress — consolidate into one module.
3. **Security polish needed**:
   - OTA password hardcoded `"admin"`
   - TLS cert loading present but fallback logic could be tighter for HiveMQ Cloud
   - `tlsSkipVerify` option (good for dev, warn in prod)
4. **Minor**: Some functions referenced in `main.cpp` (`readBatteryLevel`, `lte_power_management`, `startRental`) are stubs or in other files — complete them for full sensor/deposit flow.

### Security & Robustness
Strong watchdog + error handling. MQTT reconnect + last-will good. CRC validation excellent. Add client certificates and proper TLS CA pinning for production HiveMQ clusters.

### Conclusion & Recommendations
This is **far above typical hobby ESP32 projects**. The modular task-based design, runtime config, OTA/telemetry events, and CRC integrity show serious engineering mindset. Once the Arduino/ESP-IDF conflict is resolved and sensor/deposit logic is wired in, this firmware will be deployment-ready for real solar lockers.

**Priority fixes** (in order):
1. Resolve framework mixing (choose one path)
2. Merge duplicate MQTT files
3. Make OTA password + TLS certs fully configurable
4. Wire full `return_sensor` + `deposit_logic` integration

Push any updated files (especially `compartment_manager.cpp`, `return_sensor.cpp`, or full `config_loader.h`) and I can give an even deeper line-by-line review. Excellent foundation — ship it! 🚀





## High Priority (Next Sprint: 1-2 Weeks)

### 1. Complete OTA Update Implementation
- **Status**: Basic OTA via MQTT URL is implemented, but needs hardening.
- **Tasks**:
  - Add firmware version checking before update (compare current vs. new version).
  - Implement rollback mechanism (store previous firmware in OTA partition).
  - Add signature verification for OTA binaries (use ESP-IDF secure boot if possible).
  - Test OTA over LTE (ensure TinyGSM HTTP client works with httpUpdate).
  - Publish OTA progress/status via MQTT (e.g., {"otaProgress": 50, "status": "downloading"}).
- **Files**: `mqtt_handler.cpp`, `main.cpp` (add version define if not present).
- **Testing**: Simulate OTA with local HTTP server; verify restart and MQTT status publish.

### 2. Finish Deposit Logic & Auto-Lock
- **Status**: Basic timers in `deposit_logic.cpp`, but incomplete integration.
- **Tasks**:
  - Integrate with sensor events: On "returned" event, check rental duration and release deposit.
  - Add overdue handling: Auto-lock gate after grace period; publish MQTT alert.
  - Implement deposit hold/release via MQTT (e.g., publish to backend for Stripe refund).
  - Add PIN validation for offline mode (use `offline_fallback.cpp`).
  - Handle edge cases: Multiple rentals, partial returns, damage reports.
- **Files**: `deposit_logic.cpp`, `mqtt_handler.cpp`, `return_sensor.cpp`.
- **Testing**: Mock sensor events; verify timers with `millis()`; test MQTT publishes.

### 3. Production-Ready WiFi Provisioning
- **Status**: BLE and SoftAP skeletons exist, but need polishing.
- **Tasks**:
  - Complete BLE provisioning: Test with nRF Connect app; handle disconnections gracefully.
  - Add SoftAP fallback: Improve web UI (add CSS, error handling); test captive portal.
  - Implement credential persistence: Use NVS for WiFi creds; auto-retry on boot.
  - Add provisioning timeout and status MQTT publishes.
  - Security: Randomize AP password; use secure BLE pairing.
- **Files**: `nimble.cpp`, `webui_server.cpp`, `wifi_manager.cpp`.
- **Testing**: Flash clean ESP32; test provisioning flow; verify MQTT reconnect.

### 4. LTE Cellular Fallback
- **Status**: TinyGSM integration started, but not fully tested.
- **Tasks**:
  - Complete modem initialization: Handle PWRKEY, SIM PIN, APN setup.
  - Implement failover logic: Monitor WiFi RSSI; switch to LTE if < threshold.
  - Add data usage tracking: Publish MQTT alerts if nearing limit.
  - Test with real SIM: Verify MQTT over cellular; handle network drops.
  - Power optimization: Power-down modem when WiFi available.
- **Files**: `lte_manager.cpp`, `mqtt_client.cpp`.
- **Testing**: Use test SIM; simulate WiFi failure; check Serial logs for modem AT commands.

## Medium Priority (Next Sprint: 2-3 Weeks)

### 5. Sensor Integration & Calibration
- **Status**: Ultrasonic sensor code exists, but needs real-world tuning.
- **Tasks**:
  - Calibrate distance thresholds: Test with actual kayak sizes; adjust for weather (rain affects ultrasonic).
  - Add sensor redundancy: Use weight sensor (HX711) as backup for presence detection.
  - Implement debouncing: Avoid false triggers from waves or animals.
  - Add sensor health checks: Publish MQTT if sensor fails (e.g., invalid readings).
- **Files**: `return_sensor.cpp`, `compartment_manager.cpp`.
- **Testing**: Field test with real hardware; simulate invalid readings.

### 6. Power Management & Deep Sleep
- **Status**: Basic watchdog implemented; deep sleep TODOs in code.
- **Tasks**:
  - Implement deep sleep: Wake on MQTT or sensor interrupt; calculate battery life.
  - Add solar voltage monitoring: ADC for battery level; publish MQTT alerts.
  - Optimize for low power: Disable unused peripherals; use RTC GPIOs for wake.
  - Handle power failures: Save state to NVS; resume on boot.
- **Files**: `power_manager.cpp` (create if missing), `main.cpp`.
- **Testing**: Measure current draw; test wake sources.

### 7. Offline Mode & PIN Validation
- **Status**: `offline_fallback.cpp` skeleton exists.
- **Tasks**:
  - Complete PIN sync: Subscribe to retained MQTT topic for active bookings/PINs.
  - Implement local validation: Check entered PIN against NVS cache.
  - Add keypad support: GPIO matrix for PIN entry (optional for phase 2).
  - Handle expiration: Clean up old bookings; sync on MQTT reconnect.
- **Files**: `offline_fallback.cpp`, `mqtt_handler.cpp`.
- **Testing**: Mock PIN entries; test NVS persistence.

### 8. Error Handling & Logging
- **Status**: Basic fatal_error() implemented.
- **Tasks**:
  - Add comprehensive error codes: Map ESP errors to human-readable strings.
  - Implement retry logic: For MQTT reconnects, sensor failures.
  - Add debug logging levels: Conditional on `debugMode` config.
  - Publish errors via MQTT: For remote monitoring.
- **Files**: `error_handler.cpp`, `mqtt_handler.cpp`.
- **Testing**: Trigger errors (e.g., invalid config); verify logs and MQTT publishes.

## Low Priority (Future Sprints)

### 9. Admin Features & Remote Control
- **Tasks**:
  - Add MQTT commands for remote unlock/lock.
  - Implement config updates: Runtime changes to pins, thresholds.
  - Add telemetry enhancements: Battery %, RSSI, data usage.
- **Files**: `mqtt_handler.cpp`, `config_loader.cpp`.

### 10. Security Hardening
- **Tasks**:
  - Enable MQTT TLS with client certs.
  - Add firmware encryption (ESP-IDF secure boot).
  - Implement BLE secure pairing.
- **Files**: `mqtt_client.cpp`, `nimble.cpp`.

### 11. Multi-Compartment Support
- **Tasks**:
  - Extend to 5-10 compartments: Dynamic pin loading from config.
  - Add per-compartment logic: Individual timers, sensors.
- **Files**: `compartment_manager.cpp`, `gate_control.cpp`.

### 12. Testing & CI/CD
- **Tasks**:
  - Expand Catch2 tests: Cover all modules (MQTT, sensors, gates).
  - Add GitHub Actions: Build, test, flash simulation.
  - Integration tests: End-to-end MQTT flow.
- **Files**: `test/*.cpp`, `.github/workflows/ci.yml`.

## General Notes

- **Config-Driven**: All pins, thresholds, broker details loaded from `/config.json` on LittleFS. Update via MQTT for remote changes.
- **Modular Code**: Each feature in separate .cpp/.h; use extern g_config for shared state.
- **Testing Strategy**: Unit tests with mocks; integration on real ESP32; field testing in Bremen.
- **Dependencies**: Ensure PlatformIO libs are up-to-date (e.g., PubSubClient, ArduinoJson).
- **Version Control**: Commit frequently; use branches for features (e.g., `feature/ota-hardening`).

## Progress Tracking

- [ ] Basic MQTT connect/subscribe/publish
- [ ] Relay control for gates
- [ ] Ultrasonic sensor for presence
- [ ] BLE provisioning skeleton
- [ ] SoftAP provisioning skeleton
- [ ] LTE modem integration skeleton
- [ ] Config loading from LittleFS
- [ ] Watchdog for stability
- [ ] Unit tests with Catch2
- [ ] CI with GitHub Actions
- [ ] OTA hardening (rollback, signatures)
- [ ] Deposit auto-release
- [ ] Production provisioning
- [ ] LTE failover testing
- [ ] Sensor calibration
- [ ] Power optimization
- [ ] Offline PIN validation
- [ ] Error handling polish

Next: Prioritize OTA and deposit logic for MVP. Contact for blockers or code reviews.
