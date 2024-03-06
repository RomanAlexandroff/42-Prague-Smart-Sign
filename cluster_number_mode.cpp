
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_wakeup(void)                                    // counts exact time till the next wakeup
{
    if (rtc_g.hour == 18)
        return (12 * 3600000 - (rtc_g.minute * 60000));                           // 12 hours till next wakeup
    if (rtc_g.hour % 3 == 0 && rtc_g.hour >= 6 && rtc_g.hour <= 15)
        return (3 * 3600000 - (rtc_g.minute * 60000));                            // 3 hours till next wakeup
    return (10);                                                                  // the programm will never reach here. This return is here only to make compiler happy.
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)                        // Начинаем работу в режиме Номера Кластера и первым делом проверяем...
{                                                                                 // ...нет ли сегодня экзамена, а если есть, то во сколько часов и минут
    RTC_DATA_ATTR static bool display_cluster;

    ft_wifi_connect();                                                            // Подключаемся к вай-фай...
    ft_get_time();                                                                // ...в Интернете узнаём актуальную дату и время...
    ft_fetch_exams();                                                             // ...в Интре узнаём экзамены на актуальную дату и во сколько начало и конец
    if (rtc_g.exam_state)                                                         // Если мы узнали, что экзамен сегодня есть, то...
    {                                                                                                             // ...добавляем к номеру кластера точное время начала экзамена...
        ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE CLUSTER NUMBER WITH EXAM TIME WARNING BITMAP*/);
        *p_sleep_length = ft_time_till_event(rtc_g.exam_start_hour - 1, rtc_g.exam_start_minutes);                 // ...отсчитываем оставшееся время (миллисекунды) до "1 час до начала экзамена"...
        return;                                                                                                   // ...и засыпаем так, чтобы проснуться ровно за 1 час до начала экзамена
    }                                                                             // После сна мы перейдём в режим Экзамен
    else                                                                          // Если мы узнали, что экзамена сегодня нет, то...
        *p_sleep_length = ft_time_till_wakeup();                                  // ... рассчитываем время (миллисекунды) так, чтобы проспать ровно 3 часа
    if (!display_cluster)                                                         // Если мы не вывели на дисплей номер кластера во время одного из прошлых просыпаний...
    {                                                                             // ...то выводим номер кластера...
        ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE CLUSTER NUMBER BITMAP*/);
        display_cluster = true;                                                   // ...и запрещаем его выводить в следующие разы, чтобы не моргать дисплеем лишний раз
    }                                                                             // Выйдя из этой функции здесь, мы уснём ровно на 3 часа... 
}                                                                                 // ...и после сна снова попадём в режим Номер Кластера для следующей проверки
 
