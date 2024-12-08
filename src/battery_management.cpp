/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battery_management.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/12/07 13:20:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_battery_check(void)
{
    int8_t  samples_count;
    int16_t battery;

    samples_count = 0;
    battery = 0;
    ft_watchdog_reset();
    while (samples_count < BATTERY_SAMPLES_LIMIT)
    {
        battery += adc1_get_raw(ADC1_CHANNEL_0);
        delay(100);
        samples_count++;
    }
    battery = battery / samples_count;
    DEBUG_PRINTF("\n[BATTERY] Current battery state: %d\n", battery);
    if (battery >= BATTERY_GOOD)
        return;
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();    
    if (battery < BATTERY_CRITICAL)
    {
        ft_display_cluster_number(LOW_BATTERY);
        DEBUG_PRINTF("\n[BATTERY] Battery is too low. Going into extensive sleep\n");
        bot.sendMessage(String(rtc_g.chat_id), ft_compose_message(DEAD_BATTERY, 0), "");
        ft_go_to_sleep(DEAD_BATTERY_SLEEP);
    }
    else if (battery < BATTERY_GOOD)
    {
        ft_display_cluster_number(LOW_BATTERY);
        DEBUG_PRINTF("\n[BATTERY] Low battery! Need charging!\n");
        bot.sendMessage(String(rtc_g.chat_id), ft_compose_message(LOW_BATTERY, 0), "");
    }
}

void  ft_battery_init(void)
{
    ft_watchdog_reset();
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
}
 
