
#include "42-Prague-Smart-Sign.h"

void  ft_fetch_data(void)
{
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("Failed to obtain Intra data due to Wi-Fi connection issues\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    if (!client.connect("https://api.intra.42.fr", 442))
    {
        DEBUG_PRINTF("Intra server connection FAILED", "");
        return;
    }

   
}

 
