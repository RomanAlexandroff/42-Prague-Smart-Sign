
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_exam(void)                   // counts exact time till the exam
{
    return ((exam_begin - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000));
}

void  ft_exam_mode(unsigned int* p_sleep_length)
{
    unsigned int              preexam_time;
    RTC_DATA_ATTR static bool preexam_check;
                                                        // Мы проснулись за 1 час до экзамена...
    ft_wifi_connect();
    if (!preexam_check)
    {                                                   // На всякий случай мы ещё всего один раз...
        ft_fetch_exams();                               // ...проверим ни отменили ли экзамен за время сна
        if (!rtc_g.exam_state)                          // Если отменили, то выходим в режим Номер Кластера...
            return;
        preexam_check = true;                           // ...а если не отменили, то продолжаем в режиме Экзамена
    }
    ft_get_time();                                      // Уточняем актуальное время
    preexam_time = ft_time_till_exam();                 // Проверяем сколько времени осталось до экзамена
    if (preexam_time > 0)                               // Если время до экзамена ещё есть...
        *p_sleep_length = ft_preexam_warning();         // ...то показываем Доэкзаминационное предупреждение
    else                                                // Если времени до экзамена уже нет, значит экзамен начался...
        *p_sleep_length = ft_exam(&preexam_check);      // показываем Экзаминационное предупреждение...
}                                                       // а после экзамена снимаем запрет проверки отмены экзамена
 
