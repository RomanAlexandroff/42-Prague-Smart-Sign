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
    rtc_g.ota_active = !rtc_g.ota_active;
    if (rtc_g.ota_active)
        ft_ota_init();
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
    esp_deep_sleep_enable_gpio_wakeup(DIAGNOSTICS_BUTTON, ESP_GPIO_WAKEUP_GPIO_HIGH); //1 = High; The button is pulled down with a 10K Ohm resistor
    esp_deep_sleep_enable_gpio_wakeup(OTA_BUTTON, ESP_GPIO_WAKEUP_GPIO_HIGH);         //1 = High; The button is pulled down with a 10K Ohm resistor
    esp_deep_sleep_enable_gpio_wakeup(WARNING_BUTTON, ESP_GPIO_WAKEUP_GPIO_HIGH);     //1 = High; The button is pulled down with a 10K Ohm resistor   
}

void  ft_buttons_init(void)
{
    pinMode(DIAGNOSTICS_BUTTON, INPUT_PULLUP);
    attachInterrupt(DIAGNOSTICS_BUTTON, isr_diagnostics, FALLING);
    pinMode(OTA_BUTTON, INPUT_PULLUP);
    attachInterrupt(OTA_BUTTON, isr_ota, FALLING);
    pinMode(WARNING_BUTTON, INPUT_PULLUP);
    attachInterrupt(WARNING_BUTTON, isr_warning, FALLING);
}
 
