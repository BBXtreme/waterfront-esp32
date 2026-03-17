# Waterfront MQTT Contract – Single Source of Truth

**Topic Prefix**: `/station/{machineId}/` (machineId = e.g. "bremen-harbor-01")

## Topics & Payloads

### Incoming (Backend → ESP32)
- `/station/{machineId}/unlock`  
  ```json
  {"bookingId":"bk_abc123","pin":"7482","durationSec":7200,"depositAmountEur":50}

### Outgoing (ESP32 → Backend)

- /station/{machineId}/status (retained)

  JSON

  ```
  {"state":"connected","uptimeMin":145,"rssi":-68,"connType":"WiFi","kayakPresent":true,"batteryPercent":87,"fwVersion":"1.3.0-ota"}
  ```

- /station/{machineId}/event

  JSON

  ```
  {"event":"taken"|"returned","bookingId":"bk_abc123"}
  ```

**Always use this contract** when coding firmware, backend webhooks or admin dashboard. 



Last updated: 17 March 2026