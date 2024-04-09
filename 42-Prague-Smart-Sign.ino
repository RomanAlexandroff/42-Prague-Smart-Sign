/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42-Prague-Smart-Sign.ino                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:10 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 12:59:16 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  setup(void)
{
    #ifdef DEBUG
        ft_serial_init();
    #endif
    ft_spiffs_init();
    #if !defined(XIAO_ESP32C3)
        ft_battery_init();
    #endif
    ft_display_init();
    ft_power_down_recovery();
    #if !defined(XIAO_ESP32C3)
        ft_battery_check();
    #endif
//    ft_buttons_init();
}

static void  ft_pathfinder(void)
{
    unsigned int  sleep_length;

    if (rtc_g.exam_status)
        ft_exam_mode(&sleep_length);
    else
        ft_cluster_number_mode(&sleep_length);
    ft_go_to_sleep(sleep_length);
    DEBUG_PRINTF("  ---- This message will never be printed out", "");
}

void  loop(void)
{
    ft_pathfinder();
}
 
