/**
 * @file hal_i2c.h
 * @brief Hardware Abstraction Layer - I²C Interface
 * @layer HAL (Hardware Abstraction Layer)
 * 
 * Provides platform-agnostic I²C communication primitives.
 * This abstraction enables host-based testing with stubs.
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Status Codes
 * -------------------------------------------------------------------------- */

typedef enum {
    HAL_I2C_OK = 0,         /**< Operation successful */
    HAL_I2C_ERR_INIT,       /**< Initialization error */
    HAL_I2C_ERR_NACK,       /**< Device did not ACK */
    HAL_I2C_ERR_TIMEOUT,    /**< Operation timeout */
    HAL_I2C_ERR_BUS,        /**< Bus error */
    HAL_I2C_ERR_INVALID     /**< Invalid argument */
} hal_i2c_status_t;

/* --------------------------------------------------------------------------
 * Configuration
 * -------------------------------------------------------------------------- */

typedef struct {
    uint32_t clock_speed_hz;  /**< I²C clock speed (100000 or 400000) */
    uint8_t  port;            /**< I²C port number */
} hal_i2c_config_t;

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

/**
 * @brief Initialize I²C peripheral
 * @param config I²C configuration
 * @return HAL_I2C_OK on success
 */
hal_i2c_status_t HAL_I2C_Init(const hal_i2c_config_t *config);

/**
 * @brief Check if a device ACKs at the given address
 * @param addr 7-bit I²C device address
 * @return HAL_I2C_OK if device ACKs, HAL_I2C_ERR_NACK otherwise
 */
hal_i2c_status_t HAL_I2C_Probe(uint8_t addr);

/**
 * @brief Write data to an I²C device
 * @param addr 7-bit device address
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return HAL_I2C_OK on success
 */
hal_i2c_status_t HAL_I2C_Write(uint8_t addr, const uint8_t *data, size_t len);

/**
 * @brief Read data from an I²C device
 * @param addr 7-bit device address
 * @param data Pointer to receive buffer
 * @param len Number of bytes to read
 * @return HAL_I2C_OK on success
 */
hal_i2c_status_t HAL_I2C_Read(uint8_t addr, uint8_t *data, size_t len);

/**
 * @brief Write then read (combined transaction)
 * @param addr 7-bit device address
 * @param tx_data Data to write
 * @param tx_len Number of bytes to write
 * @param rx_data Buffer for read data
 * @param rx_len Number of bytes to read
 * @return HAL_I2C_OK on success
 */
hal_i2c_status_t HAL_I2C_WriteRead(uint8_t addr,
                                    const uint8_t *tx_data, size_t tx_len,
                                    uint8_t *rx_data, size_t rx_len);

/**
 * @brief De-initialize I²C peripheral
 */
void HAL_I2C_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_I2C_H */
