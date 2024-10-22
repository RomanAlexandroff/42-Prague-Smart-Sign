/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42-Prague-Smart-Sign.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:59:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/10/21 12:59:00 by raleksan         ###   ########.fr       */
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

/* intra_interaction.cpp */
ERROR_t               ft_fetch_exams(void);

/* battery_management.cpp */
void                  ft_battery_check(void);
void                  ft_battery_init(void);

/* buttons_handling.cpp */
void                  ft_buttons_init(void);
void                  ft_buttons_deinit(void);
void IRAM_ATTR        isr_diagnostics(void);
void IRAM_ATTR        isr_ota(void);
void IRAM_ATTR        isr_warning(void);

/* cluster_number_mode.cpp */
void                  ft_cluster_number_mode(unsigned int* p_sleep_length);

/* display_handling.cpp */
void                  ft_draw_colour_bitmap(const unsigned char* black_image, const unsigned char* red_image);
void IRAM_ATTR        ft_display_cluster_number(uint8_t mode);
void                  ft_clear_display(void);
void IRAM_ATTR        ft_display_init(void);

/* exam_mode.cpp */
void                  ft_exam_mode(void);

/* file_system.cpp */
bool                  ft_secret_verification(String text);
void                  ft_data_restore(const char* file_name);
void                  ft_data_integrity_check(void);
short                 ft_write_spiffs_file(const char* file_name, String input);
String                ft_read_spiffs_file(const char* file_name);
void                  ft_spiffs_init(void);

/* ota.h */
inline void           ft_ota_init(void) __attribute__((always_inline));
inline void           ft_ota_waiting_loop(void) __attribute__((always_inline));

/* other.cpp */
void                  ft_go_to_sleep(uint64_t time_in_millis);
void IRAM_ATTR        ft_delay(uint64_t time_in_millis);
void                  ft_serial_init(void);
void                  ft_wifi_connect(void);

/* power_down_recovery.cpp */
void                  ft_power_down_recovery(void);

/* telegram_bot.cpp */
void                  ft_telegram_check(void);

/* telegram_compose_message.cpp */
String                ft_compose_message(uint16_t subject, int8_t days_left);

/* time_utilities.cpp */
int8_t                ft_expiration_counter(void);
bool                  ft_unix_timestamp_decoder(uint8_t* p_day, uint8_t* p_month, uint16_t* p_year);
bool                  ft_get_time(void);
unsigned int          ft_time_till_wakeup(void);
unsigned int          ft_time_till_event(int8_t hours, uint8_t minutes);
int                   ft_time_sync(unsigned int preexam_time);

# include "ota.h"                                                   // has to be here

#endif
 
