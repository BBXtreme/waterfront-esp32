// src/main.cpp – Waterfront ESP32 MQTT Starter (PlatformIO + Arduino)
// Updated to use config for WiFi and MQTT, added loadConfig call.
// Note: This uses Arduino libraries; ensure PlatformIO framework is set to arduino.

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "config.h"

// Global objects
WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastStatus = 0;
unsigned long lastReconnectAttempt = 0;

// Forward declarations
void mqttCallback(char* topic, byte* payload, unsigned int length);
void connectMQTT();
void publishStatus();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Waterfront ESP32 Boot ===");
  Serial.printf("Machine ID: %s\n", MACHINE_ID);

  // Load config
  if (!loadConfig()) {
    Serial.println("Failed to load config, using defaults");
  }

  // WiFi setup using config
  vPortEnterCritical(&g_configMutex);
  WiFi.mode(WIFI_STA);
  WiFi.begin(g_config.wifiProvisioning.fallbackSsid, g_config.wifiProvisioning.fallbackPass);
  vPortExitCritical(&g_configMutex);
  Serial.print("Connecting WiFi...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected → " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi failed – continuing anyway for testing");
  }

  // MQTT setup using config
  vPortEnterCritical(&g_configMutex);
  mqtt.setServer(g_config.mqtt.broker, g_config.mqtt.port);
  mqtt.setCallback(mqttCallback);
  vPortExitCritical(&g_configMutex);
  mqtt.setBufferSize(512);  // enough for JSON payloads

  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(RELAY_LOCK_PIN, OUTPUT);
  digitalWrite(RELAY_LOCK_PIN, LOW);   // assume LOW = locked
  digitalWrite(STATUS_LED_PIN, LOW);
}

void loop() {
  if (!mqtt.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > MQTT_RECONNECT_MS) {
      lastReconnectAttempt = now;
      if (connectMQTT()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    mqtt.loop();
  }

  // Periodic status publish
  unsigned long now = millis();
  if (now - lastStatus > STATUS_PUBLISH_MS) {
    lastStatus = now;
    publishStatus();
  }
}

// Connect to MQTT broker using config
bool connectMQTT() {
  Serial.print("Connecting MQTT...");
  vPortEnterCritical(&g_configMutex);
  String clientId = g_config.mqtt.clientIdPrefix + "-" + String(random(0xffff), HEX);
  bool result = mqtt.connect(clientId.c_str(), g_config.mqtt.username, g_config.mqtt.password);
  vPortExitCritical(&g_configMutex);
  if (result) {
    Serial.println("connected!");
    
    // Subscribe to unlock & return commands
    mqtt.subscribe(TOPIC_UNLOCK);
    mqtt.subscribe(TOPIC_RETURN_CONFIRM);
    mqtt.subscribe(TOPIC_DEPOSIT_RELEASE);
    
    // Announce online
    StaticJsonDocument<128> doc;
    doc["state"] = "online";
    doc["machineId"] = MACHINE_ID;
    char buf[128];
    serializeJson(doc, buf);
    mqtt.publish(TOPIC_STATUS, buf, true);  // retained
    return true;
  } else {
    Serial.printf("failed, rc=%d → retry\n", mqtt.state());
    return false;
  }
}

// Handle incoming MQTT messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  Serial.printf("MQTT rx → %s\n", topic);

  // Basic payload → string (later parse JSON)
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Payload: " + message);

  // === TODO: real logic ===
  if (topicStr == TOPIC_UNLOCK) {
    Serial.println("→ UNLOCK command received!");
    digitalWrite(RELAY_LOCK_PIN, HIGH);   // unlock (adjust logic)
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(500);                           // pulse example
    digitalWrite(RELAY_LOCK_PIN, LOW);
    digitalWrite(STATUS_LED_PIN, LOW);
  }
  // Add returnConfirm, depositRelease handling later
}

// Publish periodic status (expand later with sensors, battery, etc.)
void publishStatus() {
  StaticJsonDocument<256> doc;
  doc["state"] = mqtt.connected() ? "connected" : "disconnected";
  doc["uptimeMin"] = millis() / 60000;
  doc["wifiRssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : -999;
  doc["machineId"] = MACHINE_ID;

  char buf[256];
  serializeJson(doc, buf);
  mqtt.publish(TOPIC_STATUS, buf);
  Serial.println("Status published");
}
