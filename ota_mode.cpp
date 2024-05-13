/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ota_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:05 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:10 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void IRAM_ATTR ft_ota_waiting_loop(void)
{
    int i;

    i = 0;
    DEBUG_PRINTF("\nOTA initialised. Waiting for update...\n", "");
    ft_display_cluster_number(OTA_WAITING);
    while (i < WAIT_FOR_OTA_LIMIT)
    {
        ArduinoOTA.handle();
        ft_delay(500);
        i++;
        if (!rtc_g.ota_active)
        {
            ft_display_cluster_number(OTA_CANCELED);
            DEBUG_PRINTF("\nOTA canceled without an update: USER TOUCH PAD\n", "");
            ft_delay(8000);
            return;
        }
    }
    DEBUG_PRINTF("\nOTA canceled without an update: AUTOMATIC TIME-OUT\n", "");
}

void IRAM_ATTR  ft_ota_init(void)
{
    const char* ssid;
    uint16_t    maxlen;
    const char* nameprefix;
    char*       fullhostname;
    uint8_t     mac[6];

    ssid = WiFi.SSID().c_str();
    nameprefix = DEVICE_NAME;
    maxlen = strlen(nameprefix) + 7;
    fullhostname = new char[maxlen];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else
            type = "filesystem";
        ft_display_cluster_number(OTA_UPDATING);
    });
    ArduinoOTA.onEnd([]() {
        ft_display_cluster_number(OTA_SUCCESS);
        ft_delay(5000);
        ft_clear_display(true);
        display.powerOff();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    });
    ArduinoOTA.onError([](ota_error_t error) {
        ft_display_cluster_number(OTA_FAIL);
        ft_delay(3000);
        ft_clear_display(true);
        ESP.restart();
    });
    ArduinoOTA.begin();
    ft_ota_waiting_loop();
}
 
