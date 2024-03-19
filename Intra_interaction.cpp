
#include "42-Prague-Smart-Sign.h"

bool  ft_fetch_exams(void)
{
    const char* server PROGMEM = "api.intra.42.fr";
    String      auth_request;
    String      server_response;
    int         i;
    String      token;
    String      month;
    String      day;
    String      api_call;

    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("\nWiFi connection lost. Reconnecting", "");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            DEBUG_PRINTF(".", "");
        }
        DEBUG_PRINTF("\nConnected to WiFi.\n", "");
    }
    WiFiClientSecure client;
    client.setInsecure();
    if (!client.connect(server, 443))
    {
        DEBUG_PRINTF("\nConnection to the Intra server failed\n", "");
        client.stop();
        return (false);
    }
    auth_request = "grant_type=client_credentials&client_id=";
    auth_request += UID;
    auth_request += "&client_secret=";
    auth_request += SECRET;
    client.print("POST https://api.intra.42.fr/oauth/token HTTP/1.1\r\n");
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(auth_request.length());
    client.println();
    client.println(auth_request);
    auth_request.clear();
    server_response = client.readStringUntil('}');
    if (server_response.length() <= 0)
    {
        DEBUG_PRINTF("\nError! Server responce to the Access Token request was not received\n", "");
        client.stop();
        return (false);
    }
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================\n\n", "");
    DEBUG_PRINTF("%s\n", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    i = server_response.indexOf("{\"access_token\":\"");
    if (i == -1)
    {
        DEBUG_PRINTF("\nError! Server responce came without the Access Token\n", "");
        client.stop();
        return (false);
    }
    token = server_response.substring(i + 17, i + 81);
    DEBUG_PRINTF("Access Token has been excracted:\n%s\n\n", token.c_str());
    server_response.clear();
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
    
    client.print("GET ");
    client.print(api_call);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.print("Authorization: Bearer ");
    client.println(token);
    client.println("Connection: close");
    client.println();
    
    server_response = client.readStringUntil(']');                                                        // забираем в память целиком всё сообщение сервера
    DEBUG_PRINTF("\n============================== SERVER RESPONSE BEGIN ==============================\n\n", "");
    DEBUG_PRINTF("%s\n", server_response.c_str());
    DEBUG_PRINTF("\n=============================== SERVER RESPONSE END ===============================\n\n", "");
    if (server_response.length() <= 900)                                                                  // если сообщение пустое, то произошла ошибка
    {
        DEBUG_PRINTF("\nError! Server responce to the Exam Time request was not received\n", "");
        client.stop();
        return (false);
    }
    if (server_response.indexOf("\"begin_at\":\"") == -1)                                                 // если в сообщении нет упоминания о начале экзамена, то и экзамена сегодня нет
    {
        DEBUG_PRINTF("\nEXAMS STATUS: As of now, there are no upcoming exams today\n", "");
        rtc_g.exam_state = false;
        client.stop();
        return (true);
    }
    else                                                                                                  // упоминание экзамена нашлось - значит экзамен сегодня будет или уже был
    {
        i = 0;
        while (i != -1)                                                                                   // check the server response until there are no instances of "begin_at"
        {
            i = server_response.indexOf("\"begin_at\":\"");                                               // find the 1st instance of "begin_at" in the the server response string
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
            if (rtc_g.exam_start_hour < rtc_g.hour)                                                       // checking if this information is about past exam or future exam today
                server_response = server_response.substring(i + 34);                                      // if about the past exam then delete this information from the string and check next instance of "begin_at" 
            else
            {
                DEBUG_PRINTF("\nEXAMS STATUS: Active Exam found!\n", "");
                rtc_g.exam_state = true;
                client.stop();
                return (true);
            }
            if (server_response.indexOf("\"begin_at\":\"") == -1)                                         // если в оставшейся части сообщения нет начала экзамена, то экзаменов больше нет
                break;
        }
        DEBUG_PRINTF("\nEXAMS STATUS: All the detected exams have already passed.\n\n", "");
        rtc_g.exam_state = false;                                                                         // all the instances of "begin_at" in the server response were about past exams, so no more exams today
    }
    client.stop();
    return (true);
}
 
