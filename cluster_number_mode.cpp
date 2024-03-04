
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_exam(void)                   // counts time till 1 hour before the exam
{                                                              // because there should be 1 hour time for warning
    return ((exam_begin - rtc_g.hour - 1) * 3600000 - (rtc_g.minute * 60000));
}

static unsigned int  ft_time_till_wakeup(void)                 // counts exact time till the next wakeup
{
    if (rtc_g.hour == 18)
        return (12 * 3600000 - (rtc_g.minute * 60000));                     // 12 hours till next wakeup
    if (rtc_g.hour % 3 == 0 && rtc_g.hour >= 6 && rtc_g.hour <= 15)
        return (3 * 3600000 - (rtc_g.minute * 60000));                      // 3 hours till next wakeup
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)
{
    RTC_DATA_ATTR static bool display_cluster;

    ft_wifi_connect();          // по рассписанию в режиме номера кластера И за 1 час до экзамена в режиме экзамена
    ft_get_time();              // по рассписанию в режиме номера кластера И постоянно в режиме экзамена
    ft_fetch_exams();           // по рассписанию в режиме номера кластера И за 1 час до экзамена в режиме экзамена
    if (rtc_g.exam_state)
    {
        ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE CLUSTER NUMBER WITH EXAM TIME WARNING BITMAP*/);
        *p_sleep_length = ft_time_till_exam();
        return;
    }
    else
        *p_sleep_length = ft_time_till_wakeup();
    if (!display_cluster)
    {
        ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE CLUSTER NUMBER BITMAP*/);
        display_cluster = true;
    }
}
