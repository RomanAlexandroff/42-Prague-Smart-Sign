
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_exam_end(void)                                    // counts exact time till the exam with 1 minute precision
{
    return ((exam_end - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000));
}

static unsigned int ft_exam(bool* p_preexam_check)
{
    int           minutes;
    unsigned int  exam_remaining_time;

    minutes = 60;
    exam_remaining_time = ft_time_till_exam_end();                                    //проверяем сколько времени (миллисекунд) осталось до конца
    ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE EXAM TIME WARNING BITMAP*/);  // Показываем предупреждение Экзамен
    if (exam_remaining_time > 3600000)                                                // Если до окончания экзамена больше 1 часа, то...
    {
        ft_display_timer_exact_end_time();                                            // ...показывем точное время завершения и...
        return (exam_remaining_time - 3600000);                                       // ...засыпаем пока до конца экзамена ни останется 1 час.
    }
    else                                                                              // Если до конца экзамена уже меньше 1 часа, то...
    {                                                                                 // вместо точного времени показываем Таймер
        while (minutes > 10)
        {
            ft_display_timer(minutes);                                        // timer counts down every 10 minutes
            ft_delay(600000);                                                 // nap for 10 minutes
            minutes -= 10;
        }
        while (minutes > 0)
        {
            ft_display_timer(minutes);                                        // timer counts down every minute
            ft_delay(60000);                                                  // nap for 1 minute
            minutes -= 1;
        }
    }                                                                     // К этому моменту экзамен подошёл к концу
    rtc_g.exam_state = false;                                             // Меняем статус чтобы перейти в режим Номер Кластера
    *p_preexam_check = false;                                             // Снимаем запрет проверки отмены экзамена
    return (10);                                                          // Перезагружаемся, чтобы сразу проверить время следующего экзамена...
}                                                                         // ...уже в режиме Номера Кластера

static void ft_preexam_warning(unsigned int* p_preexam_time)              // Показываем Доэкзаминационное предупреждение
{                                                                         // эта функция никуда не выходит до самого начала экзамена
    int minutes;

    minutes = 60
    ft_display_bitmap_with_refresh(/*HERE SHOULD BE THE PRE-EXAM TIME WARNING BITMAP*/);
    while (minutes > 10)
    {
        ft_display_timer(minutes);                                        // timer counts down every 10 minutes
        ft_delay(600000);                                                 // nap for 10 minutes
        minutes -= 10;
    }
    while (minutes > 0)
    {
        ft_display_timer(minutes);                                        // timer counts down every minute
        ft_delay(60000);                                                  // nap for 1 minute
        minutes -= 1;
    }
    *p_preexam_time = 0;                                                  // чтобы выйдя из функции сразу перейти в режим Экзамена
}

static unsigned int  ft_time_till_exam(void)                              // counts exact time till the exam with 1 minute precision
{
    return ((exam_begin - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000));
}

void  ft_exam_mode(unsigned int* p_sleep_length)
{
    RTC_DATA_ATTR static bool preexam_check;
    unsigned int              preexam_time;
                                                                          // Мы проснулись за 1 час до экзамена...
    ft_wifi_connect();
    if (!preexam_check)
    {                                                                     // На всякий случай мы ещё всего один раз...
        ft_fetch_exams();                                                 // ...проверим ни отменили ли экзамен за время сна
        if (!rtc_g.exam_state)                                            // Если отменили, то выходим в режим Номер Кластера...
            return;
        preexam_check = true;                                             // ...а если не отменили, то продолжаем в режиме Экзамена
    }
    ft_get_time();                                                        // Уточняем актуальное время
    preexam_time = ft_time_till_exam();                                   // Проверяем сколько времени (миллисекунд) осталось до экзамена
    if (preexam_time > 30000)                                             // Если время до экзамена ещё есть...
        ft_preexam_warning(&preexam_time);                                // ...то показываем Доэкзаминационное предупреждение
    if (preexam_time <= 0)                                                // Если времени до экзамена уже нет, значит экзамен начался...
        *p_sleep_length = ft_exam(&preexam_check);                        // показываем Экзаминационное предупреждение...
}                                                                         // а после экзамена снимаем запрет проверки отмены экзамена
 
