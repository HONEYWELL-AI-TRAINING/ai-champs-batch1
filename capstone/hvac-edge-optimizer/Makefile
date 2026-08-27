# HVAC Edge Optimizer - Capstone Project
# Build system for embedded C project (host-buildable with gcc/make)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -O2
CFLAGS += -I./include -I./src

# Source directories
SRC_DIR = src
HAL_DIR = $(SRC_DIR)/hal
DRIVER_DIR = $(SRC_DIR)/driver
CONTROL_DIR = $(SRC_DIR)/control
TELEMETRY_DIR = $(SRC_DIR)/telemetry
TEST_DIR = test

# Build output
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source files
HAL_SRC = $(wildcard $(HAL_DIR)/*.c)
DRIVER_SRC = $(wildcard $(DRIVER_DIR)/*.c)
CONTROL_SRC = $(wildcard $(CONTROL_DIR)/*.c)
TELEMETRY_SRC = $(wildcard $(TELEMETRY_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

ALL_SRC = $(HAL_SRC) $(DRIVER_SRC) $(CONTROL_SRC) $(TELEMETRY_SRC)
ALL_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(ALL_SRC))

# Main target
TARGET = $(BIN_DIR)/hvac_optimizer

# Test target
TEST_TARGET = $(BIN_DIR)/test_runner

.PHONY: all clean test coverage

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)/hal $(OBJ_DIR)/driver $(OBJ_DIR)/control $(OBJ_DIR)/telemetry
	@mkdir -p $(BIN_DIR)

$(TARGET): $(ALL_OBJ) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Test build
test: CFLAGS += -DUNIT_TEST -coverage
test: dirs $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(ALL_OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lgcov

# Coverage report
coverage: test
	gcov $(ALL_SRC)
	@echo "Coverage report generated"

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.gcov *.gcda *.gcno
