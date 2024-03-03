
#include "42-Prague-Smart-Sign.h"

static void  ft_clean_data(String line, String* p_exam_begin, String* p_exam_end)
{
    while (client.available())
    {
        line = client.readStringUntil('\n');
        if (line.startsWith("\"begin_at\":\"")) 
        {
            *p_exam_begin = line.substring(24, 28);
            DEBUG_PRINTF("Exam starts at: " + *p_exam_begin);
        }
                if (line.startsWith("\"end_at\":\"")) 
        {
            *p_exam_end = line.substring(24, 28);
            DEBUG_PRINTF("Exam ends at: " + *p_exam_end);
            break;
        }
    }
}

static void  ft_get_data_load(const String token, String* p_line)
{
    String  query;

    query = "/exams?filter[location]=" + CLUSTER_ID + "&range[begin_at]=" + 
            rtc_g.year + "-" + rtc_g.month + "-" + rtc_g.day + "T05:00:00.000Z," +
            rtc_g.year + "-" + rtc_g.month + "-" + rtc_g.day + "T22:00:00.000Z";
    client.print(String("GET /v2/campus/" + CAMPUS_ID + query + " HTTP/1.1\r\n" +
               "Host: api.intra.42.fr\r\n" +
               "Authorization: Bearer " + token + "\r\n" +
               "Connection: close\r\n\r\n"));
    DEBUG_PRINTF("Request for Exam Data sent\n", "");
    while (client.connected())
    {
        *p_line = client.readStringUntil('\n');
        if (*p_line == "\r")
        {
            DEBUG_PRINTF("Headers received\n", "");
            break;
        }
    }
}

static void  ft_get_token(const String* p_token)
{
    String  data;
    String  line;

    data = "grant_type=client_credentials&client_id=" + UID + "&client_secret=" + SECRET;
    client.print(String("POST /oauth/token HTTP/1.1\r\n" +
               "Host: api.intra.42.fr\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + data.length() + "\r\n" +
               "Connection: close\r\n\r\n" + data));
    DEBUG_PRINTF("Request for an Access Token sent\n", "");
    while (client.connected())
    {
        line = client.readStringUntil('\n');
        if (line == "\r")
        {
            DEBUG_PRINTF("Headers received\n", "");
            break;
        }
    }
    while (client.available())
    {
        line = client.readStringUntil('\n');
        if (line.startsWith("{\"access_token\":\"")) 
        {
            *p_token = line.substring(17, 80);
            DEBUG_PRINTF("Access Token: " + *p_token);
            break;
        }
    }
}

void  ft_fetch_exams(void)
{
    String        line;
    const String  token;
    String        exam_begin;
    String        exam_end;

    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("Failed to obtain Intra data due to Wi-Fi connection issues\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    WiFiClientSecure client;
    if (!client.connect("https://api.intra.42.fr", 443))
    {
        DEBUG_PRINTF("Intra server connection FAILED\n", "");
        return;
    }
    ft_get_token(&token);
    ft_get_data_load(token, &line);
    ft_clean_data(line, &exam_begin, &exam_end);
    client.stop();
}
 
