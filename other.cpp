
#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    display.powerOff();
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("Going to sleep for %u seconds.\n", time_in_millis / 1000);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_deep_sleep_start();
}

void   ft_delay(uint64_t time_in_millis)
{
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
}

void  ft_wifi_connect(void)
{
    short                     i;
    RTC_DATA_ATTR static bool reboot;

    i = 0;
    reboot = false;
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT)
    {
        delay(1000);
        i++;
    }
    if ((WiFi.status() != WL_CONNECTED) && !reboot)
    {
        DEBUG_PRINTF("Failed to connect to Wi-Fi. Rebooting...\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        reboot = true;
        ESP.restart();
    }
    reboot = false;   
}

void  ft_get_time(void)
{
    const char* ntp_server PROGMEM = "pool.ntp.org";
    const long  gmt_offset_sec = TIME_ZONE * 3600;
    const int   daylight_offset_sec PROGMEM = 3600;
    struct tm   time_info;

    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("Failed to obtain time due to Wi-Fi connection issues\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    if(!getLocalTime(&time_info))
    {
        DEBUG_PRINTF("Failed to obtain time from the NTP server\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    rtc_g.hour = time_info.tm_hour;
    rtc_g.minute = time_info.tm_min;
    rtc_g.day = time_info.tm_mday;
    rtc_g.month = time_info.tm_mon;
    rtc_g.year = 1900 + time_info.tm_mon;
}

unsigned int  ft_time_till_event(uint8_t hours, uint8_t minutes)                  // counts exact time till events with 1 minute precision
{
    unsigned int result;

    result = (hours - rtc_g.hour) * 3600000;
    result += (minutes * 60000) - (rtc_g.minute * 60000);
    if (result <= 0)                                                              // нельзя уходить в сон на 0 миллисекунд...
        result = 10;                                                              // ...иначе микропроцессор из него не проснётся самостоятельно, только с перезагрузкой
    return (result);
}
 
 
