#ifndef VEHICLE_CONTROL_H
#define VEHICLE_CONTROL_H

typedef enum TemperatureStatusTag {
    TEMP_STATUS_NORMAL = 0,
    TEMP_STATUS_WARNING = 1,
    TEMP_STATUS_CRITICAL = 2,
    TEMP_STATUS_SENSOR_ERROR = 3
} TemperatureStatus;

typedef enum HeadlampStateTag {
    HEADLAMP_OFF = 0,
    HEADLAMP_ON = 1
} HeadlampState;

typedef enum CanValidationTag {
    CAN_FRAME_INVALID = 0,
    CAN_FRAME_VALID = 1
} CanValidation;

TemperatureStatus check_temperature_status(int temperature_c);
HeadlampState decide_headlamp_state(int ambient_lux, int ignition_on);
CanValidation validate_can_frame(unsigned int can_id, int dlc);

const char* temperature_status_to_string(TemperatureStatus status);
const char* headlamp_state_to_string(HeadlampState state);
const char* can_validation_to_string(CanValidation state);

#endif
