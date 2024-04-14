/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
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

// Uncomment for ESP32 or ESP32—S2—WROOM-I
//extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT/2> display;

// Uncomment for ESP32—C3—1M or XIAO_ESP32C3
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display;

extern WiFiClientSecure client;
extern UniversalTelegramBot bot;

struct RTC_GlobalsManagement {
    bool     spiffs_state;
    bool     warning_active;
    bool     ota_active;
    String   Secret;
    int16_t  secret_checksum;
    time_t   secret_expiration;
    String   chat_id;
    String   from_name;
    bool     exam_status;
    uint16_t exam_start_hour;
    uint16_t exam_start_minutes;
    uint8_t  exam_end_hour;
    uint8_t  exam_end_minutes;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  day;
    uint8_t  month;
    uint16_t year;
};
extern struct RTC_GlobalsManagement rtc_g;

struct GlobalsManagement {
    uint16_t battery;
};
extern struct GlobalsManagement g;

#endif
 
