/**
 * @file sensor.c
 * @brief Sensor Acquisition Module Implementation
 * @requirement HVACOPT-01 Sensor Acquisition
 * @jira HVAC-23
 */

#include "sensor.h"
#include "hal_i2c.h"
#include <string.h>

/* --------------------------------------------------------------------------
 * Private State (No dynamic allocation - CON-1)
 * -------------------------------------------------------------------------- */

static struct {
    sensor_config_t  config;
    sensor_reading_t last_valid;
    sensor_reading_t current;
    bool             initialized;
    bool             faulted;
    uint32_t         last_poll_ms;
} g_sensor_ctx;

/* --------------------------------------------------------------------------
 * BME680 Register Definitions
 * -------------------------------------------------------------------------- */

#define BME680_REG_CHIP_ID      0xD0
#define BME680_CHIP_ID_VALUE    0x61
#define BME680_REG_CTRL_MEAS    0x74
#define BME680_REG_CTRL_HUM     0x72
#define BME680_REG_CTRL_GAS1    0x71
#define BME680_REG_STATUS       0x1D
#define BME680_REG_TEMP_MSB     0x22
#define BME680_REG_HUM_MSB      0x25
#define BME680_REG_GAS_R_MSB    0x2A

/* --------------------------------------------------------------------------
 * Private Helper Functions
 * -------------------------------------------------------------------------- */

static uint32_t get_tick_ms(void)
{
    /* TODO: Replace with actual tick source (HAL_GetTick or similar) */
#ifdef UNIT_TEST
    extern uint32_t mock_tick_ms;
    return mock_tick_ms;
#else
    static uint32_t tick = 0;
    return tick++;  /* Placeholder - replace with HAL timer */
#endif
}

static sensor_status_t read_chip_id(uint8_t *chip_id)
{
    uint8_t reg = BME680_REG_CHIP_ID;
    hal_i2c_status_t status = HAL_I2C_WriteRead(
        g_sensor_ctx.config.i2c_addr,
        &reg, 1,
        chip_id, 1
    );
    
    return (status == HAL_I2C_OK) ? SENSOR_OK : SENSOR_ERR_I2C;
}

static sensor_status_t configure_sensor(void)
{
    /* Configure humidity oversampling */
    uint8_t ctrl_hum[] = { BME680_REG_CTRL_HUM, 0x01 };  /* 1x oversampling */
    if (HAL_I2C_Write(g_sensor_ctx.config.i2c_addr, ctrl_hum, 2) != HAL_I2C_OK) {
        return SENSOR_ERR_I2C;
    }
    
    /* Configure temperature & pressure oversampling, forced mode */
    uint8_t ctrl_meas[] = { BME680_REG_CTRL_MEAS, 0x25 };  /* 1x temp, 1x press, forced */
    if (HAL_I2C_Write(g_sensor_ctx.config.i2c_addr, ctrl_meas, 2) != HAL_I2C_OK) {
        return SENSOR_ERR_I2C;
    }
    
    /* Configure gas sensor */
    uint8_t ctrl_gas[] = { BME680_REG_CTRL_GAS1, 0x10 };  /* Enable gas measurement */
    if (HAL_I2C_Write(g_sensor_ctx.config.i2c_addr, ctrl_gas, 2) != HAL_I2C_OK) {
        return SENSOR_ERR_I2C;
    }
    
    return SENSOR_OK;
}

/* --------------------------------------------------------------------------
 * Public API Implementation
 * -------------------------------------------------------------------------- */

sensor_status_t Sensor_Init(const sensor_config_t *config)
{
    /* Clear context */
    memset(&g_sensor_ctx, 0, sizeof(g_sensor_ctx));
    
    /* Apply configuration */
    if (config != NULL) {
        g_sensor_ctx.config = *config;
    } else {
        g_sensor_ctx.config.i2c_addr = SENSOR_BME680_ADDR;
        g_sensor_ctx.config.poll_interval_ms = SENSOR_POLL_INTERVAL_MS;
    }
    
    /* Initialize I²C HAL */
    hal_i2c_config_t i2c_cfg = {
        .clock_speed_hz = 100000,  /* 100 kHz standard mode */
        .port = 0
    };
    
    if (HAL_I2C_Init(&i2c_cfg) != HAL_I2C_OK) {
        return SENSOR_ERR_INIT;
    }
    
    /* AC-01.1: Verify device ACKs at expected address */
    if (HAL_I2C_Probe(g_sensor_ctx.config.i2c_addr) != HAL_I2C_OK) {
        return SENSOR_ERR_INIT;
    }
    
    /* Verify chip ID */
    uint8_t chip_id = 0;
    if (read_chip_id(&chip_id) != SENSOR_OK || chip_id != BME680_CHIP_ID_VALUE) {
        return SENSOR_ERR_INIT;
    }
    
    /* Configure sensor for measurements */
    if (configure_sensor() != SENSOR_OK) {
        return SENSOR_ERR_INIT;
    }
    
    g_sensor_ctx.initialized = true;
    g_sensor_ctx.faulted = false;
    g_sensor_ctx.last_poll_ms = get_tick_ms();
    
    return SENSOR_OK;
}

