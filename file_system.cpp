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

bool ft_data_restore(String* input, int16_t checksum)
{
    String  buffer;

    buffer = ft_read_spiffs_file("/secret.txt");
    buffer.trim();
    if (!ft_checksum(buffer, &checksum))
    {
        DEBUG_PRINTF("\nUnable to restore the Secret token. ", "");
        DEBUG_PRINTF("Contacting User %s for assistance...\n", rtc_g.from_name);
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_CORRUPTED, 0), "");
        return (false);
    }
    else
    {
        *input = buffer;
        DEBUG_PRINTF("\nFound uncorrupted Secret token in the back up. ", "");
        DEBUG_PRINTF("The Secret has been restored.\n", "");
    }
    return (true);
}

int16_t ft_checksum(String input, int16_t* checksum)
{
    const char *str;
    uint8_t    i;
    int16_t    result;

    i = 0;
    result = 0;
    str = input.c_str();
    if (!str)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Test canceled. No input was provided.\n", "");
        return (NOT_FOUND);
    }
    while (str[i])
    {
        result += str[i];
        i++;
    }
    if (*checksum && *checksum == result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Test successfully passed.\n", "");
        return (1);
    }
    if (*checksum && *checksum != result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Data corruption detected!\n", "");
        return (0);
    }
    if (!*checksum)
    {
        DEBUG_PRINTF("\n[CHECKSUM] New checksum was successfully generated.\n", "");
        *checksum = result;
        return (result);
    }
    DEBUG_PRINTF("\n[CHECKSUM] Unknown error.\n", "");
    return (NOT_FOUND);
}

short  ft_write_spiffs_file(const char* file_name, String input)
{
    short i;

    i = 1;
    File file = LittleFS.open(file_name, "w");
    while (!file && i <= 5)
    {
        DEBUG_PRINTF("An error occurred while opening %s file for writing in LittleFS. Retry.\n", file_name);
        file = LittleFS.open(file_name, "w");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTF("Failed to open %s file for writing in LittleFS. Its dependant function will be unavailable during this programm cycle.\n", file_name);
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
    File file = LittleFS.open(file_name, "r");
    while (!file && i <= 5)
    {
        DEBUG_PRINTF("An error occurred while opening %s file for reading in LittleFS. Retry.\n", file_name);
        file = LittleFS.open(file_name, "r");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTF("Failed to open %s file for reading in LittleFS. Its dependant function will be unavailable during this programm cycle.\n", file_name);
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
        DEBUG_PRINTF("\nFailed to initialise SPIFFS. Retrying...\n", "");
        ft_delay(1000);
        i--;
    }
    else
    {
        DEBUG_PRINTF("\nSPIFFS is successfully initialised.\n", "");
        return;
    }
    DEBUG_PRINTF("\nSPIFFS was not initialised. Reading and Writing data is unavailable this session.\n", "");
}
 
