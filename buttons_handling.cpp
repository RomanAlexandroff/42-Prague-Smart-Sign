/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buttons_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/12 17:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  isr_diagnostics(void)
{
    unsigned long interrupt_time;

    interrupt_time = millis();
    if (interrupt_time - com_g.last_diagnostics > DEBOUNCE_DELAY)
    {
        com_g.last_diagnostics = interrupt_time;
        /* PUT SOME USEFUL CODE HERE */
    }
}

void IRAM_ATTR  isr_ota(void)
{
    unsigned long interrupt_time;

    interrupt_time = millis();
    if (interrupt_time - com_g.last_ota > DEBOUNCE_DELAY)
    {
        com_g.last_ota = interrupt_time;
        rtc_g.ota = !rtc_g.ota;
    }
}

void IRAM_ATTR  isr_warning(void)
{
    unsigned long interrupt_time;

    interrupt_time = millis();
    if (interrupt_time - com_g.last_warning > DEBOUNCE_DELAY)
    {
        com_g.last_warning = interrupt_time;
        rtc_g.warning_active = !rtc_g.warning_active;
    }
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
 
