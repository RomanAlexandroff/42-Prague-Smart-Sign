/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:10:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/27 14:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    esp_err_t result;
    uint64_t  gpio_mask;

    ft_watchdog_stop();
    if (time_in_millis < REBOOT)
        time_in_millis = REBOOT;
    display.powerOff();
    gpio_mask = (1ULL << 2) | (1ULL << 3) | (1ULL << 4);
    result = esp_deep_sleep_enable_gpio_wakeup(gpio_mask, ESP_GPIO_WAKEUP_GPIO_LOW);
    if (result != ESP_OK)
        DEBUG_PRINTF("\nFailed to set up wake-up with a button.\n\n", "");
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("Going to sleep for %u seconds.\n", time_in_millis / 1000);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_deep_sleep_start();
}

/*
*   Delays and puts to sleep Wi-Fi and
*   Bluetooth modems to preserve
*   the battery charge
*/
void IRAM_ATTR  ft_delay(uint64_t time_in_millis)
{
    ft_watchdog_stop();
    if (time_in_millis < 10)
        time_in_millis = 10;
    esp_sleep_enable_timer_wakeup(time_in_millis * 1000);
    esp_light_sleep_start();
    ft_watchdog_start();
}

void  ft_wifi_connect(void)
{
    short i;

    i = 0;
    ft_watchdog_reset();
    WiFi.mode(WIFI_STA);
    WiFi.persistent(true);
    Telegram_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT)
    {
        ft_watchdog_reset();
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
    ft_watchdog_reset();
    Serial.begin(115200);
    while (i > 0)
    {
        DEBUG_PRINTF("-", "");
        delay(100);
        i--;
    }
    DEBUG_PRINTF("\n\nDEVICE START\nversion %s\n", String(SOFTWARE_VERSION));
}
 
