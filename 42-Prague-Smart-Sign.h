
#ifndef _42_PRAGUE_SMART_SIGN_H
# define _42_PRAGUE_SMART_SIGN_H

# include <Arduino.h>                     // String variables manipulations
# include <WiFiClientSecure.h>            // Wi-Fi connection, HTTPS requests
//# include <esp_wifi.h>
# include <WiFiUdp.h>
# include <ESPmDNS.h>
# include <ArduinoOTA.h>
# include "EEPROM.h"                      // saves data even without electricity
# include <Wire.h>                        // manages SPI connections for the display
# include <time.h>                        // gets NTP Server date and time
# include <stdio.h>
# include <stdint.h>                      // provides fixed-width integer types
# include <esp_system.h>
# include "esp_sleep.h"                   // allows to use touch pad for wake up
# include <driver/adc.h>                  // measures battery voltage
# include "bitmap_library.h"
# include "globals.h"

void IRAM_ATTR  ft_eeprom_init(void);
void IRAM_ATTR  ft_battery_init(void);
void IRAM_ATTR  ft_display_init(void);
void            ft_power_down_recovery(void);
short           ft_battery_check(void);
void            ft_buttons_init(void);
void IRAM_ATTR  ft_battery_state(const int address);
void IRAM_ATTR  isr_warning(void);
void IRAM_ATTR  isr_diagnostics(void);
void IRAM_ATTR  isr_ota(void);
void IRAM_ATTR  isr_reboot(void);
void            ft_ota_init(void);
static void     ft_ota_waiting_loop(void);
void            ft_go_to_sleep(uint64_t time_in_millis);
void IRAM_ATTR  ft_delay(uint64_t time_in_millis);
void            ft_wifi_connect(void);
void            ft_get_time(void);
void            ft_fetch_exams(void);

#endif
 
