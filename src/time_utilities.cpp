/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utilities.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/29 17:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

bool  ft_unix_timestamp_decoder(uint8_t* p_day, uint8_t* p_month, uint16_t* p_year)
{
    if (rtc_g.secret_expiration < 1000000000)
        return (false);
    ft_watchdog_reset();
    struct tm* time_info = localtime(&rtc_g.secret_expiration);
    *p_day = time_info->tm_mday;
    *p_month = time_info->tm_mon + 1;
    *p_year = time_info->tm_year + 1900;
    return (true);
}

int16_t  ft_expiration_counter(void)
{
    const int months_days[] = {MONTHS_DAYS};
    uint8_t   expire_day;
    uint8_t   expire_month;
    uint16_t  expire_year;
    uint16_t  year_correction;

    ft_watchdog_reset();
    if (!ft_unix_timestamp_decoder(&expire_day, &expire_month, &expire_year))
        return (FAILED_TO_COUNT);
    if (expire_year < com_g.year)
        return (FAILED_TO_COUNT);
    year_correction = (expire_year - com_g.year) * YEAR_DAYS;
    if (expire_month == com_g.month)
        return (expire_day + year_correction - com_g.day);
    else
        return (expire_day + year_correction + months_days[com_g.month - 1] - com_g.day);
}

ERROR_t  ft_get_time(void)
{
    const char* ntp_server PROGMEM = "pool.ntp.org";
    const long  gmt_offset_sec = TIME_ZONE * 3600;
    const int   daylight_offset_sec PROGMEM = 3600;
    struct tm   time_info;

    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Failed to obtain time due to Wi-Fi connection issues\n");
        return (TIME_NO_WIFI);
    }
    ft_watchdog_reset();
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    if(!getLocalTime(&time_info))
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Failed to obtain time from the NTP server\n");
        return (TIME_NO_SERVER);
    }
    if (time_info.tm_isdst < 0)
    {
        DEBUG_PRINTF("\n[SYSTEM TIME] Daylight Saving Time is not available\n");
        return (TIME_NO_DST);
    }
    com_g.hour = time_info.tm_hour;
    com_g.minute = time_info.tm_min;
    com_g.day = time_info.tm_mday;
    com_g.month = 1 + time_info.tm_mon;
    com_g.year = 1900 + time_info.tm_year;
    com_g.daylight_flag = time_info.tm_isdst;
    DEBUG_PRINTF("\n[SYSTEM TIME] Obtained time from the NTP server as follows:\n");
    if (com_g.daylight_flag)
        DEBUG_PRINTF("  --daylight saving time is ACTIVE (summer time)\n");
    else
        DEBUG_PRINTF("  --daylight saving time is INACTIVE (winter time)\n");
    DEBUG_PRINTF("  --hour:   %d\n", com_g.hour);
    DEBUG_PRINTF("  --minute: %d\n", com_g.minute);
    DEBUG_PRINTF("  --day:    %d\n", com_g.day);
    DEBUG_PRINTF("  --month:  %d\n", com_g.month);
    DEBUG_PRINTF("  --year:   %d\n\n", com_g.year);
    return (TIME_OK);
}

unsigned int  ft_time_till_wakeup(void)
{
    const uint8_t wakeup_hour[] = {WAKE_UP_HOURS};
    uint8_t       i;

    ft_watchdog_reset();
    i = sizeof(wakeup_hour) / sizeof(wakeup_hour[0]) - 1;
    if (com_g.hour >= wakeup_hour[i])
        return ((wakeup_hour[0] + 24 - com_g.hour) * MS_HOUR - (com_g.minute * MS_MINUTE) - millis());
    i = 0;
    while ((wakeup_hour[i] - com_g.hour) <= 0)
        i++;
    return ((wakeup_hour[i] - com_g.hour) * MS_HOUR - (com_g.minute * MS_MINUTE) - millis());
}


/*
*   Returns value in milliseconds. Max limit
*   is 24 hours. Min limit is 0,01 seconds.
*   Limits prevent indefinite sleep.
*/
unsigned int  ft_time_till_event(int8_t hours, uint8_t minutes)
{
    unsigned int result;

    ft_watchdog_reset();
    result = (hours - com_g.hour) * MS_HOUR;
    result += (minutes * MS_MINUTE) - (com_g.minute * MS_MINUTE);
    if (result > 86400000)
        result = 86400000;
    if (result < 10)
        result = 10;
    return (result);
}

int  ft_time_sync(unsigned int preexam_time)
{
    int minutes;

    ft_watchdog_stop();
    minutes = ceil(preexam_time / 1000);
    while (minutes % 10 != 0)
    {
        preexam_time = preexam_time - 1000;
        minutes = ceil(preexam_time / 1000);
        ft_delay(1000);
    }
    minutes = ceil(preexam_time / MS_MINUTE);
    while (minutes % 10 != 0 || minutes > 60)
    {
        ft_delay(59990);
        minutes--;
    }
    ft_watchdog_start();
    return (minutes);
}
 
