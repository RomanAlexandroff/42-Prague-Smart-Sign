/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster_number_mode.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:25 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:00:27 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_wakeup(void)
{
    const uint8_t wakeup_hour[] = {6, 9, 12, 15, 18};
    uint8_t       i;

    if (rtc_g.hour >= 18)
        return ((wakeup_hour[0] + 24 - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000) - millis());
    i = 0;
    while ((wakeup_hour[i] - rtc_g.hour) <= 0)
        i++;
    return ((wakeup_hour[i] - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000) - millis());
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)
{
    RTC_DATA_ATTR static uint8_t retries = 0;
    bool                         intra_connected;
    int8_t                       days_left;

    intra_connected = false;
    ft_wifi_connect();
    ft_telegram_check();
    if (!ft_get_time())
    {
        rtc_g.exam_status = false;
        if (retries == 4)
        {
            retries = 0;
            ft_display_cluster_number(INTRA_ERROR);
            *p_sleep_length = ft_time_till_wakeup();
            DEBUG_PRINTF("ERROR OBTAINING TIME. Cannot proceed. Turning off\n", "");
            return;
        }
        *p_sleep_length = 60000;
        retries++;
        DEBUG_PRINTF("Retrying in 1 minute\n", "");
        return;
    }
    retries = 0;
    while (!intra_connected && retries < 2)
    {
        delay (retries * 300000);
        DEBUG_PRINTF("Fetching exams data — try #%d\n\n", retries + 1);
        intra_connected = ft_fetch_exams();
        retries++;
        if (!intra_connected && retries < 3)
            DEBUG_PRINTF("\nRetrying in %d minutes\n\n", retries * 5);
    }
    if (!intra_connected && rtc_g.hour <= 18)
    {
        ft_display_cluster_number(INTRA_ERROR);
        rtc_g.exam_status = false;
    }
    if (rtc_g.exam_status)
    {
        if (rtc_g.hour >= (rtc_g.exam_start_hour - 1) && rtc_g.hour <= rtc_g.exam_end_hour)
            *p_sleep_length = 10;
        else
        {
            ft_display_cluster_number(EXAM_DAY);
            *p_sleep_length = ft_time_till_event(rtc_g.exam_start_hour - 1, rtc_g.exam_start_minutes);
        }
        return;
    }
    else
        *p_sleep_length = ft_time_till_wakeup();
    if (!intra_connected)
        return;
    days_left = ft_expiration_counter();
    if (days_left <= 3 && days_left != -128)
    {
        ft_display_cluster_number(SECRET_EXPIRED);
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_EXPIRED, days_left), "");
        return;
    }
    ft_display_cluster_number(DEFAULT);
}
 
