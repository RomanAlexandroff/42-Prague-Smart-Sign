
#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    display.powerOff();
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("Going to sleep for %u seconds.\n", time_in_millis / 1000);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_deep_sleep_start();
}

void  IRAM_ATTR ft_delay(unsigned int time_in_millis)
{
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
}
 
