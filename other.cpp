/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:17 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:18 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    client.stop();
    display.powerOff();
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("Going to sleep for %u seconds.\n", time_in_millis / 1000);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_deep_sleep_start();
}

void IRAM_ATTR  ft_delay(uint64_t time_in_millis)
{
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
}

void  ft_wifi_connect(void)
{
    short                     i;

    i = 0;
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT)
    {
        delay(1000);
        i++;
    }
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
}

int16_t ft_checksum(String input, int16_t checksum)
{
    const char *str;
    uint8_t    i;
    int16_t    result;

    i = 0;
    result = 0;
    str = input.c_str();
    if (!str)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Test canceled. No input was provided.\n", "");
        return (-1);
    }
    while (str[i])
    {
        result += str[i];
        i++;
    }
    if (checksum && checksum == result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Test successfully passed.\n", "");
        return (1);
    }
    if (checksum && checksum != result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Data corruption detected!\n", "");
        return (0);
    }
    if (!checksum)
    {
        DEBUG_PRINTF("\n[CHECKSUM] New checksum was successfully generated.\n", "");
        return (result);
    }
    DEBUG_PRINTF("\n[CHECKSUM] Unknown error.\n", "");
    return (-1);
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
    DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
}
 
