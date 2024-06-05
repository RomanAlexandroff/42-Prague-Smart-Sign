/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   telegram_bot.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:56 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:57 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

String  ft_compose_message(uint8_t subject, int8_t days_left)
{
    String  output;

    output = "Dear " + rtc_g.from_name;
    output += ", I need your assistance! ";
    if (subject == SECRET_EXPIRED)
    {
        output += "My SECRET token ";
        if (days_left > 1)
            output += "expires in " + String(days_left) + " days. ";
        if (days_left == 1)
            output += "expires tomorrow! ";
        if (days_left <= 0)
            output += "has already expired!!! ";
        output += "And without it I would not be able to do my job. ";
        output += "Please, retreive a new SECRET token from the Intra API page ";
        output += "and send it to me in this chat. ";
        output += "Your message should start with a forward slash (/), ";
        output += "so I could recognise it.\n\n";
        output += "Here is an example:\n";
        output += "/s-xxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
        output += "After it is done, I will leave you in pease for another month =)";
    }
    if (subject == SECRET_CORRUPTED)
    {
        output += "I detected that the SECRET token in my memory has been corrupted. ";
        output += "And now I really need you to send it to me again. ";
        output += "You do not have to generate a new one if the old one is still good. ";
        output += "Just don't forget to start the message with a forward slash (/). ";
        output += "Sorry about this! I very appreciate your help!";
    }
    if (subject == LOW_BATTERY)
    {
        output += "My battery is low. I am currently sitting on 3%";
        output += " and it keeps on draining. Please, charge me as soon as possible.";
    }
    if (subject == DEAD_BATTERY)
    {
        output += "My battery is dead, so I am stopping all the processes and turning off. ";
        output += "Please, charge me and I will turn back on again.";
    }
    return (output);
}

static void ft_reply_machine(String text)
{
    String      message;
    uint8_t     day;
    uint8_t     month;
    uint16_t    year;

    if (text == "/status")
    {
        message = "Connected to " + String(WiFi.SSID());   
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
        bot.sendMessage(rtc_g.chat_id, message, "");
        message.clear();
    }
    else if (text == "/ota")
    {
        if (rtc_g.ota == false)
        {
            ft_write_spiffs_file("/ota.txt", ACTIVE);
            ESP.restart();
        }
        else
        {
            rtc_g.ota = false;
            ft_write_spiffs_file("/ota.txt", CLOSED);
        }
        return;
    }
    else
    {
        text.remove(0, 1);
        if (ft_secret_verification(text))
        {
            rtc_g.Secret = text;
            ft_write_spiffs_file("/secret.txt", text);
            rtc_g.secret_checksum = ft_checksum(rtc_g.Secret, 0);
            message = "Accepted!\nThe SECRET token has been renewed.\n\n";
            message += "Current token now is:\n" + rtc_g.Secret;
            bot.sendMessage(rtc_g.chat_id, message, "");
        }
        else
        {
            message = "I am sorry, but I do not understand \"";
            message += text + "\"\n\n";
            message = "You may try to use \"/status\" command";
            bot.sendMessage(rtc_g.chat_id, message, "");
        }
    }
}

static void  ft_new_messages(short message_count)
{
    uint8_t i;
    String  text;

    i = 0;
    DEBUG_PRINTF("\nHandling new Telegram messages\n", "");
    DEBUG_PRINTF("Number of messages to handle: %d\n", message_count);
    while (i < message_count) 
    {
        DEBUG_PRINTF("Handling loop iterations: i = %d\n", i);
        rtc_g.chat_id = String(bot.messages[i].chat_id);
        ft_write_spiffs_file("/chat_id.txt", rtc_g.chat_id);
        text = bot.messages[i].text;
        rtc_g.from_name = bot.messages[i].from_name;
        DEBUG_PRINTF("%s says: ", rtc_g.from_name.c_str());
        DEBUG_PRINTF("%s\n\n", text.c_str());
        ft_reply_machine(text);
        i++;
    }
}

void  ft_telegram_check(void)
{
    short message_count;

    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    else
    {     
        message_count = bot.getUpdates(bot.last_message_received + 1);
        while (message_count)
        {
            ft_new_messages(message_count);
            message_count = bot.getUpdates(bot.last_message_received + 1);
        }
    }
}
 
