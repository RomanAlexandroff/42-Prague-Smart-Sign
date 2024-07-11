/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intra_interaction.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:50 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:52 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

bool  ft_fetch_exams(void)
{
    const char* server PROGMEM = "api.intra.42.fr";
    String      auth_request;
    String      server_response;
    int         i;
    String      token;
    uint8_t     expire_day;
    uint8_t     expire_month;
    uint16_t    expire_year;
    int8_t      days_left;
    String      day;
    String      month;
    String      api_call;

// WI-FI CONNECTION CHECK
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
        return (false);

// CONNECTING TO THE INTRA SERVER
    WiFiClientSecure client1;
    client1.setInsecure();
    client1.setTimeout(10);
    if (!client1.connect(server, 443))
    {
        DEBUG_PRINTF("\nConnection to the Intra server failed\n\n", "");
        return (false);
    }

// REQUESTING A TEMPORARY ACCESS TOKEN FROM THE SERVER
    auth_request = "grant_type=client_credentials&client_id=";
    auth_request += UID;
    auth_request += "&client_secret=";
    auth_request += rtc_g.Secret;

    client1.print("POST https://api.intra.42.fr/oauth/token HTTP/1.1\r\n");
    client1.print("Host: ");
    client1.println(server);
    client1.println("Content-Type: application/x-www-form-urlencoded");
    client1.print("Content-Length: ");
    client1.println(auth_request.length());
    client1.println();
    client1.println(auth_request);
    auth_request.clear();

// READING THE SERVER RESPONSE. EXTRACTING ACCESS TOKEN AND SECRET EXPIRATION DATE
    server_response = client1.readString();
    if (server_response.length() <= 0)
    {
        DEBUG_PRINTF("\nError! Server response to the Access Token request was not received\n\n", "");
        return (false);
    }
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================\n\n", "");
    DEBUG_PRINTF("%s\n", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    i = server_response.indexOf("{\"access_token\":\"");
    if (i == -1)
    {
        DEBUG_PRINTF("\nError! Server response came without the Access Token\n\n", "");
        if (!ft_checksum(rtc_g.Secret, rtc_g.secret_checksum))
        {
            DEBUG_PRINTF("The error occured due to corrupted Secret token. Resolving...\n\n", "");
            ft_data_restore(rtc_g.Secret, rtc_g.secret_checksum);
        }
        else
            DEBUG_PRINTF("The error occured due to unknown reason.\n\n", "");
        return (false);
    }
    token = server_response.substring(i + 17, i + 81);
    DEBUG_PRINTF("Access Token has been extracted:\n%s\n", token.c_str());
    i = server_response.indexOf("\"secret_valid_until\":");
    if (i == -1)
        DEBUG_PRINTF("Secret expiration date was not found in the server response\n\n", "");
    else
    {
        rtc_g.secret_expiration = server_response.substring(i + 21, i + 31).toInt();
        DEBUG_PRINTF("The secret expires on %d (UNIX timestamp format)\n", rtc_g.secret_expiration);
        if (ft_unix_timestamp_decoder(&expire_day, &expire_month, &expire_year))
        {
            DEBUG_PRINTF("The secret expires on %d.", expire_day);
            DEBUG_PRINTF("%d.", expire_month);
            DEBUG_PRINTF("%d\n", expire_year);
            days_left = ft_expiration_counter();
            DEBUG_PRINTF("The secret days left: %d\n\n", days_left);
        }
    }
    server_response.clear();

// REQUESTING THE EXAMS INFORMATION FROM THE SERVER 
    if (rtc_g.month < 10)
        month = "0" + String(rtc_g.month);
    else
        month = String(rtc_g.month);
    if (rtc_g.day < 10)
        day = "0" + String(rtc_g.day);
    else
        day = String(rtc_g.day);
    api_call = "https://api.intra.42.fr/v2/campus/";
    api_call += CAMPUS_ID;
    api_call += "/exams?filter[location]=";
    api_call += CLUSTER_ID;
    api_call += "&range[begin_at]=";
    api_call += String(rtc_g.year) + "-" + month + "-" + day + "T05:00:00.000Z,";
    api_call += String(rtc_g.year) + "-" + month + "-" + day + "T21:00:00.000Z";
    
    client1.print("GET ");
    client1.print(api_call);
    client1.println(" HTTP/1.1");
    client1.print("Host: ");
    client1.println(server);
    client1.print("Authorization: Bearer ");
    client1.println(token);
    client1.println("Connection: close");
    client1.println();

// READING THE SERVER RESPONSE. EXTRACTING & EVALUATING THE EXAMS INFORMATION
    server_response = client1.readString();
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================  \n", "");
    DEBUG_PRINTF("%s\n", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    if (server_response.length() <= 0)
    {
        DEBUG_PRINTF("\nError! Server response to the Exam Time request was not received\n\n", "");
        return (false);
    }
    if (server_response.indexOf("\"begin_at\":\"") == -1)
    {
        DEBUG_PRINTF("\nEXAMS STATUS: As of now, there are no upcoming exams today\n\n", "");
        rtc_g.exam_status = false;
        return (true);
    }
    else
    {
        i = 0;
        while (i != -1)
        {
            i = server_response.indexOf("\"begin_at\":\"");
            rtc_g.exam_start_hour = server_response.substring(i + 23, i + 25).toInt() + TIME_ZONE;
            rtc_g.exam_start_minutes = server_response.substring(i + 26, i + 28).toInt();
            i = server_response.indexOf("\"end_at\":\"");
            rtc_g.exam_end_hour = server_response.substring(i + 21, i + 23).toInt() + TIME_ZONE;
            rtc_g.exam_end_minutes = server_response.substring(i + 24, i + 26).toInt();
            DEBUG_PRINTF("\nEXAMS STATUS: Exam information detected\n", "");
            DEBUG_PRINTF("-- Begins at %d:", rtc_g.exam_start_hour);
            DEBUG_PRINTF("%d0\n", rtc_g.exam_start_minutes);
            DEBUG_PRINTF("-- Ends at %d:", rtc_g.exam_end_hour);
            DEBUG_PRINTF("%d0\n", rtc_g.exam_end_minutes);
            if (rtc_g.exam_end_hour <= rtc_g.hour)
                server_response = server_response.substring(i + 34); 
            else
            {
                DEBUG_PRINTF("\nEXAMS STATUS: Active Exam found!\n\n", "");
                rtc_g.exam_status = true;
                return (true);
            }
            if (server_response.indexOf("\"begin_at\":\"") == -1)
                break;
        }
        DEBUG_PRINTF("\nEXAMS STATUS: All the detected exams have already passed.\n\n", "");
        rtc_g.exam_status = false;
    }
    client1.stop();
    return (true);
}
 
