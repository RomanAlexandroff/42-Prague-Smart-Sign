/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:50:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/27 15:50:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR ft_watchdog_start(void)
{
    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();
}

void IRAM_ATTR ft_watchdog_reset(void)
{
    esp_task_wdt_reset();
}

void IRAM_ATTR ft_watchdog_stop(void)
{
    esp_task_wdt_delete(NULL);
}

void  ft_watchdog_init(void)
{
    esp_task_wdt_deinit();
    esp_task_wdt_init(WD_TIMEOUT, WD_RESET_INFO);
    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();
}
 
