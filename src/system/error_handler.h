/**
 * @file error_handler.h
 * @brief Header for global error handling in WATERFRONT ESP32.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Provides fatal error function for production stability.
 *       Enhanced with detailed comments and edge case notes.
 */

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <esp_err.h>

/**
 * @brief Handles fatal errors by logging and restarting ESP32.
 * @param msg Descriptive error message (null-safe).
 * @param code ESP error code.
 * @note Logs error; publishes to MQTT debug/alert topics if connected/debug enabled; restarts after delay.
 *       Edge cases: MQTT not connected (skips publish), invalid code (logs as is), buffer overflow (logs error).
 */
void fatal_error(const char* msg, esp_err_t code);

#endif // ERROR_HANDLER_H
