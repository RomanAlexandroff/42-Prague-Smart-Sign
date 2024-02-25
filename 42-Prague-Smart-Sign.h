
#ifndef 42-PRAGUE-SMART-SIGN_H
# define 42-PRAGUE-SMART-SIGN_H

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include "bitmap_library.h"
#include "credentials.h"
#include "constants.h"

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN)); // GDEY075Z08 800x480


#endif
 
