
#ifndef GLOBALS_H_
# define GLOBALS_H_

# include <GxEPD2_BW.h>
# include <GxEPD2_3C.h>
# include <GxEPD2_7C.h>
# include <Adafruit_GFX.h>
# include "credentials.h"
# include "constants.h"

extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display; // GDEY075Z08 800x480

extern RTC_DATA_ATTR bool    g_eeprom_state;
extern RTC_DATA_ATTR uint8_t g_hour;
extern RTC_DATA_ATTR uint8_t g_minute;
extern RTC_DATA_ATTR uint8_t g_day;
extern RTC_DATA_ATTR uint8_t g_month;

struct GlobalsManagement {
    uint8_t battery;
};
extern struct GlobalsManagement g;

#endif
 
