/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:10:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/12/18 13:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_go_to_sleep(uint64_t time_in_millis)
{
    esp_err_t result;

    ft_watchdog_stop();
    if (time_in_millis < MIN_SLEEP_LIMIT_MS)
        time_in_millis = MIN_SLEEP_LIMIT_MS;
    if (time_in_millis > MAX_SLEEP_LIMIT_MS)
        time_in_millis = MAX_SLEEP_LIMIT_MS;
    display.powerOff();
    result = esp_deep_sleep_enable_gpio_wakeup(GPIO_MASK, ESP_GPIO_WAKEUP_GPIO_LOW);
    if (result != ESP_OK)
        DEBUG_PRINTF("\nFailed to set up wake-up with a button.\n\n");
    DEBUG_PRINTF("The device was running for %lu second(s) this time\n", (millis() / mS_TO_S_FACTOR));
    DEBUG_PRINTF("Going to sleep for %llu seconds.\n", time_in_millis / mS_TO_S_FACTOR);
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n");
    esp_sleep_enable_timer_wakeup(time_in_millis * mS_TO_uS_FACTOR);
    esp_deep_sleep_start();
}

/*
*   Delays and puts to sleep Wi-Fi and
*   Bluetooth modems to preserve
*   the battery charge
*/
void  ft_delay(uint64_t time_in_millis)
{
    ft_watchdog_stop();
    if (time_in_millis < MIN_SLEEP_LIMIT_MS)
        time_in_millis = MIN_SLEEP_LIMIT_MS;
    if (time_in_millis > MAX_SLEEP_LIMIT_MS)
        time_in_millis = MAX_SLEEP_LIMIT_MS;
    esp_sleep_enable_timer_wakeup(time_in_millis * mS_TO_uS_FACTOR);
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
    while ((WiFi.status() != WL_CONNECTED) && i < CONNECT_TIMEOUT_S)
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
    Serial.begin(BAUD_RATE);
    while (i > 0)
    {
        DEBUG_PRINTF("-");
        delay(100);
        i--;
    }
    DEBUG_PRINTF("\n\nDEVICE START\nversion %.2f\n", SOFTWARE_VERSION);
}
 
