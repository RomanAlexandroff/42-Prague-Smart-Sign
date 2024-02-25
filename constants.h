
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SOFTWARE_VERSION        0.01
#define DEBUG                                                         // comment out this line to turn off Serial output
#ifdef DEBUG
  #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t) Serial.printf(q, w, e, r, t)
#else
  #define DEBUG_PRINTF(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t)
#endif
#define EEPROM_SIZE             8                                     // stores 2 unsigned shorts with 2-bytes buffers behind each
#define MAX_STATE_ADDR          0
#define MIN_STATE_ADDR          4
#define BATTERY_DEFAULT_MIN     367                                   // see ReadMe regarding this constant
#define BATTERY_DEFAULT_COEFF   12.06                                 // see ReadMe regarding this constant
#define CONNECT_TIMEOUT         5000                                  // WiFi timeout per each AP, in milliseconds. Increase if cannot connect.
#define WAIT_FOR_OTA_LIMIT      3600000                               // how long to wait for OTA to begin (in milliseconds, 1 HOUR)
#define SLEEP_DURATION          60000ul                               // in milliseconds
#define HOME_SLEEP_TIME         1800000                               // in milliseconds, 30 minutes
#define DEAD_BATTERY_SLEEP      7200000                               // in milliseconds, 2 HOURS

#define SPI_SCK_PIN             12                                    // also known as SCL pin
#define SPI_MISO_PIN            -1                                    // NOT USED IN THIS PROJECT
#define SPI_MOSI_PIN            11                                    // also known as SDA pin, as SPI_D pin or as DIN pin
#define SPI_SS_PIN              10                                    // also known as CS pin
#define DC_PIN                  ?
#define RST_PIN                 ?                                    // also known as RES pin
#define BUSY_PIN                ?
#define ENABLE_GxEPD2_GFX       0

#endif
 
