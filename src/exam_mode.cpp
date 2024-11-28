/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exam_mode.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/27 13:40:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static unsigned int ft_exam(void)
{
    unsigned int  exam_remaining_time;

    ft_watchdog_reset();
    exam_remaining_time = ft_time_till_event(com_g.exam_end_hour, com_g.exam_end_minutes);
    DEBUG_PRINTF("\n[THE DISPLAY] Drawing the Exam sign...\n", "");
    ft_draw_colour_bitmap(exam_warning_black, exam_warning_red);                          // execution takes 25 sec
    DEBUG_PRINTF("[THE DISPLAY] The drawing process is complete\n", "");
    rtc_g.exam_status = false;
    return (exam_remaining_time);
}

/*
*   Waits out until exam using light sleep.
*   Drawing functions, ft_time_sync and
*   ft_delay have their own watchdog resets.
*/
static void ft_preexam_warning(unsigned int* p_preexam_time)
{
    int minutes;

    minutes = ft_time_sync(*p_preexam_time);
    if (minutes == 60 || minutes == 50)
    {
        DEBUG_PRINTF("\n[THE DISPLAY] Drawing the Reservation sign...\n", "");
        ft_draw_colour_bitmap(preexam_50mins, preexam_warning_red);                       // execution takes 25 sec
        DEBUG_PRINTF("[THE DISPLAY] The drawing process is complete\n", "");
        ft_delay((minutes - 40) * 60000);
        minutes = 40;
    }
    if (minutes == 40 || minutes == 30 || minutes == 20)
    {
        ft_draw_colour_bitmap(preexam_25mins, preexam_warning_red);                       // execution takes 25 sec
        ft_delay((minutes - 10) * 60000);
        minutes = 10;
    }
    if (minutes == 10)
    {
        ft_draw_colour_bitmap(preexam_5mins, preexam_warning_red);                        // execution takes 25 sec
        ft_delay(480000);
    }
    *p_preexam_time = 0;
}


/*
*   Under normal circumstances the execution
*   time of this function is precisely 1 hour.
*   The function also accounts for situations
*   when it starts executing too early or too
*   late due to any timing errors.
*   
*   Two calls to get time are not redundant.
*/
void  ft_exam_mode(void)
{
    unsigned int  preexam_time;

    preexam_time = 0;
    ft_watchdog_reset();
    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    ft_get_time();
    ft_fetch_exams();
    if (!rtc_g.exam_status)
        return;
    preexam_time = ft_time_till_event(rtc_g.exam_start_hour, rtc_g.exam_start_minutes);
    if (preexam_time > 600000)
        ft_preexam_warning(&preexam_time);
    if (preexam_time <= 600000 && preexam_time >= 25000)
    {
        ft_delay(preexam_time - 25000);
        preexam_time = 0;
    }
    ft_get_time();
    ft_go_to_sleep(ft_exam());
}
 