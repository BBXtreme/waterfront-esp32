#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstring>

#define MAX_COMPARTMENTS 10

// Global config struct for WATERFRONT ESP32 system
// This struct holds all configuration parameters loaded from LittleFS JSON.
// It includes MQTT, WiFi, LTE, BLE, compartment, system, and other settings.
// Thread-safe access via g_configMutex.
// Defaults are provided in getDefaultConfig() for fallback.
struct GlobalConfig {
    char version[16];  // Configuration version for migration (e.g., "1.0")

    // MQTT configuration section
    struct {
        char broker[64];       // MQTT broker URL (e.g., "mqtt.example.com")
        int port;              // MQTT port (e.g., 8883 for TLS, 1883 for non-TLS)
        char username[32];     // MQTT username for authentication
        char password[32];     // MQTT password for authentication (consider encryption in production)
        char clientIdPrefix[32]; // Prefix for MQTT client ID (e.g., "waterfront")
        bool useTLS;           // Enable TLS for secure MQTT connection
        char caCertPath[64];   // Path to CA certificate file for TLS
        char clientCertPath[64]; // Path to client certificate file for TLS
        char clientKeyPath[64]; // Path to client key file for TLS
    } mqtt;

    // Location configuration section
    struct {
        char slug[32];         // Location slug (e.g., "bremen" for URL-friendly identifier)
        char code[32];         // Location code (e.g., "harbor-01" for unique identifier)
    } location;

    // WiFi provisioning configuration section
    struct {
        char fallbackSsid[32]; // Fallback WiFi SSID for provisioning
        char fallbackPass[32]; // Fallback WiFi password for provisioning
    } wifiProvisioning;

    // LTE configuration section
    struct {
        char apn[32];          // LTE APN (Access Point Name) for cellular connection
        char simPin[16];       // SIM PIN if required for LTE
        int rssiThreshold;     // RSSI threshold for LTE signal strength (e.g., -70 dBm)
        int dataUsageAlertLimitKb; // Data usage alert limit in KB
    } lte;

    // BLE configuration section
    struct {
        char serviceUuid[37];  // BLE service UUID (e.g., "12345678-1234-1234-1234-123456789abc")
        char ssidCharUuid[37]; // BLE characteristic UUID for SSID
        char passCharUuid[37]; // BLE characteristic UUID for password
        char statusCharUuid[37]; // BLE characteristic UUID for status
    } ble;

    // Compartment configuration array
    struct {
        int number;            // Compartment number (1-based index)
        int servoPin;          // GPIO pin for servo motor (0-39)
        int limitOpenPin;      // GPIO pin for open limit switch (0-39)
        int limitClosePin;     // GPIO pin for close limit switch (0-39)
        int ultrasonicTriggerPin; // GPIO pin for ultrasonic sensor trigger (0-39)
        int ultrasonicEchoPin; // GPIO pin for ultrasonic sensor echo (0-39)
        int weightSensorPin;   // GPIO pin for weight sensor (0-39)
    } compartments[MAX_COMPARTMENTS];

    int compartmentCount;      // Number of active compartments (1 to MAX_COMPARTMENTS)

    // System configuration section
    struct {
        int maxCompartments;   // Maximum compartments allowed (1-20)
        bool debugMode;        // Enable debug mode for verbose logging
        int gracePeriodSec;    // Grace period in seconds for rentals (0-86400)
        int batteryLowThresholdPercent; // Battery low threshold percentage (0-100)
        float solarVoltageMin; // Minimum solar voltage for operation (0.0-5.0V)
        int logLevel;          // ESP log level (0=ESP_LOG_NONE, 1=ESP_LOG_ERROR, 2=ESP_LOG_WARN, 3=ESP_LOG_INFO, 4=ESP_LOG_DEBUG, 5=ESP_LOG_VERBOSE)
        char otaPassword[32];  // Password for OTA updates
    } system;

    // Other configuration section
    struct {
        int offlinePinTtlSec;  // Offline PIN time-to-live in seconds
        int depositHoldAmountFallback; // Fallback deposit hold amount
    } other;
};

extern GlobalConfig g_config;
extern portMUX_TYPE g_configMutex;

#endif // CONFIG_H
