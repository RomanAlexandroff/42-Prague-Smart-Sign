/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power_down_recovery.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:26 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:27 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  ft_brownout_sequence(void)
{
    DEBUG_PRINTF("\nReset reason: Brown-out reset. Going into extensive sleep\n", "");
    ft_go_to_sleep(DEAD_BATTERY_SLEEP);
}

void  ft_poweron_sequence(void)
{
//    ft_display_bitmap_with_refresh(bootup_screen_img);
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
    rtc_g.ota_active = false;
    rtc_g.warning_active = false;
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
}
 
