
#ifndef GLOBALS_H_
# define GLOBALS_H_

# include <GxEPD2_BW.h>
# include <GxEPD2_3C.h>
# include <GxEPD2_7C.h>
# include <Adafruit_GFX.h>
# include "credentials.h"
# include "constants.h"

extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display; // GDEY075Z08 800x480

struct RTC_GlobalsManagement {
    bool    eeprom_state;
    bool    warning_active;
    bool    ota_active;
    uint8_t hour;
    uint8_t minute;
    uint8_t day;
    uint8_t month;
};
extern struct RTC_GlobalsManagement rtc_g;

struct GlobalsManagement {
    uint8_t battery;
};
extern struct GlobalsManagement g;

#endif
 
