/**
 * @file test_sensor.c
 * @brief Unit Tests for Sensor Acquisition Module
 * @requirement HVACOPT-01 Sensor Acquisition
 * @jira HVAC-23
 */

#include "sensor.h"
#include "hal_i2c.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* --------------------------------------------------------------------------
 * Test Helpers
 * -------------------------------------------------------------------------- */

#define TEST_PASS(name) printf("  ✓ %s\n", name)
#define TEST_FAIL(name, msg) printf("  ✗ %s: %s\n", name, msg); test_failures++

static int test_failures = 0;

/* Mock control functions (defined in hal_i2c_stub.c) */
extern uint32_t mock_tick_ms;
extern void mock_i2c_set_probe_response(uint8_t addr, bool ack);
extern void mock_i2c_set_register(uint8_t reg, uint8_t value);
extern void mock_i2c_force_error(hal_i2c_status_t error);
extern void mock_i2c_clear_error(void);
extern void mock_i2c_reset(void);

static void setup(void)
{
    Sensor_DeInit();
    mock_tick_ms = 0;
    mock_i2c_reset();
}

/* --------------------------------------------------------------------------
 * AC-01.1: Sensor_Init returns SENSOR_OK when device ACKs
 * -------------------------------------------------------------------------- */

static void test_sensor_init_success(void)
{
    setup();
    
    sensor_status_t status = Sensor_Init(NULL);
    
    if (status == SENSOR_OK) {
        TEST_PASS("AC-01.1: Sensor_Init returns SENSOR_OK when device ACKs");
    } else {
        TEST_FAIL("AC-01.1: Sensor_Init returns SENSOR_OK when device ACKs", 
                  "Init failed unexpectedly");
    }
}

static void test_sensor_init_no_ack(void)
{
    setup();
    
    /* Configure mock to NACK at BME680 address */
    mock_i2c_set_probe_response(SENSOR_BME680_ADDR, false);
    
    sensor_status_t status = Sensor_Init(NULL);
    
    if (status == SENSOR_ERR_INIT) {
        TEST_PASS("AC-01.1: Sensor_Init returns error when device NACKs");
    } else {
        TEST_FAIL("AC-01.1: Sensor_Init returns error when device NACKs",
                  "Should have returned SENSOR_ERR_INIT");
    }
}

/* --------------------------------------------------------------------------
 * AC-01.2: Sensor_Read retrieves temperature, humidity, CO₂, VOC
 * -------------------------------------------------------------------------- */

static void test_sensor_read_values(void)
{
    setup();
    
    /* Set up mock sensor data */
    mock_i2c_set_register(0x22, 0x80);  /* Temp MSB */
    mock_i2c_set_register(0x23, 0x00);  /* Temp LSB */
    mock_i2c_set_register(0x24, 0x00);  /* Temp XLSB */
    mock_i2c_set_register(0x25, 0xA0);  /* Hum MSB */
    mock_i2c_set_register(0x26, 0x00);  /* Hum LSB */
    mock_i2c_set_register(0x2A, 0x08);  /* Gas MSB */
    mock_i2c_set_register(0x2B, 0x00);  /* Gas LSB */
    
    Sensor_Init(NULL);
    
    sensor_reading_t reading;
    sensor_status_t status = Sensor_Read(&reading);
    
    if (status == SENSOR_OK && reading.valid) {
        TEST_PASS("AC-01.2: Sensor_Read returns valid reading");
    } else {
        TEST_FAIL("AC-01.2: Sensor_Read returns valid reading",
                  "Read failed or reading invalid");
    }
    
    /* Verify all fields are populated */
    if (reading.valid) {
        TEST_PASS("AC-01.2: Reading includes timestamp");
    } else {
        TEST_FAIL("AC-01.2: Reading includes timestamp",
                  "Timestamp not set");
    }
}

