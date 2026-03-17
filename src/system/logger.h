#ifndef LOGGER_H
#define LOGGER_H

#include <esp_log.h>

/**
 * @brief Initializes logger with config-based level.
 * @note Sets ESP log level; logs initialization.
 */
void logger_init();

/**
 * @brief Logs to file if log level allows.
 * @param tag Log tag.
 * @param message Log message.
 * @note Adds timestamp; rotates file if size exceeds limit. Logs file errors.
 */
void logger_log_to_file(const char* tag, const char* message);

/**
 * @brief Rotates log file.
 * @note Renames file; logs success/failure.
 */
void logger_rotate_log_file();

#endif // LOGGER_H
