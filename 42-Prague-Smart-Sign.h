/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42-Prague-Smart-Sign.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:10 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 12:59:16 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _42_PRAGUE_SMART_SIGN_H
# define _42_PRAGUE_SMART_SIGN_H

//# define ESP32
//# define ESP32_S2                                                    // Ai-Thinker ESP32—S2—WROVER—I board
//# define ESP32_C3                                                    // Ai-Thinker ESP32—C3—1M board 
# define XIAO_ESP32C3                                                  // Seeed Studio XIAO ESP32—C3 board

# include <Arduino.h>
# include "LittleFS.h"
# include <WiFiUdp.h>
# include <ESPmDNS.h>
# include <ArduinoOTA.h>
# include <time.h>
# include <stdio.h>
# include <stdint.h>
# include <esp_system.h>
# include "esp_sleep.h"
# include <driver/adc.h>
# include "bitmap_library.h"
# include "globals.h"

# define DEBUG                                                         // comment out this line to turn off Serial output
# ifdef DEBUG
    #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
# else
    #define DEBUG_PRINTF(x, y)
# endif

void                  ft_serial_init(void);
void                  ft_spiffs_init(void);
# if !defined(XIAO_ESP32C3)
    static bool       ft_result_check(const String address, int32_t result, int32_t old_max, int32_t old_min);
    void IRAM_ATTR    ft_battery_state(const String address);
    short             ft_battery_check(void);
    void              ft_battery_init(void);
# endif
void IRAM_ATTR        ft_display_init(void);
void                  ft_power_down_recovery(void);
static void           ft_pathfinder(void);
short                 ft_write_spiffs_file(const char* file_name, String input);
String                ft_read_spiffs_file(const char* file_name);
void IRAM_ATTR        ft_ota_init(void);
static void IRAM_ATTR ft_ota_waiting_loop(void);
void                  ft_go_to_sleep(uint64_t time_in_millis);
void IRAM_ATTR        ft_delay(uint64_t time_in_millis);
void                  ft_wifi_connect(void);
int8_t                ft_expiration_counter(void);
bool                  ft_unix_timestamp_decoder(uint8_t* p_day, uint8_t* p_month, uint16_t* p_year);
bool                  ft_get_time(void);
unsigned int          ft_time_till_event(int8_t hours, uint8_t minutes);
int                   ft_time_sync(unsigned int preexam_time);
static unsigned int   ft_exam(bool* p_preexam_check);
static void           ft_preexam_warning(unsigned int* p_preexam_time);
void                  ft_exam_mode(unsigned int* p_sleep_length);
static unsigned int   ft_time_till_wakeup(void);
void                  ft_cluster_number_mode(unsigned int* p_sleep_length);
bool                  ft_fetch_exams(void);
void                  ft_draw_colour_bitmap(const unsigned char* black_image, const unsigned char* red_image);
static void           ft_draw_text(String output, uint16_t x, uint16_t y);
static void           ft_draw_exam_start_time(void);
static void           ft_draw_bitmap_partial_update(const unsigned char* image, uint16_t width, uint16_t height);
static void           ft_draw_bitmap_full_update(const unsigned char* image, uint16_t width, uint16_t height);
void IRAM_ATTR        ft_display_cluster_number(uint8_t mode);
bool IRAM_ATTR        ft_clear_display(bool errase_display);
String                ft_compose_message(uint8_t subject, int8_t days_left);
bool                  ft_secret_verification(String text);
static void           ft_reply_machine(String text);
static void           ft_new_messages(short message_count);
void                  ft_telegram_check(void);
void IRAM_ATTR        ft_brownout_sequence(void);
void                  ft_poweron_sequence(void);
//void                  ft_buttons_init(void);
//void IRAM_ATTR        isr_warning(void);
//void IRAM_ATTR        isr_diagnostics(void);
//void IRAM_ATTR        isr_ota(void);

#endif
 
