/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secret_management.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
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
        rtc_g.spiffs_state = true;
        return;
    }
    DEBUG_PRINTF("\nSPIFFS was not initialised. Reading and Writing data is unavailable this session.\n", "");
    rtc_g.spiffs_state = false;
}
 
