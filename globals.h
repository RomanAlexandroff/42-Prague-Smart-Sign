/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:26 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:28 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H_
# define GLOBALS_H_

# include "constants.h"
# include <Wire.h>
# include <GxEPD2_BW.h>
# include <GxEPD2_3C.h>
# include "Fonts/FreeSansBold24pt7b.h"
# include <WiFi.h>
# include <WiFiClientSecure.h>
# include <UniversalTelegramBot.h>
# include "credentials.h"

extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display;
extern WiFiClientSecure Telegram_client;
extern WiFiClientSecure Intra_client;
extern UniversalTelegramBot bot;

/*
*   RTC globals preserve their values
*   over deep sleep. Others do not.
*/
struct rtc_global_variables {
    time_t   secret_expiration;
    uint16_t exam_start_hour;
    uint16_t exam_start_minutes;
    bool     warning_active;
    bool     exam_status;
    bool     ota;
    char     Secret[74];
    char     chat_id[15];
    char     from_name[33];
};
extern struct rtc_global_variables rtc_g;

struct common_global_variables {
    uint8_t  exam_end_hour;
    uint8_t  exam_end_minutes;
    bool     daylight_flag;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  day;
    uint8_t  month;
    uint16_t year;
};
extern struct common_global_variables com_g;

#endif
 
