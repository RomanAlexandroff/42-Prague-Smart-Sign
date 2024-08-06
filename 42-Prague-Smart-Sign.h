/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42-Prague-Smart-Sign.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:10 by raleksan          #+#    #+#             */
/*   Updated: 2024/05/29 12:59:16 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _42_PRAGUE_SMART_SIGN_H
# define _42_PRAGUE_SMART_SIGN_H

# define DEBUG                                                       // comment out this line to turn off Serial output
# ifdef DEBUG
    #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
# else
    #define DEBUG_PRINTF(x, y)
# endif

# include <Arduino.h>
# include <LittleFS.h>
# include <WiFiUdp.h>
# include <ESPmDNS.h>
# include <ArduinoOTA.h> 
# include <time.h>
# include <stdio.h>
# include <stdint.h>
# include <esp_system.h>
# include <esp_sleep.h>
# include <driver/adc.h>
# include <driver/gpio.h>
# include "bitmap_library.h"
# include "globals.h"

// 42-Prague-Smart-Sign.ino
static void           ft_pathfinder(void);

// Intra_interaction.cpp
static void           ft_get_exam_time(String server_response);
static bool           ft_handle_exams_info(void);
static void           ft_request_exams_info(const char* server, String* token);
static void           ft_get_secret_expiration(String server_response);
static String         ft_get_token(String server_response);
static bool           ft_handle_server_response(const char* server, String* token);
static void           ft_access_server(const char* server);
static bool           ft_intra_connect(const char* server);
bool                  ft_fetch_exams(void);

// battery_management.cpp
static bool           ft_charging_detection(int16_t battery);
void                  ft_battery_check(void);
void                  ft_battery_init(void);

// buttons_handling.cpp
void                  ft_buttons_init(void);
void                  ft_buttons_deinit(void);
void IRAM_ATTR        isr_diagnostics(void);
void IRAM_ATTR        isr_ota(void);
void IRAM_ATTR        isr_warning(void);

// cluster_number_mode.cpp
static unsigned int   ft_time_till_wakeup(void);
void                  ft_cluster_number_mode(unsigned int* p_sleep_length);

// display_handling.cpp
void                  ft_draw_colour_bitmap(const unsigned char* black_image, const unsigned char* red_image);
static void           ft_draw_text(String output, uint16_t x, uint16_t y);
static void           ft_draw_exam_start_time(void);
static void           ft_draw_bitmap_partial_update(const unsigned char* image, uint16_t width, uint16_t height);
static void           ft_draw_bitmap_full_update(const unsigned char* image, uint16_t width, uint16_t height);
void IRAM_ATTR        ft_display_cluster_number(uint8_t mode);
void                  ft_clear_display(void);
void IRAM_ATTR        ft_display_init(void);

// exam_mode.cpp
static unsigned int   ft_exam(void);
static void           ft_preexam_warning(unsigned int* p_preexam_time);
void                  ft_exam_mode(unsigned int* p_sleep_length);

// file_system.cpp
bool                  ft_secret_verification(String text);
bool                  ft_data_restore(String input, int16_t checksum);
int16_t               ft_checksum(String input, int16_t checksum);
short                 ft_write_spiffs_file(const char* file_name, String input);
String                ft_read_spiffs_file(const char* file_name);
void                  ft_spiffs_init(void);

// ota.h
inline void           ft_ota_init(void) __attribute__((always_inline));
inline void           ft_ota_waiting_loop(void) __attribute__((always_inline));

// other.cpp
void                  ft_go_to_sleep(uint64_t time_in_millis);
void IRAM_ATTR        ft_delay(uint64_t time_in_millis);
void                  ft_serial_init(void);
void                  ft_wifi_connect(void);

// power_down_recovery.cpp
static void IRAM_ATTR ft_brownout_sequence(void);
static void           ft_poweron_sequence(void);
void                  ft_power_down_recovery(void);

// telegram_bot.cpp
String                ft_compose_message(uint8_t subject, int8_t days_left);
static void           ft_reply_machine(String text);
static void           ft_new_messages(short message_count);
void                  ft_telegram_check(void);

// time_utilities.cpp
int8_t                ft_expiration_counter(void);
bool                  ft_unix_timestamp_decoder(uint8_t* p_day, uint8_t* p_month, uint16_t* p_year);
bool                  ft_get_time(void);
unsigned int          ft_time_till_event(int8_t hours, uint8_t minutes);
int                   ft_time_sync(unsigned int preexam_time);

# include "ota.h"                                                   // has to be here

#endif
 
