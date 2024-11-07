/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ota.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 12:50:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/05/29 18:45:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/*                                                                            */
/*   This file contains inline functions declared in the main header. This    */
/*   file has to be included AFTER all the functions' declarations.           */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTA_H
# define OTA_H

void ft_ota_init(void)
{
    if (rtc_g.ota)
    {
        char    fullhostname[21] = "42_Prague_Smart_Sign";

        if (WiFi.status() != WL_CONNECTED)
            ft_wifi_connect();
        if (WiFi.status() != WL_CONNECTED)
        {
            DEBUG_PRINTF("[OTA] Failed to initialise OTA due to Wi-Fi connection issues\n", "");
            rtc_g.ota = false;
            return;
        }
        ArduinoOTA.setHostname(fullhostname);
        ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else // U_SPIFFS
                    type = "filesystem";
                ft_display_cluster_number(OTA_UPDATING);
                DEBUG_PRINTF("[OTA] Start updating %s", type.c_str());
                bot.sendMessage(String(rtc_g.chat_id), "Updating...", "");
            })
            .onEnd([]() {
                ft_display_cluster_number(OTA_SUCCESS);
                DEBUG_PRINTF("\n[OTA] End", "");
                rtc_g.ota = false;
                bot.sendMessage(String(rtc_g.chat_id), "Successfully updated!", "");
                ft_delay(5000);
                ft_clear_display();
                display.powerOff();
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                DEBUG_PRINTF("\n[OTA] Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                ft_display_cluster_number(OTA_FAIL);
                DEBUG_PRINTF("\n[OTA] Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) DEBUG_PRINTF("Auth Failed\n", "");
                else if (error == OTA_BEGIN_ERROR) DEBUG_PRINTF("Begin Failed\n", "");
                else if (error == OTA_CONNECT_ERROR) DEBUG_PRINTF("Connect Failed\n", "");
                else if (error == OTA_RECEIVE_ERROR) DEBUG_PRINTF("Receive Failed\n", "");
                else if (error == OTA_END_ERROR) DEBUG_PRINTF("End Failed\n", "");
                bot.sendMessage(String(rtc_g.chat_id), "Something went wrong. Updating was not completed. Try again later", "");
                ft_delay(3000);
                ft_clear_display();
                ESP.restart();
            });
        ft_display_cluster_number(OTA_WAITING);
        DEBUG_PRINTF("\n[OTA] Ready to update\n\n", "");
        bot.sendMessage(String(rtc_g.chat_id), "OTA Update is active", "");
        ArduinoOTA.begin();
    }
}

void ft_ota_waiting_loop(void)
{
    if (rtc_g.ota)
    {
        uint16_t ota_limit;

        ota_limit = 0;
        while (rtc_g.ota && ota_limit < OTA_WAIT_LIMIT)
        {
            ArduinoOTA.handle();
            DEBUG_PRINTF("\n[OTA] Active", "");
            ota_limit++;
            delay(1000);
        }
        rtc_g.ota = false;
        ft_display_cluster_number(OTA_CANCELED);
        bot.sendMessage(String(rtc_g.chat_id), "OTA Update port closed", "");
        DEBUG_PRINTF("\n[OTA] OTA Update port closed\n", "");
    }
}

#endif
 