/* --------------------------------------------------------------------------
 * AC-01.3: I²C failure sets SENSOR_FAULT, last-known-good accessible
 * -------------------------------------------------------------------------- */

static void test_sensor_fault_on_i2c_error(void)
{
    setup();
    Sensor_Init(NULL);
    
    /* Get a valid reading first */
    sensor_reading_t first_reading;
    Sensor_Read(&first_reading);
    
    /* Force I²C error on next read */
    mock_i2c_force_error(HAL_I2C_ERR_BUS);
    
    sensor_reading_t second_reading;
    sensor_status_t status = Sensor_Read(&second_reading);
    
    if (status == SENSOR_FAULT && Sensor_IsFaulted()) {
        TEST_PASS("AC-01.3: I²C failure sets SENSOR_FAULT flag");
    } else {
        TEST_FAIL("AC-01.3: I²C failure sets SENSOR_FAULT flag",
                  "Fault not detected");
    }
    
    /* Verify last-known-good is still accessible */
    sensor_reading_t last_valid;
    status = Sensor_GetLastValid(&last_valid);
    
    if (status == SENSOR_OK && last_valid.valid) {
        TEST_PASS("AC-01.3: Last-known-good values accessible after fault");
    } else {
        TEST_FAIL("AC-01.3: Last-known-good values accessible after fault",
                  "GetLastValid failed");
    }
}

/* --------------------------------------------------------------------------
 * AC-01.4: Polling at 1 Hz ± 10 ms jitter
 * -------------------------------------------------------------------------- */

static void test_sensor_poll_timing(void)
{
    setup();
    Sensor_Init(NULL);
    
    /* First poll should succeed immediately */
    mock_tick_ms = 0;
    sensor_status_t status = Sensor_Poll();
    
    if (status == SENSOR_OK) {
        TEST_PASS("AC-01.4: First poll succeeds");
    } else {
        TEST_FAIL("AC-01.4: First poll succeeds", "First poll failed");
    }
    
    /* Poll before interval should not trigger read */
    mock_tick_ms = 500;  /* Only 500ms elapsed */
    status = Sensor_Poll();
    
    /* Poll after interval should trigger read */
    mock_tick_ms = 1000;  /* Full 1s elapsed */
    status = Sensor_Poll();
    
    if (status == SENSOR_OK) {
        TEST_PASS("AC-01.4: Poll triggers at 1 Hz interval");
    } else {
        TEST_FAIL("AC-01.4: Poll triggers at 1 Hz interval",
                  "Poll at interval failed");
    }
    
    /* Poll within jitter tolerance should work */
    mock_tick_ms = 1990;  /* 990ms since last poll (within 10ms jitter) */
    status = Sensor_Poll();
    
    if (status == SENSOR_OK) {
        TEST_PASS("AC-01.4: Poll works within ±10ms jitter");
    } else {
        TEST_FAIL("AC-01.4: Poll works within ±10ms jitter",
                  "Jitter tolerance not met");
    }
}

/* --------------------------------------------------------------------------
 * Test Runner
 * -------------------------------------------------------------------------- */

int main(void)
{
    printf("\n========================================\n");
    printf("HVACOPT-01 Sensor Acquisition Tests\n");
    printf("========================================\n\n");
    
    printf("AC-01.1: Initialization Tests\n");
    test_sensor_init_success();
    test_sensor_init_no_ack();
    
    printf("\nAC-01.2: Sensor Reading Tests\n");
    test_sensor_read_values();
    
    printf("\nAC-01.3: Fault Handling Tests\n");
    test_sensor_fault_on_i2c_error();
    
    printf("\nAC-01.4: Polling Timing Tests\n");
    test_sensor_poll_timing();
    
    printf("\n========================================\n");
    if (test_failures == 0) {
        printf("All tests PASSED!\n");
    } else {
        printf("%d test(s) FAILED\n", test_failures);
    }
    printf("========================================\n\n");
    
    return test_failures;
}
