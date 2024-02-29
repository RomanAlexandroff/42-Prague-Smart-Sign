
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

void  IRAM_ATTR ft_delay(uint64_t time_in_millis)
{
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
}

void  ft_wifi_connect(void)
{
    short              i;
    RTC_DATA_ATTR bool reboot;

    i = 0;
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT)
    {
        ft_delay(1000);
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
    const char* ntp_server = "pool.ntp.org";
    const long  gmt_offset_sec = TIME_ZONE * 3600;
    const int   daylight_offset_sec = 3600;
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
    g_hour = time_info.tm_hour;
    g_minute = time_info.tm_min;
    g_day = time_info.tm_mday;
    g_month = time_info.tm_mon;
}
 
