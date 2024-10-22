/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/10/21 09:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define SOFTWARE_VERSION        4.20

/* General constants */
# define CONNECT_TIMEOUT         5                            // in seconds
# define REBOOT                  1000                         // do not change!
# define DEAD_BATTERY_SLEEP      86400000ul                   // in milliseconds, 24 hours
# define BATTERY_CRITICAL        400
# define BATTERY_GOOD            800
# define BATTERY_SAMPLES_LIMIT   5
# define OTA_WAIT_LIMIT          600                          // in seconds, 10 minutes
# define RETRIES_LIMIT           3
# define WAKE_UP_HOURS           6, 9, 12, 15, 18, 21
# define NOT_FOUND               -1
# define SUBS_CHECK_TIME_LIMIT   3900000ul                    // in milliseconds, 1 hour 5 minutes
# define FAILED_TO_COUNT         -128

/* Errors constants */
typedef enum {
    UNKNOWN = 0,
    INTRA_OK = 200,
    INTRA_NO_SERVER = 503,
    INTRA_NO_TOKEN = 401,
    INTRA_NO_INFO = 404
} ERROR_t;

/* Buttons */
# define DIAGNOSTICS_BUTTON      D0
# define OTA_BUTTON              D1
# define WARNING_BUTTON          D2

/* Display SPI port config */
# define BUSY_PIN                D4
# define RST_PIN                 D5
# define DC_PIN                  D6
# define SPI_SS_PIN              D7
# define SPI_SCK_PIN             D8
# define SPI_MISO_PIN            -1
# define SPI_MOSI_PIN            D10

/* Display driver config */
# define GxEPD2_DISPLAY_CLASS    GxEPD2_3C
# define GxEPD2_DRIVER_CLASS     GxEPD2_750c_Z08
# define GxEPD2_3C_IS_GxEPD2_3C  true
# define IS_GxEPD2_3C(x)         IS_GxEPD(GxEPD2_3C_IS_, x)
# define MAX_DISPLAY_BUFFER_SIZE 96124ul
# define MAX_HEIGHT(EPD)         (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

/* Images constants */
# define CLUSTER                 0
# define DEFAULT                 1
# define INTRA_ERROR             2
# define SECRET_EXPIRED          3
# define EXAM_DAY                4
# define LOW_BATTERY             5
# define DEAD_BATTERY            6
# define OTA_WAITING             7
# define OTA_UPDATING            8
# define OTA_SUCCESS             9
# define OTA_FAIL                10
# define OTA_CANCELED            11
# define TELEGRAM_ERROR          12

#endif
 
