
#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  isr_warning(void)
{
    DEBUG_PRINTF("  ---- Warning Button was pressed\n", "");
    rtc_g.warning_active = !rtc_g.warning_active;
}

void IRAM_ATTR  isr_diagnostics(void)
{
    DEBUG_PRINTF("  ---- Diagnostics Button was pressed\n", "");
}

void IRAM_ATTR  isr_ota(void)
{
    DEBUG_PRINTF("  ---- OTA Button was pressed\n", "");
    rtc_g.ota_active = !rtc_g.ota_active;
    if (rtc_g.ota_active)
        ft_ota_init();
}

void IRAM_ATTR  isr_reboot(void)
{
    DEBUG_PRINTF("  ---- Reboot Button was pressed\n", "");
    ft_go_to_sleep(100);
}

void  ft_buttons_init(void)
{
    esp_err_t   touchpad_status;
    touch_pad_t touch_pin;

    touchAttachInterrupt(WARNING_BUTTON, isr_warning, BUTTON_THRESHOLD);
    touchAttachInterrupt(DIAGNOSTICS_BUTTON, isr_diagnostics, BUTTON_THRESHOLD);
    touchAttachInterrupt(OTA_BUTTON, isr_ota, BUTTON_THRESHOLD);
    touchAttachInterrupt(REBOOT_BUTTON, isr_reboot, BUTTON_THRESHOLD);
    touchSleepWakeUpEnable(WARNING_BUTTON, BUTTON_THRESHOLD);
    touchSleepWakeUpEnable(DIAGNOSTICS_BUTTON, BUTTON_THRESHOLD);
    touchSleepWakeUpEnable(OTA_BUTTON, BUTTON_THRESHOLD);
    touchpad_status = esp_sleep_enable_touchpad_wakeup();
    switch (touchpad_status)
    {
        case ESP_OK:
            DEBUG_PRINTF("\nAll Buttons were enabled to be a wake up source\n", "");
            break;
        case ESP_ERR_NOT_SUPPORTED:
            DEBUG_PRINTF("\nFailed to set Buttons as a wake up source\n", "");
            break;
        case ESP_ERR_INVALID_STATE:
            DEBUG_PRINTF("\nError setting Buttons as a wake up source! ", "");
            DEBUG_PRINTF("Wake up sources trigger a conflict\n", "");
            break;
    }
    touch_pin = esp_sleep_get_touchpad_wakeup_status();
    if(touch_pin < TOUCH_PAD_MAX)
        DEBUG_PRINTF("Touch detected on GPIO %d\n", touch_pin); 
    else
        DEBUG_PRINTF("Wakeup not by touchpad\n\n", "");
}
 
