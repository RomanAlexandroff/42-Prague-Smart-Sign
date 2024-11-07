/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster_number_mode.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/10/21 13:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void ft_report_exception(ERROR_t status)
{
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    if (WiFi.status() != WL_CONNECTED)
        return;
    bot.sendMessage(String(rtc_g.chat_id), ft_compose_message(status, 0), "");
}

static bool ft_handle_secret_expiration(void)
{
    int8_t  days_left;
    
    days_left = ft_expiration_counter();
    if (days_left <= 3 && days_left != FAILED_TO_COUNT)
    {
        ft_display_cluster_number(SECRET_EXPIRED);
        DEBUG_PRINTF("\n[INTRA] IT IS TIME TO UPDATE THE SECRET!\n", "");
        bot.sendMessage(String(rtc_g.chat_id), ft_compose_message(SECRET_EXPIRED, days_left), "");
        return (true);
    }
    return (false);
}

/*
*   Makes sure to get exams info or
*   display error and notify user
*/
static bool ft_ensure_exams(unsigned int* p_sleep_length)
{
    uint8_t retries;
    ERROR_t intra_status = UNKNOWN;
    
    retries = 0;
    while (intra_status != INTRA_OK && retries < RETRIES_LIMIT)
    {
        delay (retries * 300000);attribute
        DEBUG_PRINTF("\n[INTRA] Fetching exams data — try #%d\n\n", retries + 1);
        intra_status = ft_fetch_exams();
        retries++;
        if (intra_status != INTRA_OK && retries != RETRIES_LIMIT)
            DEBUG_PRINTF("\n[INTRA] Retrying in %d minute(s)\n", retries * 5);
    }
    if (intra_status != INTRA_OK)
    {
        ft_display_cluster_number(INTRA_ERROR);
        DEBUG_PRINTF("\n[INTRA] ERROR OBTAINING EXAMS. Turning off\n", "");
        ft_report_exception(intra_status);
        rtc_g.exam_status = false;
        *p_sleep_length = ft_time_till_wakeup();
        return (false);
    }
    return (true);
}

/*
*   Makes sure to get actual time or
*   display error and notify user.
*   Since default time is 00:00, sleep
*   length always calculates to 6 hours.
*/
static bool ft_ensure_time(unsigned int* p_sleep_length)
{
    uint8_t retries;
    ERROR_t time_status = UNKNOWN;
    
    retries = 0;
    while (time_status != TIME_OK && retries < RETRIES_LIMIT)
    {
        delay (retries * 300000);
        time_status = ft_get_time();
        retries++;
        if (time_status != TIME_OK && retries != RETRIES_LIMIT)
            DEBUG_PRINTF("\n[SYSTEM TIME] Retrying in %d minute(s)\n", retries * 5);
        
    }
    if (time_status != TIME_OK)
    {
        ft_display_cluster_number(INTRA_ERROR);
        DEBUG_PRINTF("\n[SYSTEM TIME] ERROR OBTAINING TIME. Cannot proceed. Turning off\n", "");
        ft_report_exception(time_status);
        rtc_g.exam_status = false;
        *p_sleep_length = ft_time_till_wakeup();
        return (false);
    }
    return (true);
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)
{
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    if (!ft_ensure_time(p_sleep_length))
        return;
    if (!ft_ensure_exams(p_sleep_length))
        return;
    if (rtc_g.exam_status)
    {
        if (com_g.hour >= (rtc_g.exam_start_hour - 1) && com_g.hour <= com_g.exam_end_hour)
            *p_sleep_length = REBOOT;
        else
        {
            ft_display_cluster_number(EXAM_DAY);
            *p_sleep_length = ft_time_till_event(rtc_g.exam_start_hour - 1, rtc_g.exam_start_minutes);
        }
        return;
    }
    *p_sleep_length = ft_time_till_wakeup();
    if (ft_handle_secret_expiration())
        return;
    ft_display_cluster_number(DEFAULT_IMG);        // Display ordinary day (no issues, fails or exams)
}
 
