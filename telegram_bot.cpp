/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   telegram_bot.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:56 by raleksan          #+#    #+#             */
/*   Updated: 2024/07/03 13:20:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/*                                                                            */
/*   These functions are for checking new Telegram messages, reading them     */
/*   and reacting to them.                                                    */
/*   WARNING! DO NOT CALL ft_go_to_sleep(), ft_delay() or ESP.restart()       */
/*   FROM THESE FUNCTIONS! THE DEVICE WILL BECOME UNRESPONSIVE TO ANY         */
/*   MESSAGES FROM THE TELEGRAM CHAT! YOU MAY DO IT ONLY WHEN YOU ARE ABOUT   */
/*   TO EXIT FROM ft_telegram_check().                                        */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void ft_reply_machine(String text)
{
    String      message;
    uint8_t     day;
    uint8_t     month;
    uint16_t    year;

    if (text == "/status")
    {
        DEBUG_PRINTF("\n\n[TELEGRAM MESSAGING] EXAM STATUS\n", "");
        DEBUG_PRINTF("-- Active or not: %d;\n", rtc_g.exam_status);
        DEBUG_PRINTF("-- Begins at %d:", rtc_g.exam_start_hour);
        DEBUG_PRINTF("%d0\n\n", rtc_g.exam_start_minutes);
        message = "Dear " + String(rtc_g.from_name);
        message += ", I am ";
        message += "Connected to " + String(WiFi.SSID());   
        message += ", Signal strength is " + String(WiFi.RSSI()) + " dBm, ";
        message += "Software version " + String(SOFTWARE_VERSION);
        message += ", Exams status: ";
        if (rtc_g.exam_status)
        {
            message += "next exam is today at " + String(rtc_g.exam_start_hour) + ":";
            if (rtc_g.exam_start_minutes < 10)
                message += "0";
            message += String(rtc_g.exam_start_minutes);
        }
        else
            message += "no exams are planned for today";
        if (ft_unix_timestamp_decoder(&day, &month, &year))
            message += ", Secret token expires on " + String(day) + "." + String(month) + "." + String(year);
        bot.sendMessage(String(rtc_g.chat_id), message, "");
        message.clear();
    }
    else if (text == "/ota")
    {
        rtc_g.ota = true;
        return;
    }
    else
    {
        text.remove(0, 1);
        if (ft_secret_verification(text))
        {
            text.toCharArray(rtc_g.Secret, sizeof(rtc_g.Secret));
            ft_write_spiffs_file("/secret.txt", rtc_g.Secret);
            message = "Accepted!\nThe SECRET token has been renewed.\n\n";
            message += "Current token now is:\n" + String(rtc_g.Secret);
            bot.sendMessage(String(rtc_g.chat_id), message, "");
        }
        else
        {
            message = "I am sorry, but I do not understand \"";
            message += text + "\".\n";
            message += "You may try to use \"/status\" command";
            bot.sendMessage(String(rtc_g.chat_id), message, "");
        }
    }
}

static void  ft_new_messages(short message_count)
{
    uint8_t i;
    String  text;
    String  id_buffer;
    String  name_buffer;

    i = 0;
    DEBUG_PRINTF("\n[TELEGRAM BOT] Handling new Telegram messages\n", "");
    DEBUG_PRINTF("[TELEGRAM BOT] Number of messages to handle: %d\n", message_count);
    while (i < message_count) 
    {
        DEBUG_PRINTF("[TELEGRAM BOT] Handling loop iterations: i = %d\n", i);
        id_buffer = bot.messages[i].chat_id;
        id_buffer.toCharArray(rtc_g.chat_id, sizeof(rtc_g.chat_id));
        ft_write_spiffs_file("/chat_id.txt", rtc_g.chat_id);
        text = bot.messages[i].text;
        name_buffer = bot.messages[i].from_name;
        name_buffer.toCharArray(rtc_g.from_name, sizeof(rtc_g.from_name));
        DEBUG_PRINTF("[TELEGRAM BOT] %s says: ", rtc_g.from_name);
        DEBUG_PRINTF("%s\n\n", text.c_str());
        ft_reply_machine(text);
        i++;
    }
}

void  ft_telegram_check(void)
{
    short message_count;

    if (WiFi.status() != WL_CONNECTED)
        ft_wifi_connect();  
    message_count = bot.getUpdates(bot.last_message_received + 1);
    while (message_count)
    {
        ft_new_messages(message_count);
        message_count = bot.getUpdates(bot.last_message_received + 1);
    }
}
 
