#include "mqtt_client.h"
#include "config_loader.h"
#include <esp_log.h>
#include <mqtt_client.h>
#include "cJSON.h"

#define TAG "MQTT"

esp_mqtt_client_handle_t mqttClient = NULL;
bool mqttConnected = false;

static void event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = event_data;
    if (event_id == MQTT_EVENT_CONNECTED) {
        mqttConnected = true;
        ESP_LOGI(TAG, "MQTT Connected");
        // Subscribe to topics here later
    } else if (event_id == MQTT_EVENT_DISCONNECTED) {
        mqttConnected = false;
        ESP_LOGW(TAG, "MQTT Disconnected");
    }
}

esp_err_t mqtt_init() {
    vPortEnterCritical(&g_configMutex);
    char uri[128];
    snprintf(uri, sizeof(uri), "%s://%s:%d", g_config.mqtt.useTLS ? "mqtts" : "mqtt", g_config.mqtt.broker, g_config.mqtt.port);
    esp_mqtt_client_config_t cfg = {
        .broker = {
            .address.uri = uri,
        },
        .credentials = {
            .username = g_config.mqtt.username,
            .password = g_config.mqtt.password,
        },
        .session = {
            .disable_clean_session = true,
        },
    };
    vPortExitCritical(&g_configMutex);

    mqttClient = esp_mqtt_client_init(&cfg);
    if (!mqttClient) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }
    esp_mqtt_client_register_event(mqttClient, ESP_EVENT_ANY_ID, event_handler, NULL);
    esp_err_t err = esp_mqtt_client_start(mqttClient);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MQTT client: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "MQTT client started");
    }
    return err;
}

void mqtt_publish_status() {
    if (!mqttConnected || !mqttClient) {
        ESP_LOGW(TAG, "MQTT not connected, skipping status publish");
        return;
    }
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        ESP_LOGE(TAG, "Failed to create JSON for status");
        return;
    }

    cJSON_AddStringToObject(root, "status", "online");
    cJSON_AddNumberToObject(root, "uptime", esp_timer_get_time() / 1000000);

    char* str = cJSON_PrintUnformatted(root);
    if (!str) {
        ESP_LOGE(TAG, "Failed to serialize JSON for status");
        cJSON_Delete(root);
        return;
    }
    int msg_id = esp_mqtt_client_publish(mqttClient, "waterfront/status", str, 0, 1, 0);
    if (msg_id >= 0) {
        ESP_LOGD(TAG, "Published status, msg_id=%d", msg_id);
    } else {
        ESP_LOGE(TAG, "Failed to publish status");
    }
    cJSON_free(str);
    cJSON_Delete(root);
}

// Stub other functions
void mqtt_publish_slot_status(int slotId, const char* jsonPayload) {
    if (!mqttConnected || !mqttClient || !jsonPayload) {
        ESP_LOGW(TAG, "MQTT not connected or invalid payload, skipping slot status publish for slot %d", slotId);
        return;
    }
    int msg_id = esp_mqtt_client_publish(mqttClient, "waterfront/slot/status", jsonPayload, 0, 1, 0);
    if (msg_id >= 0) {
        ESP_LOGD(TAG, "Published slot status for %d, msg_id=%d", slotId, msg_id);
    } else {
        ESP_LOGE(TAG, "Failed to publish slot status for %d", slotId);
    }
}
void mqtt_publish_retained_status(int compartmentId, const char* jsonPayload) {
    if (!mqttConnected || !mqttClient || !jsonPayload) {
        ESP_LOGW(TAG, "MQTT not connected or invalid payload, skipping retained status publish for compartment %d", compartmentId);
        return;
    }
    int msg_id = esp_mqtt_client_publish(mqttClient, "waterfront/compartment/status", jsonPayload, 0, 1, 1);  // Retained
    if (msg_id >= 0) {
        ESP_LOGD(TAG, "Published retained status for %d, msg_id=%d", compartmentId, msg_id);
    } else {
        ESP_LOGE(TAG, "Failed to publish retained status for %d", compartmentId);
    }
}
void mqtt_publish_ack(int compartmentId, const char* action) {
    if (!mqttConnected || !mqttClient || !action) {
        ESP_LOGW(TAG, "MQTT not connected or invalid action, skipping ack publish for compartment %d", compartmentId);
        return;
    }
    cJSON* root = cJSON_CreateObject();
    if (!root) return;
    cJSON_AddNumberToObject(root, "compartmentId", compartmentId);
    cJSON_AddStringToObject(root, "action", action);
    char* str = cJSON_PrintUnformatted(root);
    if (!str) {
        cJSON_Delete(root);
        return;
    }
    int msg_id = esp_mqtt_client_publish(mqttClient, "waterfront/ack", str, 0, 0, 0);
    if (msg_id >= 0) {
        ESP_LOGD(TAG, "Published ack for %d, msg_id=%d", compartmentId, msg_id);
    } else {
        ESP_LOGE(TAG, "Failed to publish ack for %d", compartmentId);
    }
    cJSON_free(str);
    cJSON_Delete(root);
}
bool mqtt_connect() { return mqttConnected; }
void mqtt_subscribe() {
    if (!mqttConnected || !mqttClient) {
        ESP_LOGW(TAG, "MQTT not connected, skipping subscribe");
        return;
    }
    // Add subscriptions here if needed
    ESP_LOGI(TAG, "MQTT subscriptions set up");
}
void mqtt_loop() {
    // ESP-IDF handles loop internally
}
void mqtt_loop_task(void *pvParameters) { vTaskDelete(NULL); }
int getMqttReconnectCount() { return 0; }  // Stub, implement if needed
