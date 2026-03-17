# WATERFRONT ESP32 API Reference

## Introduction
This guide explains how the WATERFRONT system talks to other devices using MQTT. MQTT is a simple way for devices to send messages over the internet, like texting but for machines.

## What is MQTT?
- **Topics**: Like chat rooms. Devices subscribe (listen) to topics and publish (send) to topics.
- **Messages**: JSON text (easy to read) with data.
- **Security**: Uses TLS encryption for safety.
- **Reliability**: Messages are acknowledged, with checksums for accuracy.

## MQTT Topics

### Commands You Send to the System
These are messages you publish to control the WATERFRONT device.

- `waterfront/{location}/{code}/command`
  - **Purpose**: Send general commands.
  - **Example Topic**: `waterfront/bremen/harbor-01/command`
  - **Example Payload**: `{"action": "open_gate", "compartment": 1}`
  - **What It Does**: Opens gate for compartment 1.

- `waterfront/{location}/{code}/config/update`
  - **Purpose**: Update device settings.
  - **Example Payload**: Full JSON config (see System Spec).
  - **What It Does**: Saves new config and restarts device.

- `waterfront/{location}/{code}/ota/update`
  - **Purpose**: Update software.
  - **Example Payload**: `"http://example.com/firmware.bin"`
  - **What It Does**: Downloads and installs new firmware.

- `waterfront/{location}/{code}/booking/paid`
  - **Purpose**: Start a rental.
  - **Example Payload**: `{"bookingId": "bk_123", "compartmentId": 1, "durationSec": 3600}`
  - **What It Does**: Opens gate, starts timer for 1 hour rental.

- `waterfront/{location}/{code}/compartments/+/command`
  - **Purpose**: Command specific compartments (+ is wildcard for any number).
  - **Example Topic**: `waterfront/bremen/harbor-01/compartments/1/command`
  - **Example Payload**: `"open_gate"`
  - **What It Does**: Opens gate 1.

- `waterfront/{location}/{code}/compartments/+/status`
  - **Purpose**: Send status updates (retained for sync).
  - **Example Payload**: `{"booked": true, "gateState": "locked", "crc": 1234567890}`

### Responses and Updates from the System
These are messages the device sends back.

- `waterfront/{location}/{code}/response`
  - **Purpose**: Reply to commands.
  - **Example Payload**: `{"ack": true, "action": "open_gate", "compartment": 1, "status": "success", "timestamp": 1234567890}`

- `waterfront/{location}/{code}/compartments/+/ack`
  - **Purpose**: Confirm actions.
  - **Example Payload**: `{"compartmentId":1,"action":"gate_opened","timestamp":1234567890,"crc":987654321}`

- `waterfront/{location}/{code}/debug`
  - **Purpose**: Health reports every 60 seconds.
  - **Example Payload**: `{"uptime":3600,"heapFree":204800,"fwVersion":"0.9.2-beta","batteryPercent":85,"tasks":5,"reconnects":2}`
  - **Fields**:
    - `uptime`: Seconds since start.
    - `heapFree`: Free memory in bytes.
    - `fwVersion`: Software version.
    - `batteryPercent`: Battery level (0-100).
    - `tasks`: Number of running tasks.
    - `reconnects`: MQTT reconnection count.

- `waterfront/{location}/{code}/alert`
  - **Purpose**: Error alerts.
  - **Example Payload**: `{"alert":"low_power","batteryPercent":15,"solarVoltage":2.5,"timestamp":1234567890}`

- `waterfront/machine/{code}/status`
  - **Purpose**: Device status.
  - **Example Payload**: `{"state": "idle", "battery": 92, "connType": "wifi"}`

- `waterfront/slots/{id}/status`
  - **Purpose**: Compartment status.
  - **Example Payload**: `{"slotId": 1, "state": "open", "booked": false}`

- `waterfront/locations/{code}/depositRelease`
  - **Purpose**: Release held deposits.
  - **Example Payload**: `{"bookingId": "bk_123", "release": true}`

- `waterfront/locations/{code}/returnConfirm`
  - **Purpose**: Confirm returns.
  - **Example Payload**: `{"bookingId": "bk_123", "action": "autoLock"}`

- `waterfront/{location}/{code}/ota/status`
  - **Purpose**: Update results.
  - **Example Payload**: `{"otaResult": "success", "firmwareVersion": "0.9.2-beta"}`

## Message Examples

### Opening a Gate
1. You send: Topic `waterfront/bremen/harbor-01/compartments/1/command`, Payload `"open_gate"`
2. System responds: Topic `waterfront/bremen/harbor-01/compartments/1/ack`, Payload `{"compartmentId":1,"action":"gate_opened","timestamp":1234567890,"crc":987654321}`

### Health Check
- System sends every 60s: Topic `waterfront/bremen/harbor-01/debug`, Payload `{"uptime":3600,"heapFree":204800,"fwVersion":"0.9.2-beta","batteryPercent":85,"tasks":5,"reconnects":2}`

### Low Power Alert
- System sends: Topic `waterfront/bremen/harbor-01/alert`, Payload `{"alert":"low_power","batteryPercent":15,"solarVoltage":2.5,"timestamp":1234567890}`

## Checksums
All messages include a CRC32 checksum to ensure data integrity. If checksum fails, message is ignored.

## Security
- Uses TLS for encrypted connections.
- Optional client certificates for extra security.
