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

static void ft_report_exception(ERROR_t intra_status)
{
    ft_display_cluster_number(INTRA_ERROR);
    rtc_g.exam_status = false;
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    if (WiFi.status() != WL_CONNECTED)
        return;
    if (intra_status == INTRA_NO_SERVER)
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(INTRA_NO_SERVER, 0), "");
    else if (intra_status == INTRA_NO_TOKEN)
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(INTRA_NO_TOKEN, 0), "");
    else if (intra_status == INTRA_NO_INFO)
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(INTRA_NO_INFO, 0), "");
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)
{
    uint8_t retries;
    ERROR_t intra_status = UNKNOWN;
    int8_t  days_left;

    retries = 0;
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();

    /* MAKE SURE TO HAVE ACTUAL TIME OR TURN OFF FOR 6 HOURS */
    while (!ft_get_time() && retries < RETRIES_LIMIT)
    {
        retries++;
        if (retries == RETRIES_LIMIT)
        {
            ft_display_cluster_number(INTRA_ERROR);
            DEBUG_PRINTF("\n[SYSTEM TIME] ERROR OBTAINING TIME. Cannot proceed. Turning off\n", "");
            *p_sleep_length = ft_time_till_wakeup();
            return;
        }
        DEBUG_PRINTF("\n[SYSTEM TIME] Retrying in %d minute(s)\n", retries * 5);
        delay (retries * 300000);
    }

    /* MAKE SURE TO GET EXAMS INFO OR DISPLAY ERROR */
    retries = 0;
    while (intra_status != INTRA_OK && retries < RETRIES_LIMIT)
    {
        delay (retries * 300000);
        DEBUG_PRINTF("\n[INTRA] Fetching exams data — try #%d\n\n", retries + 1);
        intra_status = ft_fetch_exams();
        retries++;
        if (!intra_status && retries != RETRIES_LIMIT)
            DEBUG_PRINTF("\n[INTRA] Retrying in %d minute(s)\n", retries * 5);
    }
    if (intra_status != INTRA_OK)
        ft_report_exception(intra_status);

    /* CALCULATE HOW LONG TO SLEEP TILL PRE-EXAM ANNOUNCEMENT AND TURN OFF */
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

    /* CALCULATE HOW LONG TO SLEEP TILL NEXT WAKE-UP */
    else
        *p_sleep_length = ft_time_till_wakeup();
    
    /* IF FAILED TO GET EXAMS INFO, TURN OFF HERE TILL NEXT WAKE-UP */
    if (intra_status != INTRA_OK)
        return;
    
    /* CALCULATE THE SECRET EXPIRATION AND DISPLAY + NOTIFY */
    days_left = ft_expiration_counter();
    if (days_left <= 3 && days_left != FAILED_TO_COUNT)
    {
        ft_display_cluster_number(SECRET_EXPIRED);
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_EXPIRED, days_left), "");
        return;
    }

    /* DISPLAY ORDINARY DAY (NO ISSUES, NO FAILS, NO EXAMS) AND TURN OFF TILL NEXT WAKE-UP */
    ft_display_cluster_number(DEFAULT);
}
 
