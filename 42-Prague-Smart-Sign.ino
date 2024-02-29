
#include "42-Prague-Smart-Sign.h"

void  setup(void)
{
    #ifdef DEBUG
        Serial.begin(115200);
        ft_delay(50);
        DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
    #endif
    ft_eeprom_init();
    ft_battery_init();
    ft_display_init();
    ft_power_down_recovery();
    ft_battery_check();
    ft_buttons_init();
    ft_wifi_connect();
    ft_get_time();
    
    
    ft_go_to_sleep(time_of_sleep);
}

void  loop(void) { /* NOTHING IS HERE */ }
 
