
#include "42-Prague-Smart-Sign.h"

void  setup(void)
{
    long          cycle_length;
    volatile long run_time;
    unsigned int  time_of_sleep;

    cycle_length = 60000;
    run_time = 0;
    time_of_sleep = 0;
    #ifdef DEBUG
        Serial.begin(115200);
        DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
    #endif
    ft_eeprom_init();
    ft_battery_init();
    ft_display_init();
    ft_power_down_recovery();
    ft_battery_check();
    ft_run_slideshow(&cycle_length);
    run_time = millis();
    time_of_sleep = cycle_length - run_time;
    if (time_of_sleep < 10)
        time_of_sleep = 10;
    DEBUG_PRINTF("The device will sleep for %lu milliseconds\n", time_of_sleep);
    ft_go_to_sleep(time_of_sleep);
}

void  loop(void) { /* NOTHING IS HERE */ }
 
