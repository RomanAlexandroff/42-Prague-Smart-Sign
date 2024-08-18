/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utilities.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:03:05 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:03:07 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

bool  ft_unix_timestamp_decoder(uint8_t* p_day, uint8_t* p_month, uint16_t* p_year)
{
    if (rtc_g.secret_expiration < 1000000000)
        return (false);
    struct tm* time_info = localtime(&rtc_g.secret_expiration);
    *p_day = time_info->tm_mday;
    *p_month = time_info->tm_mon + 1;
    *p_year = time_info->tm_year + 1900;
    return (true);
}    

int8_t  ft_expiration_counter(void)
{
    const int months_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t   expire_day;
    uint8_t   expire_month;
    uint16_t  expire_year;
    int       month;

    if (!ft_unix_timestamp_decoder(&expire_day, &expire_month, &expire_year))
        return (FAILED_TO_COUNT);
    if (expire_month == rtc_g.month)
        return (expire_day - rtc_g.day);
    else
        return (expire_day + months_days[rtc_g.month - 1] - rtc_g.day);
}

bool  ft_get_time(void)
{
    const char* ntp_server PROGMEM = "pool.ntp.org";
    const long  gmt_offset_sec = TIME_ZONE * 3600;
    const int   daylight_offset_sec PROGMEM = 3600;
    struct tm   time_info;

    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Failed to obtain time due to Wi-Fi connection issues\n", "");
        return (false);
    }
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    if(!getLocalTime(&time_info))
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Failed to obtain time from the NTP server\n", "");
        return (false);
    }
    if (time_info.tm_isdst < 0)
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Daylight Saving Time is not available\n", "");
        return (false);
    }
    rtc_g.hour = time_info.tm_hour;
    rtc_g.minute = time_info.tm_min;
    rtc_g.day = time_info.tm_mday;
    rtc_g.month = 1 + time_info.tm_mon;
    rtc_g.year = 1900 + time_info.tm_year;
    rtc_g.daylight_flag = time_info.tm_isdst;
    DEBUG_PRINTF("\n[SYSTEM TIME] Obtained time from the NTP server as follows:\n", "");
    if (rtc_g.daylight_flag)
        DEBUG_PRINTF("  --daylight saving time is ACTIVE (summer time)\n", "");
    else
        DEBUG_PRINTF("  --daylight saving time is INACTIVE (winter time)\n", "");
    DEBUG_PRINTF("  --hour:   %d\n", rtc_g.hour);
    DEBUG_PRINTF("  --minute: %d\n", rtc_g.minute);
    DEBUG_PRINTF("  --day:    %d\n", rtc_g.day);
    DEBUG_PRINTF("  --month:  %d\n", rtc_g.month);
    DEBUG_PRINTF("  --year:   %d\n\n", rtc_g.year);
    return (true);
}

static int  ft_find_last_element(const uint8_t *wakeup_hour)
{
    return (sizeof(wakeup_hour) / sizeof(wakeup_hour[0]));
}

unsigned int  ft_time_till_wakeup(void)
{
    const uint8_t wakeup_hour[] = {WAKE_UP_HOURS};
    uint8_t       i;

    i = ft_find_last_element(wakeup_hour);
    if (rtc_g.hour >= wakeup_hour[i])
        return ((wakeup_hour[0] + 24 - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000) - millis());
    i = 0;
    while ((wakeup_hour[i] - rtc_g.hour) <= 0)
        i++;
    return ((wakeup_hour[i] - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000) - millis());
}

unsigned int  ft_time_till_event(int8_t hours, uint8_t minutes)
{
    unsigned int result;

    result = (hours - rtc_g.hour) * 3600000;
    result += (minutes * 60000) - (rtc_g.minute * 60000);
    if (result > 86400000)
        result = 86400000;
    if (result < 10)
        result = 10;
    return (result);
}

int  ft_time_sync(unsigned int preexam_time)
{
    int minutes;

    minutes = ceil(preexam_time / 1000);
    while (minutes % 10 != 0)
    {
        preexam_time = preexam_time - 1000;
        minutes = ceil(preexam_time / 1000);
        ft_delay(1000);
    }
    minutes = ceil(preexam_time / 60000);
    while (minutes % 10 != 0 || minutes > 60)
    {
        ft_delay(59990);
        minutes--;
    }
    return (minutes);
}
 
