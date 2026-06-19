#include "vehicle_control.h"

TemperatureStatus check_temperature_status(int temperature_c)
{
    if (temperature_c < -40 || temperature_c > 150) {
        return TEMP_STATUS_SENSOR_ERROR;
    }

    if (temperature_c >= 100) {
        return TEMP_STATUS_CRITICAL;
    }

    if (temperature_c >= 80) {
        return TEMP_STATUS_WARNING;
    }

    return TEMP_STATUS_NORMAL;
}

HeadlampState decide_headlamp_state(int ambient_lux, int ignition_on)
{
    if (ignition_on == 0) {
        return HEADLAMP_OFF;
    }

    if (ambient_lux < 300) {
        return HEADLAMP_ON;
    }

    return HEADLAMP_OFF;
}

CanValidation validate_can_frame(unsigned int can_id, int dlc)
{
    if (dlc < 0 || dlc > 8) {
        return CAN_FRAME_INVALID;
    }

    if (can_id == 0x123U || can_id == 0x456U) {
        return CAN_FRAME_VALID;
    }

    return CAN_FRAME_INVALID;
}

const char* temperature_status_to_string(TemperatureStatus status)
{
    switch (status) {
    case TEMP_STATUS_NORMAL:
        return "NORMAL";
    case TEMP_STATUS_WARNING:
        return "WARNING";
    case TEMP_STATUS_CRITICAL:
        return "CRITICAL";
    case TEMP_STATUS_SENSOR_ERROR:
        return "SENSOR_ERROR";
    default:
        return "UNKNOWN";
    }
}

const char* headlamp_state_to_string(HeadlampState state)
{
    switch (state) {
    case HEADLAMP_OFF:
        return "OFF";
    case HEADLAMP_ON:
        return "ON";
    default:
        return "UNKNOWN";
    }
}

const char* can_validation_to_string(CanValidation state)
{
    switch (state) {
    case CAN_FRAME_INVALID:
        return "INVALID";
    case CAN_FRAME_VALID:
        return "VALID";
    default:
        return "UNKNOWN";
    }
}
