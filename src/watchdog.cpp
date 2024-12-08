/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:50:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/12/06 16:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_watchdog_start(void)
{
    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();
}

void  ft_watchdog_reset(void)
{
    esp_task_wdt_reset();
}

void  ft_watchdog_stop(void)
{
    esp_task_wdt_delete(NULL);
}

void  ft_watchdog_init(void)
{
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = WD_TIMEOUT,
        .idle_core_mask = (1 << WD_NUMBER_OF_CORES) - 1,    // Bitmask of all cores
        .trigger_panic = WD_RESET_INFO,
    };
    esp_task_wdt_deinit();
    esp_task_wdt_init(&twdt_config);
    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();
}
 
