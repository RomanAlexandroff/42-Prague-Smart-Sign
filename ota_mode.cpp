
#include "42-Prague-Smart-Sign.h"

static void  ft_ota_waiting_loop(void)
{
    int i;

    i = 0;
    DEBUG_PRINTF("\nThe OTA Waiting Loop is running...\n", "");
    while (i < WAIT_FOR_OTA_LIMIT)
    {
        ArduinoOTA.handle();
        ft_delay(500);
        i++;
    }
    DEBUG_PRINTF("\nThe OTA Waiting Loop exited without an update: TIME OUT\n", "");
}

void  ft_ota_init(void)
{
    const char* ssid;
    uint16_t    maxlen;
    const char* nameprefix;
    char*       fullhostname;
    uint8_t     mac[6];

    ssid = WiFi.SSID().c_str();
    nameprefix = "42 Prague Smart Sign";
    maxlen = strlen(nameprefix) + 7;
    fullhostname = new char[maxlen];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else
            type = "filesystem";
        ft_display_bitmap(badge_bitmap_ota_updating);
    });
    ArduinoOTA.onEnd([]() {
        ft_display_bitmap(badge_bitmap_ota_success);
        ft_delay(5000);
        ft_clear_display(true);
        display.powerOff();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    });
    ArduinoOTA.onError([](ota_error_t error) {
        ft_display_bitmap(badge_bitmap_ota_fail);
        ft_delay(3000);
        ft_clear_display(true);
        ESP.restart();
    });
    ArduinoOTA.begin();
    ft_ota_waiting_loop();
}
 
