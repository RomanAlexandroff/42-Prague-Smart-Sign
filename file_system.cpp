/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_system.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:02:40 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:02:46 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

bool ft_secret_verification(String input)
{
    if (input.length() != 73)
        return (false);
    if (input.substring(0, 1) != "s")
        return (false);
    return (true);
}

void ft_data_restore(String* p_input, const char* file_name)
{
    String  buffer;

    if (p_input == NULL)
        return;
    if (!file_name)
        return;
    buffer = ft_read_spiffs_file(file_name);
    buffer.trim();
    *p_input = buffer;
    DEBUG_PRINTF("\n[FILE SYSTEM] Found uncorrupted data in the file %s. ", file_name);
    DEBUG_PRINTF("Сorresponding value has been restored.\n", "");
}

void  ft_data_integrity_check(void)
{
    if (!LittleFS.exists("/secret.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The secret.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/secret.txt", SECRET);
        DEBUG_PRINTF("\n[FILE SYSTEM] secret.txt file created. It contains:\n%s\n", ft_read_spiffs_file("/secret.txt").c_str());
    }
    if (!LittleFS.exists("/chat_id.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The chat_id.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/chat_id.txt", "0000000000000");
        DEBUG_PRINTF("\n[FILE SYSTEM] chat_id.txt file created. The rtc_g.chat_id value is recorded as %d\n", ft_read_spiffs_file("/chat_id.txt"));
        DEBUG_PRINTF("\n[FILE SYSTEM] To set up the real chat_id value simply write /status ", "");
        DEBUG_PRINTF("into the Telegram chat %s, ", String(BOT_NAME));
        DEBUG_PRINTF("then restart the device or wait for its next wake-up.", "");
        ft_display_cluster_number(TELEGRAM_ERROR);
    }
    ft_data_restore(&rtc_g.Secret, "/secret.txt");
    DEBUG_PRINTF("\n[FILE SYSTEM] The Secret variable value is now:\n%s\n", rtc_g.Secret.c_str());
    ft_data_restore(&rtc_g.chat_id, "/chat_id.txt");
    DEBUG_PRINTF("\n[FILE SYSTEM] The rtc_g.chat_id variable has been set to \n%s\n", rtc_g.chat_id.c_str());
}

short  ft_write_spiffs_file(const char* file_name, String input)
{
    short i;

    i = 1;
    if (!file_name)
        return (0);
    if (input.isEmpty())
        return (0);
    File file = LittleFS.open(file_name, "w");
    while (!file && i <= 5)
    {
        DEBUG_PRINTF("[FILE SYSTEM] An error occurred while opening %s file for writing in LittleFS. Retry.\n", file_name);
        file = LittleFS.open(file_name, "w");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTF("[FILE SYSTEM] Failed to open %s file for writing in LittleFS. Its dependant function will be unavailable during this programm cycle.\n", file_name);
        return (0);
    }
    else
    {
        file.println(input);
        file.close();
    }
    return (1);
}

String  ft_read_spiffs_file(const char* file_name)
{
    short   i;
    String  output;

    i = 1;
    if (!file_name)
        return ("0");
    File file = LittleFS.open(file_name, "r");
    while (!file && i <= 5)
    {
        DEBUG_PRINTF("[FILE SYSTEM] An error occurred while opening %s file for reading in LittleFS. Retry.\n", file_name);
        file = LittleFS.open(file_name, "r");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTF("[FILE SYSTEM] Failed to open %s file for reading in LittleFS. Its dependant function will be unavailable during this programm cycle.\n", file_name);
        file.close();
    }  
    else
    {
        output = file.readStringUntil('\n');
        file.close(); 
    }
    return (output);
}

void  ft_spiffs_init(void)
{
    short i;

    i = 3;
    if (!LittleFS.begin(true) && i)
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] Failed to initialise SPIFFS. Retrying...\n", "");
        ft_delay(1000);
        i--;
    }
    else
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] SPIFFS is successfully initialised.\n", "");
        return;
    }
    DEBUG_PRINTF("\n[FILE SYSTEM] SPIFFS was not initialised. Reading and Writing data is unavailable this session.\n", "");
}
 
