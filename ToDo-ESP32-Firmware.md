# ToDo - ESP32 Firmware for WATERFRONT Project

This ToDo list outlines tasks for developing and improving the ESP32 firmware for the WATERFRONT kayak rental system. It is based on the project plan, test checklist, and improvement steps. Tasks are prioritized and categorized.

## Immediate Priority (Week 1-2)
- [ ] Set up PlatformIO project with ESP-IDF framework.
- [ ] Implement basic config loader (LittleFS JSON).
- [ ] Prototype MQTT client (connect/disconnect).
- [ ] Basic gate control (LEDC for servos, GPIO for switches).
- [ ] Unit tests for config and MQTT.
- [ ] Fix extern "C" issues in headers for C++ compatibility.
- [ ] Add error checking for LEDC config in gate_control.cpp.

## Short-Term (Week 3-6)
- [ ] Complete gate control with state machine and timeouts.
- [ ] Implement deposit logic (timers, overdue handling).
- [ ] Enhance MQTT with event handling and publishing.
- [ ] Add error handling and logging.
- [ ] Integration tests for MQTT and gate control.
- [ ] Update MQTT topics to use location codes from config.
- [ ] Integrate gate control with deposit logic for auto-locking.

## Medium-Term (Week 7-9)
- [ ] Comprehensive unit tests (80% coverage).
- [ ] E2E tests (booking-to-return flow).
- [ ] Load testing (concurrency, memory usage).
- [ ] Security testing (TLS, input validation).
- [ ] Optimize power consumption (deep sleep).
- [ ] Add keypad support for PIN entry.
- [ ] Extend config for multi-location support.

## Long-Term (Week 10-12)
- [ ] OTA update implementation.
- [ ] Final integration with backend.
- [ ] Documentation updates (FSD/TSD).
- [ ] CI/CD setup (GitHub Actions).
- [ ] Production deployment and monitoring.
- [ ] Add telemetry for battery/solar/LTE.
- [ ] Improve logging with remote shipping.

## Ongoing Tasks
- [ ] Code reviews and refactoring.
- [ ] Bug fixes based on testing.
- [ ] Update test checklist as tasks complete.
- [ ] Ensure thread-safe config access.
- [ ] Validate all config fields and bounds.

## Completed Tasks
- [x] Initial firmware structure.
- [x] Basic MQTT and config integration.
- [x] Gate control with retries.
- [x] Deposit logic with timers.

This list should be updated weekly. Mark tasks as completed and add new ones as needed.
