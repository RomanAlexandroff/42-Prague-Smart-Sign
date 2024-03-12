
#include "42-Prague-Smart-Sign.h"

/*
 *  We need to be able to create a total number of 12 different slides. For that we have 
 *  6 bitmaps and system fonts. The bitmaps are fixed images that can be placed staticaly 
 *  anywhere on the display. With the system fornts we can write any phrases and texts.
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



void  ft_display_cluster_number(uint8_t mode)
{
    switch (mode)
    {
        case DEFAULT:

            break;
        case INTRA_ERROR:

            break;
        case EXAM_DAY:

            break;
        case LOW_BATTERY:

            break;
        case OTA_WAITING:

            break;
        case OTA_UPDATING:

            break;
        case OTA_SUCCESS:

            break;
        case OTA_FAIL:

            break;
        case OTA_CANCELED:

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
 
