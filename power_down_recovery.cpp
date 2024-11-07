/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power_down_recovery.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:26 by raleksan          #+#    #+#             */
/*   Updated: 2024/05/29 13:02:27 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void IRAM_ATTR  ft_brownout_handle(void)
{
    DEBUG_PRINTF("\n[BOOTING INFO] Processor reset reason: Brown-out reset! Going into extensive sleep\n", "");
    ft_go_to_sleep(DEAD_BATTERY_SLEEP);
}

void  ft_power_down_recovery(void)
{
    esp_reset_reason_t  reset_reason;

    reset_reason = esp_reset_reason();
    switch (reset_reason)
    {
        case ESP_RST_BROWNOUT:
            ft_brownout_handle();
            break;
        case ESP_RST_POWERON:
            DEBUG_PRINTF("\n[BOOTING INFO] Processor reset reason: Power-on reset\n", "");
            break;
        case ESP_RST_SW:
            DEBUG_PRINTF("\n[BOOTING INFO] Processor reset reason: Software reset\n", "");
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\n[BOOTING INFO] Processor reset reason: Panic/exception reset\n", "");
            break;
    }
//    ft_data_integrity_check();
    DEBUG_PRINTF("\n[BOOTING INFO] Power-down Recovery was performed.\n\n", "");
}
 
