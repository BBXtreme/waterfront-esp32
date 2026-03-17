#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/portmacro.h>
#include <cJSON.h>
#include "config.h"

#define JSON_BUFFER_SIZE 4096

extern portMUX_TYPE g_configMutex;
extern GlobalConfig g_config;

/**
 * @brief Loads config from LittleFS JSON file.
 * @return True on success, false on failure (uses defaults).
 * @note Handles missing files, invalid JSON, validation failures. Logs detailed errors for debugging. Thread-safe.
 */
bool loadConfig();

/**
 * @brief Saves current config to LittleFS JSON file.
 * @return True on success, false on failure.
 * @note Serializes to JSON; logs errors. Thread-safe.
 */
bool saveConfig();

/**
 * @brief Updates config from JSON payload.
 * @param jsonPayload JSON string payload.
 * @return True on success, false on failure.
 * @note Validates JSON; reloads config. Logs errors. Handles edge cases like empty payload.
 */
bool updateConfigFromJson(const char* jsonPayload);

/**
 * @brief Gets default config values.
 * @return Default GlobalConfig struct.
 * @note Used for fallbacks; logs generation.
 */
GlobalConfig getDefaultConfig();

/**
 * @brief Serializes current config to JSON string.
 * @return JSON string (caller must free with cJSON_free).
 * @note Useful for MQTT publishing. Thread-safe.
 */
const char* getConfigAsJson();

/**
 * @brief Validates config values.
 * @param cfg Config to validate.
 * @return True if valid, false otherwise.
 * @note Checks bounds/types; logs specific errors for debugging.
 */
bool validateConfig(const GlobalConfig& cfg);

#endif
