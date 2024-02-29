
#include "42-Prague-Smart-Sign.h"

void  ft_power_down_recovery(void)
{
    esp_reset_reason_t        reset_reason;
    esp_sleep_wakeup_cause_t  wakeup_reason;

    reset_reason = esp_reset_reason();
    switch (reset_reason)
    {
        case ESP_RST_BROWNOUT:
            ft_display_bitmap_with_refresh(empty_battery_img);
            ft_battery_state(MIN_STATE_ADDR);
            display.powerOff();
            esp_sleep_enable_timer_wakeup(DEAD_BATTERY_SLEEP);
            esp_deep_sleep_start();
            break;
        case ESP_RST_POWERON:
            ft_display_bitmap_with_refresh(boot_up_screen_img);
            ft_battery_state(MAX_STATE_ADDR);
            g_cycle_counter = 0;
            DEBUG_PRINTF("\nReset reason: Power-on or Brown-out reset\n", "");
            DEBUG_PRINTF("Power-down Recovery was performed.\n", "");
            ft_delay(3000);
            ft_clear_display(true);
            break;
        case ESP_RST_SW:
            DEBUG_PRINTF("\nReset reason: Software reset\n", "");
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\nReset reason: Panic/exception reset\n", "");
            break;
    }
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            DEBUG_PRINTF("\nWakeup caused by a Button\n", "");
            break;
        case ESP_SLEEP_WAKEUP_ULP:
            DEBUG_PRINTF("\nWakeup caused by ULP program\n", "");
            break;
    }
}
 
