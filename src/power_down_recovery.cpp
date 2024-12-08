/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power_down_recovery.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/27 14:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void ft_report_reboot_reason(esp_reset_reason_t reset_reason)
{
    ft_watchdog_reset();
    if (reset_reason == ESP_RST_UNKNOWN)
        DEBUG_PRINTF("[BOOTING INFO] Unknown reset reason\n");
    else if (reset_reason == ESP_RST_POWERON)
        DEBUG_PRINTF("[BOOTING INFO] Power on reset\n");
    else if (reset_reason == ESP_RST_EXT)
        DEBUG_PRINTF("[BOOTING INFO] External reset\n");
    else if (reset_reason == ESP_RST_SW)
        DEBUG_PRINTF("[BOOTING INFO] Software reset\n");
    else if (reset_reason == ESP_RST_PANIC)
        DEBUG_PRINTF("[BOOTING INFO] CPU Panic reset\n");
    else if (reset_reason == ESP_RST_INT_WDT)
        DEBUG_PRINTF("[BOOTING INFO] Interrupt watchdog reset\n");
    else if (reset_reason == ESP_RST_TASK_WDT)
        DEBUG_PRINTF("[BOOTING INFO] Task watchdog reset\n");
    else if (reset_reason == ESP_RST_WDT)
        DEBUG_PRINTF("[BOOTING INFO] General watchdog reset\n");
    else if (reset_reason == ESP_RST_DEEPSLEEP)
        DEBUG_PRINTF("[BOOTING INFO] Deep sleep reset\n");
    else if (reset_reason == ESP_RST_BROWNOUT)
        DEBUG_PRINTF("[BOOTING INFO] Brownout reset\n");
    else if (reset_reason == ESP_RST_SDIO)
        DEBUG_PRINTF("[BOOTING INFO] SDIO reset\n");
    else
        DEBUG_PRINTF("[BOOTING INFO] Reset info is missing\n");
}

void  ft_power_down_recovery(void)
{
    esp_reset_reason_t  reset_reason;

    ft_watchdog_reset();
    reset_reason = esp_reset_reason();
    if (reset_reason == ESP_RST_BROWNOUT)
    {
        DEBUG_PRINTF("\n[BOOTING INFO] Brown-out reset! Going into extensive sleep\n");
        ft_go_to_sleep(DEAD_BATTERY_SLEEP);
    }
    ft_report_reboot_reason(reset_reason);
    if (reset_reason != ESP_RST_DEEPSLEEP)
    {
        DEBUG_PRINTF("\n[BOOTING INFO] Hard reset detected. Restoring RTC memory data...\n");
        ft_data_integrity_check();
    }
    DEBUG_PRINTF("\n[BOOTING INFO] Power-down Recovery was performed.\n\n");
}
 
