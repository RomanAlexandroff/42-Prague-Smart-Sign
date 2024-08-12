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

bool ft_data_restore(String* p_input, int16_t* checksum, const char* file_name)
{
    String  buffer;

    buffer = ft_read_spiffs_file(file_name);
    buffer.trim();
    *p_input = buffer;
    DEBUG_PRINTF("\n[FILE SYSTEM] Found uncorrupted data in the file %s. ", file_name);
    DEBUG_PRINTF("Сorresponding value has been restored.\n", "");
    *checksum = 0;
    ft_checksum(*p_input, checksum);
    if (!ft_checksum(*p_input, checksum))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] Unable to restore data from %s. ", file_name);
        if (String(file_name) == "/chat_id.txt")
        {
            DEBUG_PRINTF("\n[FILE SYSTEM] To restore chat_id value write /status command ", "");
            DEBUG_PRINTF("into the Telegram chat %s ", String(BOT_NAME));
            DEBUG_PRINTF("then restart the device", "");
            ft_display_cluster_number(TELEGRAM_ERROR);
        }
        if (String(file_name) == "/secret.txt")
        {
            DEBUG_PRINTF("\n[FILE SYSTEM] To restore Secret value write the Secret ", "");
            DEBUG_PRINTF("into the Telegram chat %s ", String(BOT_NAME));
            DEBUG_PRINTF("then restart the device or wait for its next wake-up.", "");
            DEBUG_PRINTF("\n[FILE SYSTEM] Contacting User %s for assistance...\n", rtc_g.from_name);
            ft_display_cluster_number(SECRET_EXPIRED);
            bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_CORRUPTED, 0), "");
        }
        return (false);
    }
    return (true);
}

int16_t ft_checksum(String input, int16_t* p_checksum)
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
    if (*p_checksum && *p_checksum == result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Test successfully passed.\n", "");
        return (1);
    }
    if (*p_checksum && *p_checksum != result)
    {
        DEBUG_PRINTF("\n[CHECKSUM] Data corruption detected!\n", "");
        return (0);
    }
    if (!*p_checksum)
    {
        DEBUG_PRINTF("\n[CHECKSUM] New checksum was successfully generated.\n", "");
        *p_checksum = result;
        return (0);
    }
    DEBUG_PRINTF("\n[CHECKSUM] Unknown error.\n", "");
    return (NOT_FOUND);
}

void  ft_data_integrity_check(void)
{
    if (!LittleFS.exists("/secret.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The secret.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/secret.txt", SECRET);
        DEBUG_PRINTF("\n[FILE SYSTEM] secret.txt file created. It contains:\n%s\n", ft_read_spiffs_file("/secret.txt").c_str());
    }
    if (!rtc_g.secret_checksum || !ft_checksum(rtc_g.Secret, &rtc_g.secret_checksum))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] Secret token is corrupted. Resolving...\n\n", "");
        ft_data_restore(&rtc_g.Secret, &rtc_g.secret_checksum, "/secret.txt");
        DEBUG_PRINTF("\n[FILE SYSTEM] The Secret variable value is now:\n%s\n", rtc_g.Secret.c_str());
    }
    if (!LittleFS.exists("/chat_id.txt"))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] The chat_id.txt file does not exist. Creating...\n", "");
        ft_write_spiffs_file("/chat_id.txt", "00000000000000000");
        DEBUG_PRINTF("\n[FILE SYSTEM] chat_id.txt file created. The rtc_g.chat_id value is recorded as %d\n", ft_read_spiffs_file("/chat_id.txt"));
    }
    if (!rtc_g.chat_id_checksum || !ft_checksum(rtc_g.chat_id, &rtc_g.chat_id_checksum))
    {
        DEBUG_PRINTF("\n[FILE SYSTEM] Chat_id is corrupted. Resolving...\n\n", "");
        ft_data_restore(&rtc_g.chat_id, &rtc_g.chat_id_checksum, "/chat_id.txt");
        DEBUG_PRINTF("\n[FILE SYSTEM] The rtc_g.chat_id variable has been set to \n%s\n", rtc_g.chat_id.c_str());
    }
}

short  ft_write_spiffs_file(const char* file_name, String input)
{
    short i;

    i = 1;
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
 
