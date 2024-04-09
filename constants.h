/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:36 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:00:37 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define SOFTWARE_VERSION        3.00

# define MAX_STATE               "/battery_max.txt"
# define MIN_STATE               "/battery_min.txt"
# define BATTERY_DEFAULT_MAX     4094
# define BATTERY_DEFAULT_MIN     2000
# define BATTERY_DEFAULT_COEFF   12.06
# define CONNECT_TIMEOUT         5
# define WAIT_FOR_OTA_LIMIT      3600000ul
# define DEAD_BATTERY_SLEEP      7200000ul
/*
# define BUTTON_THRESHOLD        5000
# define WARNING_BUTTON          1
# define DIAGNOSTICS_BUTTON      2
# define OTA_BUTTON              3
# define BUTTON_4                4
*/
# if defined(ESP32) || defined(ESP32_S2)
    #define BUSY_PIN             7
    #define RST_PIN              8
    #define DC_PIN               9
    #define SPI_SS_PIN           10
    #define SPI_MOSI_PIN         11
    #define SPI_SCK_PIN          12
    #define SPI_MISO_PIN         -1
# elif defined(ESP32_C3)
    #define BUSY_PIN             19
    #define RST_PIN              18
    #define DC_PIN               0
    #define SPI_SS_PIN           10
    #define SPI_MOSI_PIN         7
    #define SPI_SCK_PIN          6
    #define SPI_MISO_PIN         -1
# elif defined(XIAO_ESP32C3)
    #define BUSY_PIN             D4
    #define RST_PIN              D5
    #define DC_PIN               D6
    #define SPI_SS_PIN           D7
    #define SPI_SCK_PIN          D8
    #define SPI_MISO_PIN         -1
    #define SPI_MOSI_PIN         D10
# endif
# define GxEPD2_DISPLAY_CLASS    GxEPD2_3C
# define GxEPD2_DRIVER_CLASS     GxEPD2_750c_Z08
# define GxEPD2_3C_IS_GxEPD2_3C  true
# define IS_GxEPD2_3C(x)         IS_GxEPD(GxEPD2_3C_IS_, x)
# define MAX_DISPLAY_BUFFER_SIZE 96124ul
# define MAX_HEIGHT(EPD)         (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

# define CLUSTER                 0
# define DEFAULT                 1
# define INTRA_ERROR             2
# define SECRET_EXPIRED          3
# define EXAM_DAY                4
# define LOW_BATTERY             5
# define OTA_WAITING             6
# define OTA_UPDATING            7
# define OTA_SUCCESS             8
# define OTA_FAIL                9
# define OTA_CANCELED            10

#endif
 
