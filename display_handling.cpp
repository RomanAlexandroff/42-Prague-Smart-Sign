/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:00:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/11/09 12:30:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"


/*
*   For all the text notes on the right side
*   (480x170 window) of the cluster number.
*   Partial display update, text only, no images
*/
static void  ft_draw_text(String output, uint16_t x, uint16_t y)
{
    const int16_t   text_box_x PROGMEM = 224;
    const int16_t   text_box_y PROGMEM = 732;
    const uint16_t  text_width PROGMEM = 140;
    const uint16_t  text_height PROGMEM = 40;

    display.setRotation(3);
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setPartialWindow(text_box_x, text_box_y, text_width, text_height);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y);
        display.print(output);
    }
    while (display.nextPage());
}


/*
*   Only for the text note about the exam start
*   time on the right side (480x170 window) of
*   the cluster number.
*   Partial display update, text only, no images
*/
static void  ft_draw_exam_start_time(void)
{
    String         text;
    const int16_t  text_x PROGMEM = 27;
    const int16_t  text_y PROGMEM = 776;
    const int16_t  window_x PROGMEM = 0;
    const int16_t  window_y PROGMEM = 740;
    const int16_t  window_width PROGMEM = 480;
    const int16_t  window_height PROGMEM = 40;

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


/*
*   Only for the right-side (480x170) images
*   Partial display update, Black and white only
*/
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


/*
*   For full-screen b/r/w images.
*   Full display update, Black-Red-White only
*/
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


/*
*   For any size b/w images.
*   Full display update, Black-White only
*/
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


/*
*   This function displays the number of the cluster and
*   any additional image/message separately.
*   The function ensures that whatever is already drawn
*   on the display does not get drawn repeatedly.
*   
*   "displaying_now = CLUSTER" is there to unblock drawing
*   of ALL the additional images/messages. Useful after exams.
*/
void IRAM_ATTR  ft_display_cluster_number(IMAGE_t mode)
{
    RTC_DATA_ATTR static bool    display_cluster;
    RTC_DATA_ATTR static IMAGE_t displaying_now;

    if (display_cluster && mode == displaying_now)
    {
        DEBUG_PRINTF("\n[THE DISPLAY] Nothing new to draw. Drawing aborted\n\n", "");
        return;
    }
    if (!display_cluster)
    {
        DEBUG_PRINTF("\n[THE DISPLAY] Drawing the cluster number with...\n", "");
        ft_draw_bitmap_full_update(cluster_number_img, 630, 480);
        display_cluster = true;
        displaying_now = CLUSTER;
    }
    if (mode == DEFAULT_IMG && (displaying_now != DEFAULT_IMG && displaying_now != LOW_BATTERY))
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the default cluster icons\n", "");
        ft_draw_bitmap_partial_update(default_cluster_icons, 170, 480);
        displaying_now = DEFAULT_IMG;
    }
    else if (mode == INTRA_ERROR && displaying_now != INTRA_ERROR)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the Intra error warning\n", "");
        ft_draw_bitmap_partial_update(intra_error_img, 170, 480);
        displaying_now = INTRA_ERROR;
    }
    else if (mode == SECRET_EXPIRED && displaying_now != SECRET_EXPIRED)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the Secret expiration warning\n", "");
        ft_draw_bitmap_partial_update(secret_expire_img, 170, 480);
        displaying_now = SECRET_EXPIRED;
    }
    else if (mode == EXAM_DAY && displaying_now != EXAM_DAY)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the exam time note\n", "");
        ft_draw_bitmap_partial_update(reserve_note_img, 170, 480);
        delay (500);
        ft_draw_exam_start_time();
        display_cluster = false;
        displaying_now = EXAM_DAY;
    }
    else if (mode == LOW_BATTERY && displaying_now != LOW_BATTERY)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the low battery warning\n", "");
        ft_draw_bitmap_partial_update(low_battery_img, 170, 480);
        displaying_now = LOW_BATTERY;
    }
    else if (mode == OTA_WAITING && displaying_now != OTA_WAITING)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the OTA notification\n", "");
        ft_draw_text("WAITING FOR OTA UPDATE", 50, 710);
        displaying_now = OTA_WAITING;
    }
    else if (mode == OTA_SUCCESS && displaying_now != OTA_SUCCESS)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the OTA notification\n", "");
        ft_draw_text("OTA UPDATE SUCCESS", 50, 710);
        displaying_now = OTA_SUCCESS;
    }
    else if (mode == OTA_FAIL && displaying_now != OTA_FAIL)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the OTA notification\n", "");
        ft_draw_text("OTA UPDATE FAIL", 50, 710);
        displaying_now = OTA_FAIL;
    }
    else if (mode == OTA_CANCELED && displaying_now != OTA_CANCELED)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the OTA notification\n", "");
        ft_draw_text("OTA UPDATE CANCELED", 50, 710);
        displaying_now = OTA_CANCELED;
    }
    else if (mode == TELEGRAM_ERROR && displaying_now != TELEGRAM_ERROR)
    {
        DEBUG_PRINTF("[THE DISPLAY] ...the Telegram error warning\n", "");
        ft_draw_text("TELEGRAM BOT ERROR", 50, 710);
        displaying_now = TELEGRAM_ERROR;
    } 
    DEBUG_PRINTF("[THE DISPLAY] The drawing process is complete\n", "");
}

void  ft_clear_display(void)
{
    display.clearScreen();
    display.writeScreenBuffer();
}

void IRAM_ATTR  ft_display_init(void)
{
    SPI.end();
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    display.init(115200);
}
 
