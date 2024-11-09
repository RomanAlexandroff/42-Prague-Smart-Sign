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

# define SOFTWARE_VERSION        4.26

/* General constants */
# define CONNECT_TIMEOUT         5                            // in seconds
# define REBOOT                  1000                         // do not change
# define MS_MINUTE               60000                        // do not change
# define MS_HOUR                 3600000ul                    // do not change
# define DEAD_BATTERY_SLEEP      86400000ul                   // in milliseconds, 24 hours
# define BATTERY_CRITICAL        400
# define BATTERY_GOOD            800
# define BATTERY_SAMPLES_LIMIT   5
# define OTA_WAIT_LIMIT          600                          // in seconds, 10 minutes
# define RETRIES_LIMIT           3
# define WAKE_UP_HOURS           6, 9, 12, 15, 18, 21
# define NOT_FOUND               -1                           // do not change
# define SUBS_CHECK_TIME_LIMIT   3900000ul                    // in milliseconds, 1 hour 5 minutes
# define FAILED_TO_COUNT         -128

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
typedef enum {
    CLUSTER = 1000,
    DEFAULT_IMG,
    INTRA_ERROR,
    SECRET_EXPIRED,
    EXAM_DAY,
    LOW_BATTERY,
    DEAD_BATTERY,
    OTA_WAITING,
    OTA_SUCCESS,
    OTA_FAIL,
    OTA_CANCELED,
    TELEGRAM_ERROR,
    TELEGRAM_STATUS
} IMAGE_t;

/* Errors constants */
typedef enum {
    UNKNOWN = 2000,
    TIME_OK,
    TIME_NO_WIFI,
    TIME_NO_SERVER,
    TIME_NO_DST,
    INTRA_OK,
    INTRA_NO_SERVER,
    INTRA_NO_TOKEN,
    INTRA_NO_INFO,
    FS_OK,
    FS_INIT_FAIL,
    FS_OPEN_FAIL,
    FS_EMPTY_FILE,
    FS_ENTRY_ERROR,
    FS_VALID_SECRET,
    FS_INVALID_SECRET,
    FS_NOT_A_SECRET
} ERROR_t;

#endif
 
