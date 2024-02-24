
#include "42-Prague-Smart-Sign.h"

void  IRAM_ATTR ft_display_animated_text_with_font(String output)                 // flikers and inverts colours while running, animation doesn't work yet
{
    int16_t   tbx;
    int16_t   tby;
    uint16_t  tbw;
    uint16_t  tbh;
    uint16_t  y;
    uint16_t  x;

    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setRotation(1);
    display.getTextBounds(output, 0, 0, &tbx, &tby, &tbw, &tbh);
    y = (display.height() - tbh) / 2;
    x = display.width();
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(x, y);
        display.print(output);
        x -= 25;                                                                  // this value here controls the speed of movement
        if (x + tbw < 0)
            x = display.width();
    }
    while (display.nextPage());
}

void  IRAM_ATTR ft_display_bitmap_with_refresh(const unsigned char* output)       // flickers, does NOT invert colours while running 
{
    display.setRotation(1);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, output, 296, 128, GxEPD_BLACK);
    }
    while (display.nextPage());
}

void  IRAM_ATTR ft_display_bitmap(const unsigned char* output)                     // uses Full Screen Partial Mode. Does NOT flicker, does NOT invert colours while running 
{
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.setRotation(1);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, output, 296, 128, GxEPD_BLACK);
    }
    while (display.nextPage());
}

void  IRAM_ATTR ft_display_battery_state(void)
{
    String    output;
    int16_t   text_x;
    int16_t   text_y;
    uint16_t  text_width;
    uint16_t  text_height;
    uint16_t  y;
    uint16_t  x;

    ft_display_bitmap(badge_bitmap_low_battery);
    output = String(globals.battery) + "%";
    display.getTextBounds(output, 0, 0, &text_x, &text_y, &text_width, &text_height);
  // center bounding box by transposition of origin:
    x = ((display.width() - text_width) / 2) - text_x + 60;
    y = ((display.height() - text_height) / 2) - text_y;
    display.setRotation(1);                                      // 0 for vertical display, 1 for horizontal rotated right 90 degrees
    display.setPartialWindow(x, y, text_width, text_height);     // x, y, width, height
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.firstPage();
    do
    {
        display.setCursor(x, y);                                 // x and y coordinates (for vertical display)
        display.print(output);
    }
    while (display.nextPage());
}

bool  IRAM_ATTR ft_clear_display(bool errase_display)                          // flickers
{
    if (errase_display)
    {
        display.clearScreen();
        display.writeScreenBuffer();
        errase_display = false;
    }
    return (errase_display);
}

void  ft_run_slideshow(long* p_cycle_length)
{
    ft_display_bitmap_with_refresh(badge_bitmap_slide_6_logo);
    ft_delay(8000);
    if (globals.battery <= 20)
    {
        ft_display_battery_state();
        *p_cycle_length = *p_cycle_length + 8000 * (21 - globals.battery);
        ft_delay(8000);
    }
    ft_display_bitmap(badge_bitmap_slide_4_github);
    ft_delay(8000);
    ft_display_bitmap(badge_bitmap_your_ad_slide);
    ft_delay(8000);
    ft_display_bitmap(badge_bitmap_name_slide_v2);
    display.powerOff();
}

void  IRAM_ATTR ft_display_init(void)
{
    SPI.end();
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    display.init(115200);
}
 
