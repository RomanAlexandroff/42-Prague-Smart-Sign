
#include "42-Prague-Smart-Sign.h"

void  ft_fetch_exams(void)
{
    String        data;
    String        line;
    const String  token;

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
            token = line.substring(17, line.length() - 3);
            DEBUG_PRINTF("Access Token: " + token);
            break;
        }
    }
    client.print(String("GET /v2/campus/56/exams HTTP/1.1\r\n" +
               "Host: api.intra.42.fr\r\n" +
               "Authorization: Bearer " + token + "\r\n" +
               "Connection: close\r\n\r\n"));
    DEBUG_PRINTF("Request for Exam Data sent\n", "");
    client.stop();
}
 
