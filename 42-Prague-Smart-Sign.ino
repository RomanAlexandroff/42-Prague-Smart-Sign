
#include "42-Prague-Smart-Sign.h"

static void  setup(void)
{
    #ifdef DEBUG
        Serial.begin(115200);
        ft_delay(50);
        DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
    #endif
    ft_eeprom_init();           // всегда
    ft_battery_init();          // всегда
    ft_display_init();          // всегда
    ft_power_down_recovery();   // всегда
    ft_battery_check();         // всегда
    ft_buttons_init();          // всегда
}

static void  ft_pathfinder(void)
{
    unsigned int  sleep_length;

    if (rtc_g.exam_state)
        ft_exam_mode(&sleep_length);
    else
        ft_cluster_number_mode(&sleep_length);
    ft_go_to_sleep(sleep_length);
    DEBUG_PRINTF("This message will never be printed out", "");
}

void  loop(void) { /* NOTHING IS HERE */ }
 
