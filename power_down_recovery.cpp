
#include "42-Prague-Smart-Sign.h"

void  ft_power_down_recovery(void)
{
    globals.reason = esp_reset_reason();
    switch (globals.reason)
    {
        case ESP_RST_BROWNOUT:
            ft_display_bitmap_with_refresh(badge_bitmap_empty_battery);
            ft_battery_state(MIN_STATE_ADDR);
            display.powerOff();
            esp_sleep_enable_timer_wakeup(DEAD_BATTERY_SLEEP);
            esp_deep_sleep_start();
            break;
        case ESP_RST_POWERON:
            ft_display_bitmap_with_refresh(badge_bitmap_boot_up_screen);
            ft_battery_state(MAX_STATE_ADDR);
            g_cycle_counter = 0;
            DEBUG_PRINTF("\nReset reason: Power-on or Brown-out reset\n", "");
            DEBUG_PRINTF("Power-down Recovery was performed.\nThe cycle counter was set to %d\n", g_cycle_counter);
            ft_delay(3000);
            ft_clear_display(true);
            break;
        case ESP_RST_SW:
            g_cycle_counter++;
            if (g_cycle_counter >= 65004)
                g_cycle_counter = 0;
            DEBUG_PRINTF("\nReset reason: Software reset\n", "");
            DEBUG_PRINTF("cycle number %d\n\n", g_cycle_counter);
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\nReset reason: Panic/exception reset\n", "");
            break;
    }
}
 
