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
}

void IRAM_ATTR  isr_warning(void)
{
    DEBUG_PRINTF("  ---- Warning Button was pressed\n", "");
    rtc_g.warning_active = !rtc_g.warning_active;
    if (rtc_g.warning_active)
        ft_draw_colour_bitmap(exam_warning_black, exam_warning_red);
    else
        ft_display_cluster_number(STOCK);
}

void  ft_buttons_deinit(void)
{
/*    detachInterrupt(DIAGNOSTICS_BUTTON);
    pinMode(DIAGNOSTICS_BUTTON, INPUT);
    detachInterrupt(OTA_BUTTON);
    pinMode(OTA_BUTTON, INPUT);
    detachInterrupt(WARNING_BUTTON);
    pinMode(WARNING_BUTTON, INPUT);
    esp_deep_sleep_enable_gpio_wakeup(DIAGNOSTICS_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW); // If button PULLED DOWN, wake up on HIGH
    esp_deep_sleep_enable_gpio_wakeup(OTA_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW);         // If button PULLED UP, wake up on LOW
    esp_deep_sleep_enable_gpio_wakeup(WARNING_BUTTON, ESP_GPIO_WAKEUP_GPIO_LOW);*/
}

void  ft_buttons_init(void)
{   
/*    gpio_new_pin_glitch_filter(const gpio_pin_glitch_filter_config_t *config, gpio_glitch_filter_handle_t *ret_filter);
    gpio_glitch_filter_enable(gpio_glitch_filter_handle_t filter);
    pinMode(DIAGNOSTICS_BUTTON, INPUT_PULLUP);                                       // If button PULLED UP, connect the other button pin to GND
    attachInterrupt(DIAGNOSTICS_BUTTON, isr_diagnostics, FALLING);
    pinMode(OTA_BUTTON, INPUT_PULLUP);                                               // If button PULLED DOWN, connect the other button pin to VCC
    attachInterrupt(OTA_BUTTON, isr_ota, FALLING);
    pinMode(WARNING_BUTTON, INPUT_PULLUP);
    attachInterrupt(WARNING_BUTTON, isr_warning, FALLING);*/
}
 
