/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42-Prague-Smart-Sign.ino                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:10 by raleksan          #+#    #+#             */
/*   Updated: 2024/05/29 12:59:16 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  setup(void)
{
    #ifdef DEBUG
        ft_serial_init();
    #endif
    ft_spiffs_init();
    ft_buttons_init();
    ft_battery_init();
    ft_display_init();
    ft_power_down_recovery();
    ft_battery_check();
    ft_telegram_check();
//    ft_ota_init();                                // firewall blocks OTA over Wi-Fi
}

void  loop(void)
{
//    ft_ota_waiting_loop();                        // firewall blocks OTA over Wi-Fi
    ft_pathfinder();
}

static void  ft_pathfinder(void)
{
    unsigned int  sleep_length;

    if (rtc_g.exam_status)
        ft_exam_mode();
    if (!rtc_g.exam_status)                         // do not change
        ft_cluster_number_mode(&sleep_length);
    ft_go_to_sleep(sleep_length);
    DEBUG_PRINTF("  ---- This message will never be printed out", "");
}
 
