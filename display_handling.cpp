/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:56 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:00:57 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

void  ft_draw_colour_bitmap(const unsigned char* black_image, const unsigned char* red_image)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, black_image, 800, 480, GxEPD_WHITE);
        display.drawBitmap(0, 0, red_image, 800, 480, GxEPD_RED);
    }
    while (display.nextPage());
}

static void  ft_draw_text(String output, uint16_t x, uint16_t y)
{
    int16_t         text_box_x;
    int16_t         text_box_y;
    uint16_t        text_width;
    uint16_t        text_height;

    display.setRotation(3);
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setPartialWindow(224, 732, 140, 40);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y);
        display.print(output);
    }
    while (display.nextPage());
}

static void  ft_draw_exam_start_time(void)
{
    String   text;
    int16_t  text_x = 27;
    int16_t  text_y = 776;
    int16_t  window_x = 0;
    int16_t  window_y = 740;
    int16_t  window_width = 480;
    int16_t  window_height = 40;

    text = "TODAY AT ";
    text += String(rtc_g.exam_start_hour);
    if (rtc_g.exam_start_minutes < 10)
        text += ":0" + String(rtc_g.exam_start_minutes);
    else
        text += ":" + String(rtc_g.exam_start_minutes);
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setRotation(3);
    display.setPartialWindow(window_x, window_y, window_width, window_height);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(text_x, text_y);
        display.print(text);
    }
    while (display.nextPage());
    display.powerOff();
}

static void ft_draw_bitmap_partial_update(const unsigned char* image, uint16_t width, uint16_t height)
{
    display.setRotation(0);
    display.setPartialWindow(630, 0, width, height);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(630, 0, image, width, height, GxEPD_BLACK);
    }
    while (display.nextPage());
    display.powerOff();
}

static void ft_draw_bitmap_full_update(const unsigned char* image, uint16_t width, uint16_t height)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, image, width, height, GxEPD_BLACK);
    }
    while (display.nextPage());
}

void IRAM_ATTR  ft_display_cluster_number(uint8_t mode)
{
    RTC_DATA_ATTR static bool    display_cluster;
    RTC_DATA_ATTR static uint8_t flag;

    if (!display_cluster)
    {
        ft_draw_bitmap_full_update(cluster_number_img, 630, 480);
        display_cluster = true;
        flag = CLUSTER;
    }
    switch (mode)
    {
        case DEFAULT:
            if (flag == DEFAULT)
                return;
            ft_draw_bitmap_partial_update(default_cluster_icons, 170, 480);
            flag = DEFAULT;
            break;
        case INTRA_ERROR:
            if (flag == INTRA_ERROR)
                return;
            ft_draw_bitmap_partial_update(intra_error_img, 170, 480);
            flag = INTRA_ERROR;
            break;
        case SECRET_EXPIRED:
            if (flag == SECRET_EXPIRED)
                return;
            ft_draw_bitmap_partial_update(secret_expire_img, 170, 480);
            flag = SECRET_EXPIRED;
            break;
        case EXAM_DAY:
            if (flag == EXAM_DAY)
                return;
            ft_draw_bitmap_partial_update(reserve_note_img, 170, 480);
            delay (500);
            ft_draw_exam_start_time();
            display_cluster = false;
            flag = EXAM_DAY;
            break;
        case LOW_BATTERY:
            if (flag == LOW_BATTERY)
                return;
            ft_draw_bitmap_partial_update(low_battery_img, 170, 480);
            flag = LOW_BATTERY;
            break;
        case OTA_WAITING:
            if (flag == OTA_WAITING)
                return;
            ft_draw_text("WAITING FOR OTA UPDATE", 50, 710);
            flag = OTA_WAITING;
            break;
        case OTA_UPDATING:
            if (flag == OTA_UPDATING)
                return;
            ft_draw_text("OTA UPDATE IN PROGRESS...", 50, 710);
            flag = OTA_UPDATING;
            break;
        case OTA_SUCCESS:
            if (flag == OTA_SUCCESS)
                return;
            ft_draw_text("OTA UPDATE SUCCESS", 50, 710);
            flag = OTA_SUCCESS;
            break;
        case OTA_FAIL:
            if (flag == OTA_FAIL)
                return;
            ft_draw_text("OTA UPDATE FAIL", 50, 710);
            flag = OTA_FAIL;
            break;
        case OTA_CANCELED:
            if (flag == OTA_CANCELED)
                return;
            ft_draw_text("OTA UPDATE CANCELED", 50, 710);
            flag = OTA_CANCELED;
            break;
    }
}

bool IRAM_ATTR   ft_clear_display(bool errase_display)
{
    if (errase_display)
    {
        display.clearScreen();
        display.writeScreenBuffer();
        errase_display = false;
    }
    return (errase_display);
}

void IRAM_ATTR  ft_display_init(void)
{
    SPI.end();
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    display.init(115200);
}
 
