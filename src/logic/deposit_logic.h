#ifndef DEPOSIT_LOGIC_H
#define DEPOSIT_LOGIC_H

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <mqtt_client.h>

#define MAX_TIMERS 10 ///< Maximum number of active timers

struct RentalTimer {
    int compartmentId; ///< ID of the compartment
    unsigned long startMs; ///< Start time in milliseconds
    unsigned long durationSec; ///< Duration in seconds
    TimerHandle_t timerHandle; ///< FreeRTOS timer handle
};

extern RentalTimer activeTimers[MAX_TIMERS]; ///< Array of active rental timers
extern int activeTimersCount; ///< Number of active timers

/**
 * @brief Initializes deposit logic.
 * @note Resets state; thread-safe with mutex. Logs initialization.
 */
void deposit_init();

/**
 * @brief Starts a rental timer for a compartment.
 * @param compartmentId The compartment ID.
 * @param durationSec The rental duration in seconds.
 * @note Validates ID; creates FreeRTOS timer; logs errors on failure. Thread-safe.
 */
void startRental(int compartmentId, unsigned long durationSec);

/**
 * @brief Checks for overdue rentals.
 * @note Fallback check; removes expired timers; logs overdue events. Thread-safe.
 */
void checkOverdue();

/**
 * @brief Handles deposit on take action.
 * @param client MQTT client for publishing.
 * @note Sets deposit held; logs action. Validates client.
 */
void deposit_on_take(esp_mqtt_client_handle_t client);

/**
 * @brief Handles deposit on return action.
 * @param client MQTT client for publishing.
 * @note Checks elapsed time; releases deposit if on-time; publishes MQTT; logs. Validates client.
 */
void deposit_on_return(esp_mqtt_client_handle_t client);

/**
 * @brief Checks if deposit is held.
 * @return True if held, false otherwise.
 * @note Thread-safe with mutex.
 */
bool deposit_is_held();

#endif // DEPOSIT_LOGIC_H
