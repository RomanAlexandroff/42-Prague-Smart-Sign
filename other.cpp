/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:17 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:18 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    if (time_in_millis < 10)
        time_in_millis = 10;
    client.stop();
    display.powerOff();
    ft_buttons_deinit();
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("Going to sleep for %u seconds.\n", time_in_millis / 1000);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_deep_sleep_start();
}

void IRAM_ATTR  ft_delay(uint64_t time_in_millis)
{
    if (time_in_millis < 10)
        time_in_millis = 10;
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
}

void  ft_wifi_connect(void)
{
    short i;

    i = 0;
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT)
    {
        delay(1000);
        i++;
    }
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
}

void  ft_serial_init(void)
{
    uint8_t i;
    
    i = 15;
    Serial.begin(115200);
    while (i)
    {
        DEBUG_PRINTF("-", "");
        delay(100);
        i--;
    }
    DEBUG_PRINTF("\n\nDEVICE START\nversion %s\n", String(SOFTWARE_VERSION));
}
 
