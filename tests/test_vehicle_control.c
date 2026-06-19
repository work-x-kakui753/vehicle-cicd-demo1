#include <stdio.h>
#include <stdlib.h>

#include "../src/vehicle_control.h"

static int g_failures = 0;

#define ASSERT_EQ_INT(test_name, expected, actual)                                             \
    do {                                                                                        \
        int expected_value__ = (expected);                                                      \
        int actual_value__ = (actual);                                                          \
        if (expected_value__ != actual_value__) {                                               \
            printf("[TEST] %s ... FAIL (expected=%d actual=%d)\n",                             \
                (test_name), expected_value__, actual_value__);                                 \
            g_failures++;                                                                       \
        } else {                                                                                \
            printf("[TEST] %s ... PASS\n", (test_name));                                        \
        }                                                                                       \
    } while (0)

static void test_temperature_cases(void)
{
    ASSERT_EQ_INT("Temperature normal case", TEMP_STATUS_NORMAL,
        check_temperature_status(25));
    ASSERT_EQ_INT("Temperature warning case", TEMP_STATUS_WARNING,
        check_temperature_status(85));
    ASSERT_EQ_INT("Temperature critical case", TEMP_STATUS_CRITICAL,
        check_temperature_status(105));
    ASSERT_EQ_INT("Temperature sensor error low case", TEMP_STATUS_SENSOR_ERROR,
        check_temperature_status(-41));
    ASSERT_EQ_INT("Temperature sensor error high case", TEMP_STATUS_SENSOR_ERROR,
        check_temperature_status(151));
}

static void test_headlamp_cases(void)
{
    ASSERT_EQ_INT("Headlamp ON case", HEADLAMP_ON, decide_headlamp_state(100, 1));
    ASSERT_EQ_INT("Headlamp OFF bright case", HEADLAMP_OFF, decide_headlamp_state(800, 1));
    ASSERT_EQ_INT("Headlamp OFF ignition case", HEADLAMP_OFF, decide_headlamp_state(100, 0));
}

static void test_can_cases(void)
{
    ASSERT_EQ_INT("CAN valid frame case", CAN_FRAME_VALID, validate_can_frame(0x123U, 8));
    ASSERT_EQ_INT("CAN valid alternate frame case", CAN_FRAME_VALID,
        validate_can_frame(0x456U, 4));
    ASSERT_EQ_INT("CAN invalid DLC case", CAN_FRAME_INVALID, validate_can_frame(0x123U, 9));
    ASSERT_EQ_INT("CAN invalid ID case", CAN_FRAME_INVALID, validate_can_frame(0x777U, 8));
}

int main(void)
{
    printf("[INFO] Vehicle control unit test start\n");

    test_temperature_cases();
    test_headlamp_cases();
    test_can_cases();

    if (g_failures > 0) {
        printf("[INFO] Test result: FAIL (%d failures)\n", g_failures);
        return 1;
    }

    printf("[INFO] Test result: PASS\n");
    return 0;
}
