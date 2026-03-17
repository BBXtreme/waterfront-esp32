# ESP32 Firmware Test Checklist for WATERFRONT Project

This checklist ensures comprehensive testing of the ESP32 firmware for the WATERFRONT kayak rental system. It covers unit tests, integration tests, end-to-end tests, and edge cases. Use this to validate functionality, reliability, and performance before deployment.

## 1. Unit Tests (Module-Level Testing)
- [ ] **Config Loader Tests**:
  - [ ] Load valid config.json from LittleFS.
  - [ ] Handle missing config.json (fallback to defaults).
  - [ ] Validate config fields (pins 0-39, ports 1-65535, etc.).
  - [ ] Test invalid JSON parsing.
  - [ ] Save config to LittleFS.
  - [ ] Update config from JSON payload via MQTT.
- [ ] **MQTT Client Tests**:
  - [ ] Initialize MQTT with config credentials.
  - [ ] Handle connection/disconnection events.
  - [ ] Publish status, slot status, retained status, and ack messages.
  - [ ] Validate topic paths with location codes.
  - [ ] Test publish failures (e.g., not connected).
- [ ] **Gate Control Tests**:
  - [ ] Initialize LEDC timers and channels.
  - [ ] Open/close compartments (non-blocking).
  - [ ] Check gate states via limit switches.
  - [ ] Handle timeouts and retries.
  - [ ] Validate compartment ID bounds.
- [ ] **Deposit Logic Tests**:
  - [ ] Start rental timers with grace periods.
  - [ ] Check overdue rentals and auto-lock.
  - [ ] Handle deposit on take/return.
  - [ ] Publish MQTT events on return.
  - [ ] Test timer creation failures.
- [ ] **Logger Tests**:
  - [ ] Initialize with config log level.
  - [ ] Log to SPIFFS file with timestamps.
  - [ ] Rotate log files when size exceeds limit.
- [ ] **Error Handler Tests**:
  - [ ] Publish fatal errors to MQTT topics.
  - [ ] Restart ESP32 on fatal errors.
  - [ ] Handle buffer overflows and null messages.

## 2. Integration Tests (Component Interaction)
- [ ] **MQTT and Config Integration**:
  - [ ] Load config and use in MQTT init.
  - [ ] Publish config updates via MQTT.
  - [ ] Test thread-safe config access with mutexes.
- [ ] **Gate and Deposit Integration**:
  - [ ] Start rental and open gate.
  - [ ] Auto-lock on overdue via timer callback.
  - [ ] Publish status changes to MQTT.
- [ ] **WiFi/MQTT Connectivity**:
  - [ ] Connect to WiFi using config SSID/password.
  - [ ] MQTT reconnect on WiFi loss.
  - [ ] Test fallback to LTE (if implemented).
- [ ] **LittleFS Operations**:
  - [ ] Mount/format LittleFS.
  - [ ] Read/write config files.
  - [ ] Handle file corruption.

## 3. End-to-End Tests (Full System Flow)
- [ ] **Booking Flow**:
  - [ ] Simulate MQTT unlock command → open gate.
  - [ ] Publish taken event on sensor trigger.
  - [ ] Start deposit timer.
- [ ] **Return Flow**:
  - [ ] Simulate return → close gate.
  - [ ] Publish returned event.
  - [ ] Release deposit if on-time.
- [ ] **Overdue Handling**:
  - [ ] Timer expires → auto-lock gate.
  - [ ] Publish overdue alert.
- [ ] **Error Scenarios**:
  - [ ] MQTT disconnection → retry logic.
  - [ ] Invalid compartment ID → log error.
  - [ ] Config validation failure → use defaults.

## 4. Performance and Load Tests
- [ ] **Concurrency Tests**:
  - [ ] Simulate 100+ concurrent bookings.
  - [ ] Test mutex deadlocks in config access.
- [ ] **Memory Tests**:
  - [ ] Monitor heap/stack usage during operations.
  - [ ] Test JSON buffer overflows.
- [ ] **Power Tests**:
  - [ ] Deep sleep modes.
  - [ ] Battery/solar voltage monitoring.

## 5. Security Tests
- [ ] **Credential Validation**:
  - [ ] MQTT username/password from config.
  - [ ] TLS certificate paths.
- [ ] **Input Validation**:
  - [ ] Sanitize MQTT payloads.
  - [ ] Validate config fields against bounds.
- [ ] **Access Control**:
  - [ ] Test offline PIN validation (if implemented).

## 6. Hardware Integration Tests
- [ ] **Sensor Tests**:
  - [ ] Ultrasonic sensors for presence detection.
  - [ ] Limit switches for gate state.
- [ ] **Actuator Tests**:
  - [ ] Servo motors for gate control.
  - [ ] Relay for locks.
- [ ] **Connectivity Tests**:
  - [ ] WiFi provisioning.
  - [ ] BLE setup (if implemented).

## 7. Regression Tests
- [ ] **Version Compatibility**:
  - [ ] Test config migration.
  - [ ] Backward compatibility with old JSON.
- [ ] **OTA Updates**:
  - [ ] Firmware update via MQTT.
  - [ ] Rollback on failure.

## 8. Documentation and Reporting
- [ ] **Test Coverage**:
  - [ ] Ensure 80%+ code coverage.
  - [ ] Document uncovered areas.
- [ ] **Bug Tracking**:
  - [ ] Log all failures with steps to reproduce.
  - [ ] Prioritize fixes based on severity.
- [ ] **Sign-Off**:
  - [ ] All tests pass before release.
  - [ ] Update FSD/TSD with test results.

## Tools and Setup
- **Testing Framework**: Unity or Catch2 for ESP32.
- **Simulation**: Mosquitto for MQTT broker.
- **Hardware**: ESP32 dev board with sensors/actuators.
- **CI/CD**: GitHub Actions for automated tests.

This checklist should be updated as the firmware evolves. Mark items as completed and note any issues.
