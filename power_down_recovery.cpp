/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power_down_recovery.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:26 by raleksan          #+#    #+#             */
/*   Updated: 2024/05/29 13:02:27 by raleksan         ###   ########.fr       */
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
    if (!LittleFS.exists("/secret.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The secret.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/secret.txt", SECRET);
        DEBUG_PRINTF("[FILE SYSTEM] secret.txt file created. It contains:\n%s\n", ft_read_spiffs_file("/secret.txt").c_str());
    }
    if (!ft_secret_verification(rtc_g.Secret))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The Secret variable value is lost. Restoring...\n", "");
        rtc_g.Secret = ft_read_spiffs_file("/secret.txt");
        rtc_g.Secret.trim();
        DEBUG_PRINTF("[FILE SYSTEM] The Secret variable value is now:\n%s\n", rtc_g.Secret.c_str());
    }
    if (!LittleFS.exists("/ota.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The ota.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/ota.txt", CLOSED);
        DEBUG_PRINTF("[FILE SYSTEM] ota.txt file created. The rtc_g.ota value is recorded as %d\n", ft_read_spiffs_file("/ota.txt").toInt() != 0);
    }
    rtc_g.ota = ft_read_spiffs_file("/ota.txt").toInt() != 0;
    DEBUG_PRINTF("[FILE SYSTEM] The rtc_g.ota variable has been set to %d\n", rtc_g.ota);
    if (!LittleFS.exists("/chat_id.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The chat_id.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/chat_id.txt", "00000000000000000");
        DEBUG_PRINTF("[FILE SYSTEM] chat_id.txt file created. The rtc_g.chat_id value is recorded as %d\n", ft_read_spiffs_file("/chat_id.txt"));
    }
    rtc_g.chat_id = ft_read_spiffs_file("/chat_id.txt");
    rtc_g.chat_id.trim();
    DEBUG_PRINTF("[FILE SYSTEM] The rtc_g.chat_id variable has been set to %d\n", rtc_g.chat_id);
    rtc_g.warning_active = false;
    DEBUG_PRINTF("Power-down Recovery was performed.\n\n", "");
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
            DEBUG_PRINTF("\nReset reason: Power-on reset\n", "");
            ft_poweron_sequence();
            break;
        case ESP_RST_SW:
            DEBUG_PRINTF("\nReset reason: Software reset\n", "");
            ft_poweron_sequence();
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\nReset reason: Panic/exception reset\n", "");
            break;
    }
}
 
