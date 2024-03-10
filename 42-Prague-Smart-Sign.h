
#ifndef _42_PRAGUE_SMART_SIGN_H
# define _42_PRAGUE_SMART_SIGN_H

# include <Arduino.h>                     // String variables manipulations
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

void       ft_eeprom_init(void);
void       ft_battery_init(void);
void       ft_display_init(void);
void                ft_power_down_recovery(void);
short               ft_battery_check(void);
void                ft_buttons_init(void);
static void         ft_pathfinder(void);
static bool         ft_result_chech(const int address, uint16_t result, uint16_t old_max, uint16_t old_min);
void IRAM_ATTR      ft_battery_state(const int address);
void IRAM_ATTR      isr_warning(void);
void IRAM_ATTR      isr_diagnostics(void);
void IRAM_ATTR      isr_ota(void);
void IRAM_ATTR      isr_reboot(void);
void                ft_ota_init(void);
static void         ft_ota_waiting_loop(void);
void                ft_go_to_sleep(uint64_t time_in_millis);
void       ft_delay(uint64_t time_in_millis);
void                ft_wifi_connect(void);
void                ft_get_time(void);
unsigned int        ft_time_till_event(uint8_t hours, uint8_t minutes);
static unsigned int ft_exam(bool* p_preexam_check);
static void         ft_preexam_warning(unsigned int* p_preexam_time);
void                ft_exam_mode(unsigned int* p_sleep_length);
static unsigned int ft_time_till_wakeup(void);
void                ft_cluster_number_mode(unsigned int* p_sleep_length);
static uint8_t      ft_number_of_exams(String* p_server_message);
static void         ft_clean_data(String server_message);
static void         ft_get_data_load(const String token, String* p_server_message);
static void         ft_get_token(const String* p_token);
void                ft_fetch_exams(void);

void ft_display_exam_sign(void);
void ft_display_timer_exact_end_time(void);
void ft_display_timer(int minutes);
void   ft_display_animated_text_with_font(String output);
void   ft_display_bitmap_with_refresh(const unsigned char* output);
void   ft_display_bitmap(const unsigned char* output);
void   ft_display_battery_state(void);
bool   ft_clear_display(bool errase_display);


#endif
 
