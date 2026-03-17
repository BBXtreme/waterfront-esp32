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
 *       Registers event handler for connection management.
 */
esp_err_t mqtt_init(void);

/**
 * @brief Event handler for MQTT events (connection, disconnection, errors).
 * @param handler_args Handler arguments (unused).
 * @param base Event base (MQTT_EVENT).
 * @param event_id Event ID (e.g., MQTT_EVENT_CONNECTED).
 * @param event_data Event data (esp_mqtt_event_handle_t).
 * @note Logs events for debugging; updates mqttConnected flag.
 *       Handles various events like connect, disconnect, error, subscribe, etc.
 */
void event_handler(void *handler_args,
                   esp_event_base_t base,
                   int32_t event_id,
                   void *event_data);

/**
 * @brief Publishes status to MQTT.
 * @note Creates JSON payload with uptime; publishes to "waterfront/{location}/status". Logs on failure.
 *       Checks connection before publishing.
 */
void mqtt_publish_status(void);

/**
 * @brief Publishes slot status to MQTT.
 * @param slotId Slot ID.
 * @param jsonPayload JSON payload string.
 * @note Publishes to "waterfront/{location}/slot/status". Checks for connection; logs errors.
 *       Validates payload and topic length.
 */
void mqtt_publish_slot_status(int slotId, const char *jsonPayload);

/**
 * @brief Publishes retained compartment status to MQTT.
 * @param compartmentId Compartment ID.
 * @param jsonPayload JSON payload string.
 * @note Publishes retained to "waterfront/{location}/compartment/status". Checks for connection; logs errors.
 *       Validates payload and topic length.
 */
void mqtt_publish_retained_status(int compartmentId,
                                  const char *jsonPayload);

/**
 * @brief Publishes acknowledgment to MQTT.
 * @param compartmentId Compartment ID.
 * @param action Action string (e.g., "open", "close").
 * @note Creates JSON payload; publishes to "waterfront/{location}/ack". Logs errors.
 *       Validates action and topic length.
 */
void mqtt_publish_ack(int compartmentId, const char *action);

/**
 * @brief Checks if MQTT is connected.
 * @return True if connected, false otherwise.
 * @note Returns the current connection state.
 */
bool mqtt_connect(void);

/**
 * @brief Subscribes to MQTT topics.
 * @note Currently a stub; add subscriptions here. Logs setup.
 *       Should be called after connection.
 */
void mqtt_subscribe(void);

/**
 * @brief MQTT loop (ESP-IDF handles internally).
 * @note Stub for compatibility with Arduino-style code.
 */
void mqtt_loop(void);

/**
 * @brief MQTT loop task (deletes itself).
 * @param pvParameters Task parameters (unused).
 * @note Stub; not used in ESP-IDF.
 */
void mqtt_loop_task(void *pvParameters);

/**
 * @brief Gets MQTT reconnect count.
 * @return Reconnect count (stub, implement if needed).
 * @note Placeholder for future implementation.
 */
int getMqttReconnectCount(void);

#ifdef __cplusplus
}
#endif

#endif // MQTT_CLIENT_H
