/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exam_mode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:07 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:09 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static unsigned int ft_exam(bool* p_preexam_check)
{
    int           minutes;
    unsigned int  exam_remaining_time;

    minutes = 60;
    exam_remaining_time = ft_time_till_event(rtc_g.exam_end_hour, rtc_g.exam_end_minutes);
    ft_draw_colour_bitmap(exam_warning_black, exam_warning_red);
    if (exam_remaining_time > 3600000)
    {
//        ft_display_timer_exact_end_time();
        return (exam_remaining_time - 3600000);
    }
    else
    {
        while (minutes > 10)
        {
//            ft_display_timer(minutes);
            ft_delay(600000);
            minutes -= 10;
        }
        while (minutes > 0)
        {
//            ft_display_timer(minutes);
            ft_delay(60000);
            minutes -= 1;
        }
    }
    rtc_g.exam_status = false;
    *p_preexam_check = false;
    return (10);
}

static void ft_preexam_warning(unsigned int* p_preexam_time)
{
    int minutes;

    minutes = ft_time_sync(*p_preexam_time);
    ft_draw_colour_bitmap(preexam_50mins, preexam_warning_red);
    while (minutes > 10)
    {
//        ft_display_timer(minutes);
        ft_delay(600000);
        minutes -= 10;
    }
    while (minutes > 0)
    {
//        ft_display_timer(minutes);
        ft_delay(60000);
        minutes -= 1;
    }
    *p_preexam_time = 0;
}

void  ft_exam_mode(unsigned int* p_sleep_length)
{
    RTC_DATA_ATTR static bool preexam_check;
    unsigned int              preexam_time;

    ft_wifi_connect();
    if (!preexam_check)
    {
        ft_fetch_exams();
        if (!rtc_g.exam_status)
            return;
        preexam_check = true;
    }
    if (!ft_get_time())
        rtc_g.hour += 1;
    preexam_time = ft_time_till_event(rtc_g.exam_start_hour, rtc_g.exam_start_minutes);
    if (preexam_time > 4200000)
        preexam_time = 3600000;
    if (preexam_time > 600000)
        ft_preexam_warning(&preexam_time);
    if (preexam_time <= 600000 && preexam_time >= 25000)
    {
        ft_delay(preexam_time - 25000);
        preexam_time = 0;
    }
    if (preexam_time < 25000)
        *p_sleep_length = ft_exam(&preexam_check);
}
 
