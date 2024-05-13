/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battery_management.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:29 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 12:59:32 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static bool ft_charging_detection(void)
{
    short i;
    int   new_result;
    int   old_result;
    int   counter;

    i = 5;
    counter = 0;
    old_result = g.battery;
    while(i)
    {
        new_result = adc1_get_raw(ADC1_CHANNEL_0);
        if (new_result < 550)
            return (false);
        if ((old_result - new_result) > 0)
            counter++;
        old_result = new_result;
        i--;
        delay(60000);
    }
    if (counter >= 4)
        return (false);
    return (true);
}

void  ft_battery_check(void)
{
    short i;

    i = 0;
    while (i < 10)
    {
        g.battery += adc1_get_raw(ADC1_CHANNEL_0);
        delay(100);
        i++;
    }
    g.battery = g.battery / i;
    if (g.battery < 400)
    {
//        ft_display_bitmap_with_refresh(empty_battery_img);
        DEBUG_PRINTF("\nBattery is too low. Going into extensive sleep\n", "");
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(DEAD_BATTERY, 0), "");
        ft_go_to_sleep(DEAD_BATTERY_SLEEP);
    }
    if (g.battery < 700 && g.battery > 600)
    {
        if (ft_charging_detection())
        {
            DEBUG_PRINTF("\nThe device is charging...\n", "");
            return;
        }
    }
    if (g.battery < 800)
    {
        ft_display_cluster_number(LOW_BATTERY);
        DEBUG_PRINTF("\nLow battery! Need charging!\n", "");
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(LOW_BATTERY, 0), "");
    }
}

void  ft_battery_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
}
 
