#include "logger.h"
#include "config_loader.h"
#include <esp_spiffs.h>
#include <esp_log.h>

#define LOG_FILE "/spiffs/system.log"
#define LOG_MAX_SIZE 102400  // 100KB

// Initialize logger
void logger_init() {
    // Set log level from config
    vPortEnterCritical(&g_configMutex);
    esp_log_level_set("*", (esp_log_level_t)g_config.system.logLevel);
    vPortExitCritical(&g_configMutex);
    ESP_LOGI("LOGGER", "Logger initialized with level %d", g_config.system.logLevel);
}

// Log to file if verbose (logLevel >= INFO)
void logger_log_to_file(const char* tag, const char* message) {
    vPortEnterCritical(&g_configMutex);
    if (g_config.system.logLevel >= ESP_LOG_INFO) {
        vPortExitCritical(&g_configMutex);
        FILE *logFile = fopen(LOG_FILE, "a");
        if (logFile) {
            // Add timestamp
            time_t now = time(NULL);
            char timeStr[20];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
            fprintf(logFile, "[%s] %s: %s\n", timeStr, tag, message);
            fclose(logFile);
            // Check size and rotate
            struct stat st;
            if (stat(LOG_FILE, &st) == 0 && st.st_size > LOG_MAX_SIZE) {
                logger_rotate_log_file();
            }
        } else {
            ESP_LOGE("LOGGER", "Failed to open log file for writing");
        }
    } else {
        vPortExitCritical(&g_configMutex);
    }
}

// Rotate log file
void logger_rotate_log_file() {
    esp_err_t ret = esp_spiffs_rename(LOG_FILE, LOG_FILE ".1");
    if (ret == ESP_OK) {
        ESP_LOGI("LOGGER", "Log file rotated successfully");
    } else {
        ESP_LOGE("LOGGER", "Failed to rotate log file: %s", esp_err_to_name(ret));
    }
}
