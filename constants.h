
#ifndef CONSTANTS_H
# define CONSTANTS_H

# define SOFTWARE_VERSION        1.01

#define DEBUG                                                         // comment out this line to turn off Serial output
#ifdef DEBUG
    #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
#else
    #define DEBUG_PRINTF(x, y)
#endif

# define EEPROM_SIZE             8                                     // stores 2 unsigned shorts with 2-bytes buffers behind each
# define MAX_STATE_ADDR          0
# define MIN_STATE_ADDR          4
# define BATTERY_DEFAULT_MIN     367                                   // see ReadMe regarding this constant
# define BATTERY_DEFAULT_COEFF   12.06                                 // see ReadMe regarding this constant
# define CONNECT_TIMEOUT         5                                     // WiFi timeout per each network, in seconds. Increase if cannot connect.
# define WAIT_FOR_OTA_LIMIT      3600000                               // how long to wait for OTA to begin (in milliseconds, 1 HOUR)
# define SLEEP_DURATION          60000ul                               // in milliseconds
# define DEAD_BATTERY_SLEEP      7200000                               // in milliseconds, 2 HOURS

# define BUTTON_THRESHOLD        5000                                  // lower the value, more sensitive the buttons
# define WARNING_BUTTON          T1
# define DIAGNOSTICS_BUTTON      T2
# define OTA_BUTTON              T3
# define REBOOT_BUTTON           T4
# define BUTTON_5                T5                                    // for future development
# define BUTTON_6                T6                                    // for future development

# define BUSY_PIN                7
# define RST_PIN                 8                                     // also known as RES pin
# define DC_PIN                  9
# define SPI_SS_PIN              10                                    // also known as CS pin
# define SPI_MOSI_PIN            11                                    // also known as SDA pin, as SPI_D pin or as DIN pin
# define SPI_SCK_PIN             12                                    // also known as SCL pin
# define SPI_MISO_PIN            -1                                    // NOT USED IN THIS PROJECT
# define GxEPD2_DISPLAY_CLASS    GxEPD2_3C
# define GxEPD2_DRIVER_CLASS     GxEPD2_750c_Z08
# define GxEPD2_3C_IS_GxEPD2_3C  true
# define MAX_DISPLAY_BUFFER_SIZE 32000ul                               // recomended buffer size for 3-colour 800x480 e-paper display — 96 KB
# define MAX_HEIGHT(EPD)         (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

# define DEFAULT                 1
# define INTRA_ERROR             2
# define EXAM_DAY                3
# define LOW_BATTERY             4
# define OTA_WAITING             5
# define OTA_UPDATING            6
# define OTA_SUCCESS             7
# define OTA_FAIL                8
# define OTA_CANCELED            9

#endif
 
