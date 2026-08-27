# Specification: HVACOPT-01 Sensor Acquisition Module

| Field | Value |
|-------|-------|
| **Requirement ID** | HVACOPT-01 |
| **Jira Issue** | [HVAC-23](https://labskraft-team1.atlassian.net/browse/HVAC-23) |
| **Epic** | [HVAC-20](https://labskraft-team1.atlassian.net/browse/HVAC-20) |
| **Branch** | `feature/HVAC-23-sensor-acquisition-module` |
| **Status** | Draft |

---

## 1. Overview

The Sensor Acquisition Module reads environmental data from the BME680 sensor via I²C and exposes the latest readings through a thread-safe API. This module forms the foundation of the HVAC Edge Optimizer's environmental monitoring capability.

---

## 2. Functional Requirements

| ID | Requirement | Acceptance Criteria |
|----|-------------|---------------------|
| **REQ-1** | Initialize BME680 sensor over I²C | AC-01.1: `Sensor_Init()` returns `SENSOR_OK` when the device ACKs at the expected address (0x76) |
| **REQ-2** | Read environmental sensor data | AC-01.2: `Sensor_Read()` retrieves temperature (°C), humidity (%), CO₂-equivalent (ppm), and VOC index, stored in a timestamped struct |
| **REQ-3** | Handle I²C communication failures gracefully | AC-01.3: On I²C failure, module sets `SENSOR_FAULT` flag; last-known-good values remain accessible via `Sensor_GetLastValid()` |
| **REQ-4** | Support periodic polling from scheduler | AC-01.4: Sensor polling occurs at 1 Hz ± 10 ms jitter when invoked from the scheduler |

---

## 3. Non-Functional Requirements

| ID | Requirement | Metric |
|----|-------------|--------|
| **NFR-1** | No dynamic memory allocation | Zero calls to `malloc`/`free` |
| **NFR-2** | Deterministic execution | Fixed-time budget per call |
| **NFR-3** | Host-buildable | Compiles with gcc/make on Linux/macOS |
| **NFR-4** | Test coverage | ≥90% unit-test branch coverage |

---

## 4. Constraints

| ID | Constraint |
|----|------------|
| **CON-1** | No dynamic memory allocation (`malloc`/`free`) - all buffers must be caller-owned or static |
| **CON-2** | Module must not call HAL functions for GPIO, ADC (only I²C HAL) |
| **CON-3** | Polling interval must be configurable at initialization |
| **CON-4** | Sensor data struct must be timestamped with monotonic tick |
| **CON-5** | Thread-safe access to last valid reading |

---

## 5. Interfaces & Contracts

### 5.1 Public API

```c
/* Initialization */
sensor_status_t Sensor_Init(const sensor_config_t *config);

/* Data access */
sensor_status_t Sensor_Read(sensor_reading_t *reading);
sensor_status_t Sensor_GetLastValid(sensor_reading_t *reading);

/* Fault management */
bool Sensor_IsFaulted(void);
sensor_status_t Sensor_ClearFault(void);

/* Scheduler hook */
sensor_status_t Sensor_Poll(void);

/* Cleanup */
void Sensor_DeInit(void);
```

### 5.2 Data Structures

```c
typedef struct {
    int16_t  temperature_c_x100;  /* Temperature in °C × 100 */
    uint16_t humidity_pct_x10;    /* Humidity in % × 10 */
    uint16_t co2_ppm;             /* CO₂-equivalent in ppm */
    uint16_t voc_index;           /* VOC index (0-500) */
    uint32_t timestamp_ms;        /* Monotonic timestamp */
    bool     valid;               /* Reading validity flag */
} sensor_reading_t;
```

### 5.3 Dependencies

| Layer | Module | Usage |
|-------|--------|-------|
| HAL | `hal_i2c.h` | I²C communication primitives |

### 5.4 Must NOT Use

- `hal_gpio.h` - GPIO functions
- `hal_adc.h` - ADC functions
- `<stdlib.h>` - Dynamic allocation functions

---

## 6. Acceptance Criteria

### AC-01.1: Initialization Success
**Given** the BME680 sensor is connected and powered  
**When** `Sensor_Init()` is called with valid configuration  
**Then** the function returns `SENSOR_OK` when the device ACKs at address 0x76

### AC-01.2: Sensor Reading
**Given** the sensor module is initialized  
**When** `Sensor_Read()` is called  
**Then** it retrieves and stores:
- Temperature (°C × 100 for fixed-point)
- Humidity (% × 10 for fixed-point)
- CO₂-equivalent (ppm)
- VOC index (0-500 scale)
- Monotonic timestamp (ms)

### AC-01.3: Fault Handling
**Given** the sensor module is initialized  
**When** an I²C transaction fails  
**Then**:
- The module sets internal `SENSOR_FAULT` flag
- `Sensor_IsFaulted()` returns `true`
- `Sensor_GetLastValid()` returns the last successfully read values

### AC-01.4: Polling Timing
**Given** the sensor module is initialized with 1Hz polling  
**When** `Sensor_Poll()` is invoked from the scheduler  
**Then** sensor reading occurs at 1 Hz ± 10 ms jitter tolerance

---

## 7. Test Plan

| Test ID | Acceptance Criteria | Test Description |
|---------|---------------------|------------------|
| T-01.1a | AC-01.1 | Verify `Sensor_Init()` returns `SENSOR_OK` with valid device |
| T-01.1b | AC-01.1 | Verify `Sensor_Init()` returns `SENSOR_ERR_INIT` when device NACKs |
| T-01.2a | AC-01.2 | Verify `Sensor_Read()` returns all fields populated |
| T-01.2b | AC-01.2 | Verify timestamp is monotonically increasing |
| T-01.3a | AC-01.3 | Verify fault flag set on I²C error |
| T-01.3b | AC-01.3 | Verify last-known-good accessible after fault |
| T-01.4a | AC-01.4 | Verify polling respects interval timing |
| T-01.4b | AC-01.4 | Verify jitter tolerance of ±10ms |

---

## 8. Traceability Matrix

| Requirement | Acceptance Criteria | Test Cases | Source File | Line |
|-------------|---------------------|------------|-------------|------|
| REQ-1 | AC-01.1 | T-01.1a, T-01.1b | `sensor.c` | `Sensor_Init()` |
| REQ-2 | AC-01.2 | T-01.2a, T-01.2b | `sensor.c` | `Sensor_Read()` |
| REQ-3 | AC-01.3 | T-01.3a, T-01.3b | `sensor.c` | `Sensor_Read()`, `Sensor_GetLastValid()` |
| REQ-4 | AC-01.4 | T-01.4a, T-01.4b | `sensor.c` | `Sensor_Poll()` |

---

## 9. Change Log

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-08-27 | AI Assistant | Initial specification |
