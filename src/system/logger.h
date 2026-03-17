#ifndef LOGGER_H
#define LOGGER_H

#include <esp_log.h>

/**
 * @brief Initializes logger with config-based level.
 * @note Sets ESP log level; logs initialization.
 *       Uses config for log level.
 */
void logger_init();

/**
 * @brief Logs to file if log level allows.
 * @param tag Log tag.
 * @param message Log message.
 * @note Adds timestamp; rotates file if size exceeds limit. Logs file errors.
 *       Uses SPIFFS for file storage.
 */
void logger_log_to_file(const char* tag, const char* message);

/**
 * @brief Rotates log file.
 * @note Renames file; logs success/failure.
 *       Prevents log file from growing too large.
 */
void logger_rotate_log_file();

#endif // LOGGER_H
