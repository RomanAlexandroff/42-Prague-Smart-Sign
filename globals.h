
#ifndef GLOBALS_H_
# define GLOBALS_H_

# include "constants.h"
# include <GxEPD2_BW.h>                             // we still need the black/white library version
# include <GxEPD2_3C.h>                             // 3-coloured version of the GxEPD2 library
# include <Fonts/FreeSansBold24pt7b.h>              // the fonts come from the Adafruit GFX library which gets called by the GxEPD2 library
# include <WiFiClientSecure.h>                      // has to be here. For Wi-Fi connection and HTTPS requests
# include "credentials.h"

extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT/2> display; // GDEY075Z08 800x480
extern WiFiClientSecure client;

struct RTC_GlobalsManagement {
    bool     eeprom_state;
    bool     warning_active;
    bool     ota_active;
    bool     exam_state;
    uint8_t  exams_number;
    uint8_t  exam_start_hour;
    uint8_t  exam_start_minutes;
    uint8_t  exam_end_hour;
    uint8_t  exam_end_minutes;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  day;
    uint8_t  month;
    uint16_t year;
};
extern struct RTC_GlobalsManagement rtc_g;

struct GlobalsManagement {
    uint16_t battery;
};
extern struct GlobalsManagement g;

#endif
 
