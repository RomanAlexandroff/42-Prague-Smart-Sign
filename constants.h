/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:36 by raleksan          #+#    #+#             */
/*   Updated: 2024/08/08 11:00:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define SOFTWARE_VERSION        4.09

/* General constants */
# define CONNECT_TIMEOUT         5                            // in seconds
# define DEAD_BATTERY_SLEEP      86400000ul                   // in milliseconds, 24 hours
# define BATTERY_SAMPLES_LIMIT   5
# define OTA_WAIT_LIMIT          600                          // in seconds, 10 minutes
# define RETRIES_LIMIT           3
# define WAKE_UP_HOURS           6, 9, 12, 15, 18, 21
# define NOT_FOUND               -1
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
# define CLUSTER                 0
# define DEFAULT                 1
# define INTRA_ERROR             2
# define SECRET_EXPIRED          3
# define SECRET_CORRUPTED        4
# define EXAM_DAY                5
# define LOW_BATTERY             6
# define DEAD_BATTERY            7
# define OTA_WAITING             8
# define OTA_UPDATING            9
# define OTA_SUCCESS             10
# define OTA_FAIL                11
# define OTA_CANCELED            12
# define TELEGRAM_ERROR          13

#endif
 
