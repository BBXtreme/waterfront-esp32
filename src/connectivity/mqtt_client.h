#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <stdbool.h>          // bool in C
#include <esp_event.h>
#include <esp_mqtt_client.h>

#ifdef __cplusplus
extern "C" {
#endif

extern esp_mqtt_client_handle_t mqttClient;
extern bool mqttConnected;

/**
 * @brief Initializes the MQTT client with config-based settings.
 * @return ESP_OK on success, error code otherwise.
 * @note Thread-safe, uses config for broker/credentials. Logs errors for debugging.
 */
esp_err_t mqtt_init(void);

/**
 * @brief Event handler for MQTT events (connection, disconnection, errors).
 * @param handler_args Handler arguments.
 * @param base Event base.
 * @param event_id Event ID.
 * @param event_data Event data.
 * @note Logs events for debugging; updates mqttConnected flag.
 */
void event_handler(void *handler_args,
                   esp_event_base_t base,
                   int32_t event_id,
                   void *event_data);

/**
 * @brief Publishes status to MQTT.
 * @note Creates JSON payload with uptime; publishes to "waterfront/status". Logs on failure.
 */
void mqtt_publish_status(void);

/**
 * @brief Publishes slot status to MQTT.
 * @param slotId Slot ID.
 * @param jsonPayload JSON payload string.
 * @note Publishes to "waterfront/slot/status". Checks for connection; logs errors.
 */
void mqtt_publish_slot_status(int slotId, const char *jsonPayload);

/**
 * @brief Publishes retained compartment status to MQTT.
 * @param compartmentId Compartment ID.
 * @param jsonPayload JSON payload string.
 * @note Publishes retained to "waterfront/compartment/status". Checks for connection; logs errors.
 */
void mqtt_publish_retained_status(int compartmentId,
                                  const char *jsonPayload);

/**
 * @brief Publishes acknowledgment to MQTT.
 * @param compartmentId Compartment ID.
 * @param action Action string.
 * @note Creates JSON payload; publishes to "waterfront/ack". Logs errors.
 */
void mqtt_publish_ack(int compartmentId, const char *action);

/**
 * @brief Checks if MQTT is connected.
 * @return True if connected, false otherwise.
 */
bool mqtt_connect(void);

/**
 * @brief Subscribes to MQTT topics.
 * @note Currently a stub; add subscriptions here. Logs setup.
 */
void mqtt_subscribe(void);

/**
 * @brief MQTT loop (ESP-IDF handles internally).
 * @note Stub for compatibility.
 */
void mqtt_loop(void);

/**
 * @brief MQTT loop task (deletes itself).
 * @param pvParameters Task parameters.
 * @note Stub; not used in ESP-IDF.
 */
void mqtt_loop_task(void *pvParameters);

/**
 * @brief Gets MQTT reconnect count.
 * @return Reconnect count (stub, implement if needed).
 */
int getMqttReconnectCount(void);

#ifdef __cplusplus
}
#endif

#endif // MQTT_CLIENT_H
