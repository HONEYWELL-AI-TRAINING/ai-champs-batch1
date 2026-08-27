/**
 * @file sensor.h
 * @brief Sensor Acquisition Module - BME680 Environmental Sensor Driver
 * @requirement HVACOPT-01 Sensor Acquisition
 * @jira HVAC-23
 * 
 * This module reads environmental data from the BME680 sensor via I²C
 * and exposes the latest readings through a thread-safe API.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------- */

/** BME680 I²C address (default) */
#define SENSOR_BME680_ADDR      0x76

/** Polling interval in milliseconds */
#define SENSOR_POLL_INTERVAL_MS 1000

/** Maximum jitter tolerance for polling (±10ms) */
#define SENSOR_POLL_JITTER_MS   10

/* --------------------------------------------------------------------------
 * Status Codes
 * -------------------------------------------------------------------------- */

typedef enum {
    SENSOR_OK = 0,          /**< Operation successful */
    SENSOR_ERR_INIT,        /**< Initialization failed */
    SENSOR_ERR_I2C,         /**< I²C communication error */
    SENSOR_ERR_TIMEOUT,     /**< Operation timeout */
    SENSOR_ERR_INVALID_ARG, /**< Invalid argument */
    SENSOR_FAULT            /**< Sensor fault detected */
} sensor_status_t;

/* --------------------------------------------------------------------------
 * Data Structures
 * -------------------------------------------------------------------------- */

/**
 * @brief Environmental sensor reading with timestamp
 * 
 * Contains temperature, humidity, CO₂-equivalent, and VOC index
 * readings from the BME680 sensor.
 */
typedef struct {
    int16_t  temperature_c_x100;  /**< Temperature in °C × 100 (e.g., 2350 = 23.50°C) */
    uint16_t humidity_pct_x10;    /**< Relative humidity in % × 10 (e.g., 655 = 65.5%) */
    uint16_t co2_ppm;             /**< CO₂-equivalent in ppm */
    uint16_t voc_index;           /**< VOC index (0-500 scale) */
    uint32_t timestamp_ms;        /**< Monotonic timestamp in milliseconds */
    bool     valid;               /**< True if reading is valid */
} sensor_reading_t;

/**
 * @brief Sensor module configuration
 */
typedef struct {
    uint8_t  i2c_addr;            /**< I²C device address */
    uint32_t poll_interval_ms;    /**< Polling interval in milliseconds */
} sensor_config_t;

/* --------------------------------------------------------------------------
 * Public API - Acceptance Criteria AC-01.1 through AC-01.4
 * -------------------------------------------------------------------------- */

/**
 * @brief Initialize the BME680 sensor over I²C
 * @param config Sensor configuration (NULL for defaults)
 * @return SENSOR_OK if device ACKs at expected address, error code otherwise
 * 
 * @acceptance AC-01.1: Returns SENSOR_OK when the device ACKs at the expected address
 */
sensor_status_t Sensor_Init(const sensor_config_t *config);

/**
 * @brief Read current sensor values
 * @param[out] reading Pointer to store the sensor reading
 * @return SENSOR_OK on success, SENSOR_FAULT if I²C transaction fails
 * 
 * @acceptance AC-01.2: Retrieves temperature, humidity, CO₂-equivalent, and VOC index
 * @acceptance AC-01.3: On I²C failure, sets internal SENSOR_FAULT flag
 */
sensor_status_t Sensor_Read(sensor_reading_t *reading);

/**
 * @brief Get the last valid sensor reading
 * @param[out] reading Pointer to store the last valid reading
 * @return SENSOR_OK if valid reading available, SENSOR_FAULT otherwise
 * 
 * @acceptance AC-01.3: Last-known-good values remain accessible via this function
 */
sensor_status_t Sensor_GetLastValid(sensor_reading_t *reading);

/**
 * @brief Check if sensor is in fault state
 * @return true if SENSOR_FAULT flag is set, false otherwise
 */
bool Sensor_IsFaulted(void);

/**
 * @brief Clear the sensor fault flag
 * @return SENSOR_OK on success
 */
sensor_status_t Sensor_ClearFault(void);

/**
 * @brief Periodic polling function (call from scheduler at 1Hz)
 * @return SENSOR_OK on successful poll
 * 
 * @acceptance AC-01.4: Polling occurs at 1 Hz ± 10 ms jitter when invoked from scheduler
 */
sensor_status_t Sensor_Poll(void);

/**
 * @brief De-initialize the sensor module
 */
void Sensor_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H */
