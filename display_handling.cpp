
#include "42-Prague-Smart-Sign.h"

/*
 *  We need to be able to create a total number of 13 different slides. For that we have 
 *  6 bitmaps and system fonts. The bitmaps are fixed images that can be placed staticaly 
 *  anywhere on the display. With the system fonts we can write any phrases and texts.
 *  Here is the list of the slides and what each of them shall consist of:
 *      - boot_up_img                         system fonts only
 *      - empty_battery_img                   system fonts only
 *      - default_cluster_number_img          cluster_number_img + default_cluster_icons
 *      - intra_error_img                     cluster_number_img + system fonts for the error note
 *      - cluster_number_with_exam_note       cluster_number_img + system fonts for the exam note
 *      - low_battery_img                     cluster_number_img + system fonts for the low battery note
 *      - ota_waiting_img                     cluster_number_img + system fonts for the OTA waiting note
 *      - ota_updating_img                    cluster_number_img + system fonts for the OTA updating note
 *      - ota_success_img                     cluster_number_img + system fonts for the OTA success note
 *      - ota_fail_img                        cluster_number_img + system fonts for the OTA fail note
 *      - ota canceled                        cluster_number_img + system fonts for the OTA cancel note
 *      - exam_warning_img                    exam_warning_img_0 + exam_warning_img_1 + system fonts for the exam time note
 *      - preexam_warning_img                 preexam_warning_img + preexam_warning_icons + system fonts for the exam time note
 *  You may see what the final slides look should be like in the Slides Final Look folder.
 *  The slides may be devided into 4 groups by the maner of their assembly:
 *      - 2 slides that consist of system fonts only,
 *      - 8 slides that consist of the cluster_number and some note in system font,
 *      - 1 slide of preexam warning that has a unique assembly steps,
 *      - 1 slide of exam warning that has a unique assembly steps.
 *  It means that we will need at least 4 different assembly functions to take care of each
 *  of the slides groups.
 */

/*

NOTES ON COORDINATES, SIZES IN PIXELS AND ROTATIONS

In the setRotation(uint8) function you may set how the window you want to output on the display will be turned compared to the display
physical orientation. All the displays have their default physical orientation, but it may vary from display to display, so finding out
the display's default physical orientation is the starting point for any work with display graphics. It is easy to do: take a look at
the display's physical appearence, note the side where a ribbon of wires sticks out of it — that's the bottom side of this
display's default physical orientation. Knowing that, we now can set the rotation:

    0 — for the default rotation, it is equal to the display default physical orientation
    1 — will rotate the window 90 degrees clockwise comparing to the display default physical orientation
    2 — will rotate the window 180 degrees (or simply flips it up-side-down) comparing to the display default physical orientation
    3 — will rotate the window 270 degrees clockwise comparing to the display default physical orientation

Keep in mind that however you rotate the window, its system of coordinates always rotates with it. So, the 0,0 coordinates are always in
the TOP LEFT corner of your window. 

When dealing with the Partial Update windows, it is important to remember about one unobvious limitation of the e-paper controller:
partial update window size and position is on byte boundary with physical x direction. Meaning that the controller may set
the horisontal window boundaries only on every 8th pixel comparing to its default physical orientation. Let's summon it up with a simple
rule that is easy to follow:
    x and width should be multiple of 8, for rotation 0 or 2,
    y and hight should be multiple of 8, for rotation 1 or 3.

The function setPartialWindow(x, y, width, height) allows us to create on our display a window of any size and, instead of the whole
display, update only this window. It is very useful feature since it is much faster than the conventional full display update with
the setFullWindow() function. Moreover this type of an update does not flicker. So, if you need to get rid off of the flickers
between the slides, with setPartialWindow(x, y, width, height) you may set the whole display as a Partial Update window. But be
careful as THE FIRST UPDATE AFTER TURNING ON OR RESET SHALL ALWAYS BE THE FULL DISPLAY UPDATE with the setFullWindow() function.
Otherwise the display may be irrevertably stuck on the last image for ever.

It may be useful to note the relationship between the setPartialWindow(x, y, width, height) and the setRotation(uint8) functions.
Some may think that setRotation(uint8) may turn a Partial Update window created with the setPartialWindow(x, y, width, height) function.
And it is true, but only to some extent. setRotation(uint8) does not turn Partial Update windows, but it actually turns the whole display window
orientation with Partial Update windows inside of it. That is why when we change the display orientation with the setRotation(uint8) function,
we also need to remember that the system of coordinates for x and y in the setPartialWindow(x, y, width, height) also changes. And when
the system of coordinates changes, the understanding of width and height changes with it.
E.g. if while setRotation(0) the window's width is 800 and the window's height is 480, then while setRotation(1) the window's width becomes 480 
and the window's height becomes 800.
 */

