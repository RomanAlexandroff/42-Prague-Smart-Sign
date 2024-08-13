/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intra_interaction.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:50 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:52 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static void  ft_get_exam_time(String server_response)
{
    int i;

    i = 0;
    while (i != NOT_FOUND)
    {
        i = server_response.indexOf("\"begin_at\":\"");
        rtc_g.exam_start_hour = server_response.substring(i + 23, i + 25).toInt() + TIME_ZONE;
        rtc_g.exam_start_minutes = server_response.substring(i + 26, i + 28).toInt();
        i = server_response.indexOf("\"end_at\":\"");
        rtc_g.exam_end_hour = server_response.substring(i + 21, i + 23).toInt() + TIME_ZONE;
        rtc_g.exam_end_minutes = server_response.substring(i + 24, i + 26).toInt();
        if (rtc_g.daylight_flag)
        {
            rtc_g.exam_start_hour += 1;
            rtc_g.exam_end_hour += 1;
        }
        DEBUG_PRINTF("\n[INTRA] EXAM STATUS: Exam information detected\n", "");
        DEBUG_PRINTF("-- Begins at %d:", rtc_g.exam_start_hour);
        DEBUG_PRINTF("%d0\n", rtc_g.exam_start_minutes);
        DEBUG_PRINTF("-- Ends at %d:", rtc_g.exam_end_hour);
        DEBUG_PRINTF("%d0\n", rtc_g.exam_end_minutes);
        if (rtc_g.exam_end_hour <= rtc_g.hour)
            server_response = server_response.substring(i + 34); 
        else
        {
            DEBUG_PRINTF("\n[INTRA] EXAM STATUS: Active Exam found!\n\n", "");
            rtc_g.exam_status = true;
        }
        if (server_response.indexOf("\"begin_at\":\"") == NOT_FOUND)
            break;
    }
    DEBUG_PRINTF("\n[INTRA] EXAM STATUS: All the detected exams have already passed.\n\n", "");
    rtc_g.exam_status = false;
}

static bool  ft_handle_exams_info(void)
{
    String  server_response;

    server_response = client1.readString();
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================\n\n", "");
    DEBUG_PRINTF("%s", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    if (server_response.length() <= 0)
    {
        DEBUG_PRINTF("\n[INTRA] Error! Server response to the Exam Time request was not received\n\n", "");
        return (false);
    }
    if (server_response.indexOf("\"begin_at\":\"") == NOT_FOUND)
    {
        DEBUG_PRINTF("\n[INTRA] EXAM STATUS: As of now, there are no upcoming exams today\n\n", "");
        rtc_g.exam_status = false;
        return (true);
    }
    else
        ft_get_exam_time(server_response);
    return (true);
}

static void  ft_request_exams_info(const char* server, String* token)
{
    String  day;
    String  month;
    String  api_call;

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
    client1.println(*token);
    client1.println("Connection: close");
    client1.println();
}

static void ft_get_secret_expiration(String server_response)
{
    int      i;
    uint8_t  expire_day;
    uint8_t  expire_month;
    uint16_t expire_year;

    i = server_response.indexOf("\"secret_valid_until\":");
    if (i == NOT_FOUND)
        DEBUG_PRINTF("[INTRA] Secret expiration date was not found in the server response\n\n", "");
    else
    {
        rtc_g.secret_expiration = server_response.substring(i + 21, i + 31).toInt();
        DEBUG_PRINTF("[INTRA] The secret expires on %d (UNIX timestamp format)\n", rtc_g.secret_expiration);
        if (ft_unix_timestamp_decoder(&expire_day, &expire_month, &expire_year))
        {
            DEBUG_PRINTF("[INTRA] The secret expires on %d.", expire_day);
            DEBUG_PRINTF("%d.", expire_month);
            DEBUG_PRINTF("%d\n", expire_year);
            DEBUG_PRINTF("[INTRA] The secret days left: %d\n\n", ft_expiration_counter());
        }
    }
}

static String ft_get_token(String server_response)
{
    int     i;
    String  token;

    i = server_response.indexOf("{\"access_token\":\"");
    if (i == NOT_FOUND)
    {
        DEBUG_PRINTF("\n[INTRA] Error! Server response came without the Access Token\n\n", "");
        return ("NOT_FOUND");
    }
    token = server_response.substring(i + 17, i + 81);
    DEBUG_PRINTF("[INTRA] Access Token has been extracted:\n%s\n", token.c_str());
    return (token);
}

static bool  ft_handle_server_response(const char* server, String* token)
{
    String  server_response;

    server_response = client1.readString();
    if (server_response.length() <= 0)
    {
        DEBUG_PRINTF("\n[INTRA] Error! Server response to the Access Token request was not received\n\n", "");
        return (false);
    }
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================\n\n", "");
    DEBUG_PRINTF("%s", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    *token = ft_get_token(server_response);
    if (*token == "NOT_FOUND")
        return (false);
    ft_get_secret_expiration(server_response);
    return (true);
}

static void  ft_access_server(const char* server)
{
    String  auth_request;

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
}

static bool  ft_intra_connect(const char* server)
{
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("\n[INTRA] Unable to connect to Wi-Fi\n\n", "");
        return (false);
    }
    client1.setInsecure();
    client1.setTimeout(10);
    if (!client1.connect(server, 443))
    {
        DEBUG_PRINTF("\n[INTRA] Connection to the server failed\n\n", "");
        return (false);
    }
    return (true);
}

bool  ft_fetch_exams(void)
{
    const char* server PROGMEM = "api.intra.42.fr";
    String      token;

    if (!ft_intra_connect(server))
        return (false);
    ft_access_server(server);
    if (!ft_handle_server_response(server, &token))
        return (false);
    ft_request_exams_info(server, &token);
    if (!ft_handle_exams_info())
        return (false);
    client1.stop();
    return (true);
}
 
