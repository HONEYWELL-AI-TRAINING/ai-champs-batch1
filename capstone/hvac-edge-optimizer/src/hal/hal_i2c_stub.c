/**
 * @file hal_i2c_stub.c
 * @brief I²C HAL Stub for Host-Based Testing
 * 
 * This stub implementation allows building and testing on the host
 * without actual hardware. Replace with real HAL for target MCU.
 */

#include "hal_i2c.h"
#include <string.h>

#ifdef UNIT_TEST

/* --------------------------------------------------------------------------
 * Mock State for Testing
 * -------------------------------------------------------------------------- */

static struct {
    bool initialized;
    uint8_t probe_response[128];  /* 1 = ACK, 0 = NACK for each address */
    uint8_t registers[256];       /* Simulated device registers */
    bool force_error;
    hal_i2c_status_t next_error;
} g_mock_i2c;

/* Test hooks */
uint32_t mock_tick_ms = 0;

void mock_i2c_set_probe_response(uint8_t addr, bool ack)
{
    g_mock_i2c.probe_response[addr & 0x7F] = ack ? 1 : 0;
}

void mock_i2c_set_register(uint8_t reg, uint8_t value)
{
    g_mock_i2c.registers[reg] = value;
}

void mock_i2c_force_error(hal_i2c_status_t error)
{
    g_mock_i2c.force_error = true;
    g_mock_i2c.next_error = error;
}

void mock_i2c_clear_error(void)
{
    g_mock_i2c.force_error = false;
}

void mock_i2c_reset(void)
{
    memset(&g_mock_i2c, 0, sizeof(g_mock_i2c));
    g_mock_i2c.initialized = true;
    /* Default: BME680 at 0x76 ACKs, chip ID = 0x61 */
    g_mock_i2c.probe_response[0x76] = 1;
    g_mock_i2c.registers[0xD0] = 0x61;
}

#endif /* UNIT_TEST */

/* --------------------------------------------------------------------------
 * HAL Implementation (Stub)
 * -------------------------------------------------------------------------- */

static struct {
    bool initialized;
    hal_i2c_config_t config;
} g_i2c_ctx;

hal_i2c_status_t HAL_I2C_Init(const hal_i2c_config_t *config)
{
    if (config == NULL) {
        return HAL_I2C_ERR_INVALID;
    }
    
    g_i2c_ctx.config = *config;
    g_i2c_ctx.initialized = true;
    
#ifdef UNIT_TEST
    /* Preserve test-configured mock state; tests reset via mock_i2c_reset() */
    if (!g_mock_i2c.initialized) {
        mock_i2c_reset();
    }
#endif
    
    return HAL_I2C_OK;
}

hal_i2c_status_t HAL_I2C_Probe(uint8_t addr)
{
    if (!g_i2c_ctx.initialized) {
        return HAL_I2C_ERR_INIT;
    }
    
#ifdef UNIT_TEST
    if (g_mock_i2c.force_error) {
        g_mock_i2c.force_error = false;
        return g_mock_i2c.next_error;
    }
    return g_mock_i2c.probe_response[addr & 0x7F] ? HAL_I2C_OK : HAL_I2C_ERR_NACK;
#else
    /* Real implementation would do a zero-byte write to check ACK */
    (void)addr;
    return HAL_I2C_OK;
#endif
}

hal_i2c_status_t HAL_I2C_Write(uint8_t addr, const uint8_t *data, size_t len)
{
    if (!g_i2c_ctx.initialized) {
        return HAL_I2C_ERR_INIT;
    }
    if (data == NULL || len == 0) {
        return HAL_I2C_ERR_INVALID;
    }
    
#ifdef UNIT_TEST
    if (g_mock_i2c.force_error) {
        g_mock_i2c.force_error = false;
        return g_mock_i2c.next_error;
    }
    /* Store written data in mock registers */
    if (len >= 2) {
        g_mock_i2c.registers[data[0]] = data[1];
    }
#endif
    
    (void)addr;
    return HAL_I2C_OK;
}

hal_i2c_status_t HAL_I2C_Read(uint8_t addr, uint8_t *data, size_t len)
{
    if (!g_i2c_ctx.initialized) {
        return HAL_I2C_ERR_INIT;
    }
    if (data == NULL || len == 0) {
        return HAL_I2C_ERR_INVALID;
    }
    
#ifdef UNIT_TEST
    if (g_mock_i2c.force_error) {
        g_mock_i2c.force_error = false;
        return g_mock_i2c.next_error;
    }
    /* Return mock data */
    memset(data, 0, len);
#endif
    
    (void)addr;
    return HAL_I2C_OK;
}

hal_i2c_status_t HAL_I2C_WriteRead(uint8_t addr,
                                    const uint8_t *tx_data, size_t tx_len,
                                    uint8_t *rx_data, size_t rx_len)
{
    if (!g_i2c_ctx.initialized) {
        return HAL_I2C_ERR_INIT;
    }
    if (tx_data == NULL || rx_data == NULL) {
        return HAL_I2C_ERR_INVALID;
    }
    
#ifdef UNIT_TEST
    if (g_mock_i2c.force_error) {
        g_mock_i2c.force_error = false;
        return g_mock_i2c.next_error;
    }
    /* Return data from mock register starting at tx_data[0] */
    uint8_t reg = tx_data[0];
    for (size_t i = 0; i < rx_len; i++) {
        rx_data[i] = g_mock_i2c.registers[(reg + i) & 0xFF];
    }
#endif
    
    (void)addr;
    (void)tx_len;
    return HAL_I2C_OK;
}

void HAL_I2C_DeInit(void)
{
    g_i2c_ctx.initialized = false;
    /* Mock state persists across DeInit; tests reset via mock_i2c_reset() */
}
