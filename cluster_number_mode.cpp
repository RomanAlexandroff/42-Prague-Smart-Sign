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

void  ft_cluster_number_mode(unsigned int* p_sleep_length)
{
    uint8_t retries;
    bool    intra_connected;
    int8_t  days_left;

    retries = 0;
    intra_connected = false;
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    while (!ft_get_time() && retries < RETRIES_LIMIT)
    {
        retries++;
        if (retries == RETRIES_LIMIT)
        {
            ft_display_cluster_number(INTRA_ERROR);
            DEBUG_PRINTF("ERROR OBTAINING TIME. Cannot proceed. Turning off\n", "");
            *p_sleep_length = ft_time_till_wakeup();
            return;
        }
        DEBUG_PRINTF("\nRetrying in %d minute(s)\n\n", retries * 5);
        delay (retries * 300000);
    }
    retries = 0;
    while (!intra_connected && retries < RETRIES_LIMIT)
    {
        delay (retries * 300000);
        DEBUG_PRINTF("Fetching exams data — try #%d\n\n", retries + 1);
        intra_connected = ft_fetch_exams();
        retries++;
        if (!intra_connected && retries != RETRIES_LIMIT)
            DEBUG_PRINTF("\nRetrying in %d minute(s)\n\n", retries * 5);
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
    if (days_left <= 3 && days_left != FAILED_TO_COUNT)
    {
        ft_display_cluster_number(SECRET_EXPIRED);
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_EXPIRED, days_left), "");
        return;
    }
    ft_display_cluster_number(DEFAULT);
}
 
