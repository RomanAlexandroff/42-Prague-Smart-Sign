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

/*
*   Checks only within SUBS_CHECK_TIME_LIMIT before the exam.
*/
static void  ft_check_exam_subscribers(String &server_response)
{
    int i;
    int subscribers;

    i = 0;
    subscribers = 0;
    if (ft_time_till_event(rtc_g.exam_start_hour, rtc_g.exam_start_minutes) > SUBS_CHECK_TIME_LIMIT)
        return;
    i = server_response.indexOf("\"nbr_subscribers\":\"");
    if (i == NOT_FOUND)
        return;
    subscribers = server_response.substring(i + 18, i + 19).toInt();
    if (subscribers == 0)
    {
        DEBUG_PRINTF("\n[INTRA] Noone has subscribed to this exam. Exam mode canceled.\n\n", "");
        rtc_g.exam_status = false;
    }
    else
        DEBUG_PRINTF("\n[INTRA] Subscribers detected. Continuing with the Exam mode.\n\n", "");
}

static void  ft_get_exam_time(String &server_response)
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
            server_response = server_response.substring(i + 87);              // remove data of a past exam
        else
        {
            DEBUG_PRINTF("\n[INTRA] EXAM STATUS: Active Exam found!\n\n", "");
            rtc_g.exam_status = true;
            return;
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

    server_response = Intra_client.readString();
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
    {
        ft_get_exam_time(server_response);
        ft_check_exam_subscribers(server_response);
    }
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
    Intra_client.print("GET ");
    Intra_client.print(api_call);
    Intra_client.println(" HTTP/1.1");
    Intra_client.print("Host: ");
    Intra_client.println(server);
    Intra_client.print("Authorization: Bearer ");
    Intra_client.println(*token);
    Intra_client.println("Connection: close");
    Intra_client.println();
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

    server_response = Intra_client.readString();
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
    Intra_client.print("POST https://api.intra.42.fr/oauth/token HTTP/1.1\r\n");
    Intra_client.print("Host: ");
    Intra_client.println(server);
    Intra_client.println("Content-Type: application/x-www-form-urlencoded");
    Intra_client.print("Content-Length: ");
    Intra_client.println(auth_request.length());
    Intra_client.println();
    Intra_client.println(auth_request);
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
    Intra_client.setInsecure();
    Intra_client.setTimeout(10);
    if (!Intra_client.connect(server, 443))
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
    {
        Intra_client.stop();
        return (false);
    }
    ft_request_exams_info(server, &token);
    if (!ft_handle_exams_info())
    {
        Intra_client.stop();
        return (false);
    }
    Intra_client.stop();
    return (true);
}
 
