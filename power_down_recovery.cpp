
#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  ft_brownout_sequence(void)
{
//    ft_display_bitmap_with_refresh(empty_battery_img);
    #if !defined(XIAO_ESP32C3)
        ft_battery_state(MIN_STATE);
    #endif
    DEBUG_PRINTF("\nReset reason: Brown-out reset. Going into extensive sleep\n", "");
    display.powerOff();
    esp_sleep_enable_timer_wakeup(DEAD_BATTERY_SLEEP * 1000);
    esp_deep_sleep_start();
}

void  ft_poweron_sequence(void)
{
//    ft_display_bitmap_with_refresh(bootup_screen_img);
    if (!LittleFS.exists("/battery_max.txt"))
        LittleFS.open("/battery_max.txt", "w");
    if (!LittleFS.exists("/battery_min.txt"))
        LittleFS.open("/battery_min.txt", "w");
    if (!LittleFS.exists("/secret.txt"))
    {
        DEBUG_PRINTF("\nThe secret.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/secret.txt", SECRET);
        DEBUG_PRINTF("secret.txt file created. It contains:\n%s\n", ft_read_spiffs_file("/secret.txt").c_str());
    }
    if (!ft_secret_verification(rtc_g.Secret))
    {
        DEBUG_PRINTF("\nThe Secret variable value is lost. Restoring...\n", "");
        rtc_g.Secret = ft_read_spiffs_file("/secret.txt");
        rtc_g.Secret.trim();
        DEBUG_PRINTF("The Secret variable value is now:\n%s\n", rtc_g.Secret.c_str());
    }
    #if !defined(XIAO_ESP32C3)
        ft_battery_state(MAX_STATE);
    #endif
    rtc_g.ota_active = false;
    DEBUG_PRINTF("\nReset reason: Power-on reset\n", "");
    DEBUG_PRINTF("Power-down Recovery was performed.\n\n", "");
//    ft_delay(3000);
}

void  ft_power_down_recovery(void)
{
    esp_reset_reason_t        reset_reason;
    esp_sleep_wakeup_cause_t  wakeup_reason;

    reset_reason = esp_reset_reason();
    switch (reset_reason)
    {
        case ESP_RST_BROWNOUT:
            ft_brownout_sequence();
            break;
        case ESP_RST_POWERON:
            ft_poweron_sequence();
            break;
        case ESP_RST_SW:
            rtc_g.ota_active = false;
            DEBUG_PRINTF("\nReset reason: Software reset\n", "");
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\nReset reason: Panic/exception reset\n", "");
            break;
    }
    #if defined(ESP32) || defined(ESP32_S2)
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
    #endif
}
 