void ft_display_exam_sign(void)
{
  return;
}

void ft_display_timer_exact_end_time(void)
{
  return;
}

void ft_display_timer(int minutes)
{
  return;
}

//================= The following function is only an example of drawing a 3-colour bitmap ====================
//================= from the library and was not yet adapted for the program ==================================

#if defined(ESP32) && defined(_GxBitmaps3c1304x984_H_)
void drawBitmaps3c1304x984()
{
    display.setFullWindow();
    display.setRotation(0);
    //display.drawImage(Bitmap3c1304x984_black, Bitmap3c1304x984_red, 0, 0, 1304, 984, false, false, true);
    display.writeImage(0, Bitmap3c1304x984_red, 0, 0, 1304, 984, true, false, true); // red bitmap is inverted
    display.drawImage(Bitmap3c1304x984_black, 0, 0, 0, 1304, 984, true, false, true); // black bitmap is normal
}
#endif

//=============================================================================================================








static void  ft_draw_text(String output)                                              // flikers and inverts colours while running
{
    const int16_t   window_x PROGMEM = 0;
    const int16_t   window_y PROGMEM = 631;
    const uint16_t  window_width PROGMEM = 480;
    const uint16_t  window_height PROGMEM = 169;
    int16_t         text_x;
    int16_t         text_y;
    uint16_t        text_width;
    uint16_t        text_height;

    display.setPartialWindow(window_x, window_y, window_width, window_height);                // create partial update window
    display.setRotation(3);                                                                   // rotate the whole display so the future text faces right
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.getTextBounds(output, 0, 0, &text_x, &text_y, &text_width, &text_height);         // check the dimensions of the future text
    text_x = ((window_width - text_width) / 2) - text_x;
    text_y = display.height() - (window_height - text_height) / 2;
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(text_x, text_y);
        display.print(output);
    }
    while (display.nextPage());
}

static void ft_draw_cluster_slide(const unsigned char* output)                      // with display refresh: flickers, does NOT invert colours while running 
{                                                                                   // this function only draws the cluster slides
    display.setFullWindow();
    display.setRotation(1);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, cluster_number_img, 631, 480, GxEPD_BLACK);
        if (output)
            display.drawBitmap(631, 0, output, 169, 480, GxEPD_BLACK);
    }
    while (display.nextPage()); 
}

void  ft_display_cluster_number(uint8_t mode)
{
    switch (mode)
    {
        case DEFAULT:
            ft_draw_cluster_slide(default_cluster_icons);
            break;
        case INTRA_ERROR:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("COULD NOT CONNECT TO INTRA. CHECK EXAMS TIME YOURSELF");
            break;
        case EXAM_DAY:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("THE CLUSTER WILL BE RESERVED FOR AN EXAM TODAY AT " + 
                        String(rtc_g.exam_start_hour) + ":" + String(rtc_g.exam_start_minutes));
            break;
        case LOW_BATTERY:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("LOW BATTERY!");
            break;
        case OTA_WAITING:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("WAITING FOR OTA UPDATE");
            break;
        case OTA_UPDATING:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("OTA UPDATE IN PROGRESS...");
            break;
        case OTA_SUCCESS:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("OTA UPDATE SUCCESS");
            break;
        case OTA_FAIL:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("OTA UPDATE FAIL");
            break;
        case OTA_CANCELED:
            ft_draw_cluster_slide(NULL);
            ft_draw_text("OTA UPDATE CANCELED");
            break;
    }
}

bool   ft_clear_display(bool errase_display)                          // flickers
{
    if (errase_display)
    {
        display.clearScreen();
        display.writeScreenBuffer();
        errase_display = false;
    }
    return (errase_display);
}

void   ft_display_init(void)
{
    SPI.end();
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    display.init(115200);
}
 