sensor_status_t Sensor_Read(sensor_reading_t *reading)
{
    if (reading == NULL) {
        return SENSOR_ERR_INVALID_ARG;
    }
    
    if (!g_sensor_ctx.initialized) {
        return SENSOR_ERR_INIT;
    }
    
    /* Read raw sensor data */
    uint8_t reg = BME680_REG_TEMP_MSB;
    uint8_t raw_data[8];
    
    hal_i2c_status_t status = HAL_I2C_WriteRead(
        g_sensor_ctx.config.i2c_addr,
        &reg, 1,
        raw_data, sizeof(raw_data)
    );
    
    /* AC-01.3: On I²C failure, set SENSOR_FAULT flag */
    if (status != HAL_I2C_OK) {
        g_sensor_ctx.faulted = true;
        return SENSOR_FAULT;
    }
    
    /* AC-01.2: Parse temperature, humidity, CO₂, VOC from raw data */
    /* Temperature: 20-bit ADC value, convert to °C × 100 */
    int32_t temp_adc = ((int32_t)raw_data[0] << 12) | 
                       ((int32_t)raw_data[1] << 4) | 
                       ((int32_t)raw_data[2] >> 4);
    reading->temperature_c_x100 = (int16_t)((temp_adc * 100) / 16384 - 4000);
    
    /* Humidity: 16-bit ADC value, convert to % × 10 */
    uint16_t hum_adc = ((uint16_t)raw_data[3] << 8) | raw_data[4];
    reading->humidity_pct_x10 = (uint16_t)((hum_adc * 1000) / 65536);
    
    /* Gas resistance to CO₂-equivalent (simplified estimation) */
    uint16_t gas_adc = ((uint16_t)raw_data[5] << 2) | (raw_data[6] >> 6);
    reading->co2_ppm = 400 + (4096 - gas_adc) / 4;  /* Simplified mapping */
    
    /* VOC index (0-500 scale) */
    reading->voc_index = (500 * (4096 - gas_adc)) / 4096;
    
    reading->timestamp_ms = get_tick_ms();
    reading->valid = true;
    
    /* Update last valid and current readings */
    g_sensor_ctx.current = *reading;
    g_sensor_ctx.last_valid = *reading;
    
    return SENSOR_OK;
}

sensor_status_t Sensor_GetLastValid(sensor_reading_t *reading)
{
    if (reading == NULL) {
        return SENSOR_ERR_INVALID_ARG;
    }
    
    if (!g_sensor_ctx.last_valid.valid) {
        return SENSOR_FAULT;
    }
    
    /* AC-01.3: Return last-known-good values */
    *reading = g_sensor_ctx.last_valid;
    return SENSOR_OK;
}

bool Sensor_IsFaulted(void)
{
    return g_sensor_ctx.faulted;
}

sensor_status_t Sensor_ClearFault(void)
{
    g_sensor_ctx.faulted = false;
    return SENSOR_OK;
}

sensor_status_t Sensor_Poll(void)
{
    if (!g_sensor_ctx.initialized) {
        return SENSOR_ERR_INIT;
    }
    
    /* AC-01.4: Polling at 1 Hz ± 10 ms jitter */
    uint32_t now = get_tick_ms();
    uint32_t elapsed = now - g_sensor_ctx.last_poll_ms;
    
    if (elapsed < (g_sensor_ctx.config.poll_interval_ms - SENSOR_POLL_JITTER_MS)) {
        return SENSOR_OK;  /* Not time to poll yet */
    }
    
    g_sensor_ctx.last_poll_ms = now;
    
    /* Trigger forced measurement and read */
    sensor_reading_t reading;
    return Sensor_Read(&reading);
}

void Sensor_DeInit(void)
{
    HAL_I2C_DeInit();
    memset(&g_sensor_ctx, 0, sizeof(g_sensor_ctx));
}
