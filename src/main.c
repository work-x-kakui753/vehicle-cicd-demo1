#include <stdio.h>

#include "vehicle_control.h"

int main(void)
{
    TemperatureStatus temp_status = check_temperature_status(85);
    HeadlampState lamp_state = decide_headlamp_state(120, 1);
    CanValidation can_status = validate_can_frame(0x123U, 8);

    printf("Vehicle CI/CD demo scenario\n");
    printf("[DEMO] Temperature 85C => %s\n", temperature_status_to_string(temp_status));
    printf("[DEMO] Ambient 120 lux / ignition ON => headlamp %s\n",
        headlamp_state_to_string(lamp_state));
    printf("[DEMO] CAN ID 0x123 / DLC 8 => %s\n", can_validation_to_string(can_status));

    return 0;
}
