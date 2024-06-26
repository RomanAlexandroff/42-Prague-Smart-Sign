/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buttons_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:13 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:00:15 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  isr_diagnostics(void)
{
    DEBUG_PRINTF("  ---- Diagnostics Button was pressed\n", "");
}

void IRAM_ATTR  isr_ota(void)
{
    DEBUG_PRINTF("  ---- OTA Button was pressed\n", "");
    rtc_g.ota = !rtc_g.ota;
    if (rtc_g.ota == true)
    {
        ft_write_spiffs_file("/ota.txt", ACTIVE);
        ESP.restart();
    }
    else
        ft_write_spiffs_file("/ota.txt", CLOSED);
}

void IRAM_ATTR  isr_warning(void)
{
    DEBUG_PRINTF("  ---- Warning Button was pressed\n", "");
    rtc_g.warning_active = !rtc_g.warning_active;
    if (rtc_g.warning_active)
        ft_draw_colour_bitmap(exam_warning_black, exam_warning_red);
    else
        ft_display_cluster_number(DEFAULT);
}

void  ft_buttons_deinit(void)
{
    detachInterrupt(DIAGNOSTICS_BUTTON);
    pinMode(DIAGNOSTICS_BUTTON, INPUT);
    detachInterrupt(OTA_BUTTON);
    pinMode(OTA_BUTTON, INPUT);
    detachInterrupt(WARNING_BUTTON);
    pinMode(WARNING_BUTTON, INPUT);
    esp_deep_sleep_enable_gpio_wakeup(DIAGNOSTICS_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW); // If button PULLED DOWN, wake up on HIGH
    esp_deep_sleep_enable_gpio_wakeup(OTA_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW);         // If button PULLED UP, wake up on LOW
    esp_deep_sleep_enable_gpio_wakeup(WARNING_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW);   
}

void  ft_buttons_init(void)
{
    pinMode(DIAGNOSTICS_BUTTON, INPUT_PULLUP);                                       // If button PULLED UP, connect the other button pin to GND
    attachInterrupt(DIAGNOSTICS_BUTTON, isr_diagnostics, FALLING);
    pinMode(OTA_BUTTON, INPUT_PULLUP);                                               // If button PULLED DOWN, connect the other button pin to VCC
    attachInterrupt(OTA_BUTTON, isr_ota, FALLING);
    pinMode(WARNING_BUTTON, INPUT_PULLUP);
    attachInterrupt(WARNING_BUTTON, isr_warning, FALLING);
}
 
