
#include "globals.h"

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT/2> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN)); // GDEY075Z08 800x480

RTC_DATA_ATTR struct RTC_GlobalsManagement rtc_g;

struct GlobalsManagement g = {
    .battery = 0
};
 
