
#include "globals.h"

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN)); // GDEY075Z08 800x480

RTC_DATA_ATTR bool    g_eeprom_state;
RTC_DATA_ATTR uint8_t g_hour;
RTC_DATA_ATTR uint8_t g_minute;
RTC_DATA_ATTR uint8_t g_day;
RTC_DATA_ATTR uint8_t g_month;

struct GlobalsManagement g = {
    .battery = 0
};
 
