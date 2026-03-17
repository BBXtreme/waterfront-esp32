#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstring>

#define MAX_COMPARTMENTS 10

// Global config struct
struct GlobalConfig {
    char version[16];  // Config version for migration

    struct {
        char broker[64];       // MQTT broker URL
        int port;              // MQTT port (e.g., 8883 for TLS)
        char username[32];     // MQTT username
        char password[32];     // MQTT password (consider encryption in production)
        char clientIdPrefix[32]; // Prefix for client ID
        bool useTLS;           // Enable TLS for MQTT
        char caCertPath[64];   // Path to CA certificate
        char clientCertPath[64]; // Path to client certificate
        char clientKeyPath[64]; // Path to client key
    } mqtt;

    struct {
        char slug[32];         // Location slug (e.g., "bremen")
        char code[32];         // Location code (e.g., "harbor-01")
    } location;

    struct {
        char fallbackSsid[32]; // Fallback WiFi SSID
        char fallbackPass[32]; // Fallback WiFi password
    } wifiProvisioning;

    struct {
        char apn[32];          // LTE APN
        char simPin[16];       // SIM PIN (if required)
        int rssiThreshold;     // RSSI threshold for LTE (e.g., -70 dBm)
        int dataUsageAlertLimitKb; // Data usage alert limit in KB
    } lte;

    struct {
        char serviceUuid[37];  // BLE service UUID
        char ssidCharUuid[37]; // SSID characteristic UUID
        char passCharUuid[37]; // Password characteristic UUID
        char statusCharUuid[37]; // Status characteristic UUID
    } ble;

    struct {
        int number;            // Compartment number (1-based)
        int servoPin;          // GPIO pin for servo (0-39)
        int limitOpenPin;      // GPIO pin for open limit switch (0-39)
        int limitClosePin;     // GPIO pin for close limit switch (0-39)
        int ultrasonicTriggerPin; // GPIO pin for ultrasonic trigger (0-39)
        int ultrasonicEchoPin; // GPIO pin for ultrasonic echo (0-39)
        int weightSensorPin;   // GPIO pin for weight sensor (0-39)
    } compartments[MAX_COMPARTMENTS];

    int compartmentCount;      // Number of active compartments (1 to MAX_COMPARTMENTS)

    struct {
        int maxCompartments;   // Max compartments allowed (1-20)
        bool debugMode;        // Enable debug mode
        int gracePeriodSec;    // Grace period in seconds (0-86400)
        int batteryLowThresholdPercent; // Battery low threshold (0-100)
        float solarVoltageMin; // Min solar voltage (0.0-5.0V)
        int logLevel;          // ESP log level (0-5)
        char otaPassword[32];  // OTA password
    } system;

    struct {
        int offlinePinTtlSec;  // Offline PIN TTL in seconds
        int depositHoldAmountFallback; // Fallback deposit amount
    } other;
};

extern GlobalConfig g_config;
extern portMUX_TYPE g_configMutex;

#endif // CONFIG_H
