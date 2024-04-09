
#include "42-Prague-Smart-Sign.h"

static unsigned int ft_exam(bool* p_preexam_check)
{
    int           minutes;
    unsigned int  exam_remaining_time;

    minutes = 60;
    exam_remaining_time = ft_time_till_event(rtc_g.exam_end_hour, rtc_g.exam_end_minutes);   // проверяем сколько времени (миллисекунд) осталось до конца
    ft_draw_colour_bitmap(exam_warning_black, exam_warning_red);                             // Показываем предупреждение Экзамен
    if (exam_remaining_time > 3600000)                                                       // Если до окончания экзамена больше 1 часа, то...
    {
//        ft_display_timer_exact_end_time();                                            // ...показывем точное время завершения и...
        return (exam_remaining_time - 3600000);                                       // ...засыпаем пока до конца экзамена ни останется 1 час.
    }
    else                                                                              // Если до конца экзамена уже меньше 1 часа, то...
    {                                                                                 // вместо точного времени показываем Таймер
        while (minutes > 10)
        {
//            ft_display_timer(minutes);                                        // timer counts down every 10 minutes
            ft_delay(600000);                                                 // nap for 10 minutes
            minutes -= 10;
        }
        while (minutes > 0)
        {
//            ft_display_timer(minutes);                                        // timer counts down every minute
            ft_delay(60000);                                                  // nap for 1 minute
            minutes -= 1;
        }
    }                                                                     // К этому моменту экзамен подошёл к концу
    rtc_g.exam_status = false;                                             // Меняем статус чтобы перейти в режим Номер Кластера
    *p_preexam_check = false;                                             // Снимаем запрет проверки отмены экзамена
    return (10);                                                          // Перезагружаемся, чтобы сразу проверить время следующего экзамена...
}                                                                         // ...уже в режиме Номера Кластера

static void ft_preexam_warning(unsigned int* p_preexam_time)              // Показываем Доэкзаминационное предупреждение
{                                                                         // эта функция никуда не выходит до самого начала экзамена
    int minutes;

    minutes = ft_time_sync(*p_preexam_time);                              // just in case the device wakes up not when expected
    ft_draw_colour_bitmap(preexam_warning_black, preexam_warning_red);
    while (minutes > 10)
    {
//        ft_display_timer(minutes);                                        // timer counts down every 10 minutes
        ft_delay(600000);                                                 // nap for 10 minutes
        minutes -= 10;
    }
    while (minutes > 0)
    {
//        ft_display_timer(minutes);                                        // timer counts down every minute
        ft_delay(60000);                                                  // nap for 1 minute
        minutes -= 1;
    }
    *p_preexam_time = 0;                                                  // чтобы выйдя из функции сразу перейти в режим Экзамена
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
        if (!rtc_g.exam_status)                                            // Если отменили, то выходим в режим Номер Кластера...
            return;
        preexam_check = true;                                             // ...а если не отменили, то продолжаем в режиме Экзамена
    }
    if (!ft_get_time())                                                   // Уточняем актуальное время. Если не получилось, то просто...
        rtc_g.exam_start_hour += 1;                                                             // ...прибавляем 1 час к значению в памяти и надеемся, что не сильно отклонились по времени
    preexam_time = ft_time_till_event(rtc_g.exam_start_hour, rtc_g.exam_start_minutes);         // Проверяем сколько времени (миллисекунд) осталось до экзамена
    if (preexam_time > 4200000)                                                                 // Если время до экзамена больше 70 минут, то...
        preexam_time = 3600000;                                           // ... явно произошла какая-то ошибка. Указываем время экзамена - через 60 минут. 
    if (preexam_time > 600000)                                            // Если время до экзамена больше 10 минут, то...
        ft_preexam_warning(&preexam_time);                                // ...показываем Доэкзаминационное предупреждение
    if (preexam_time <= 600000 && preexam_time >= 25000)                  // Если из-за какой-то ошибки в программе мы проснулись менее 10 минут до экзамена, то...
    {                                                                     // ... уже нет смысла мерцать дисплеем и показывать Доэкзаминационное предупреждение, поэтому...
        ft_delay(preexam_time - 25000);                                   // ...просто вздремнём до времени начала экзамена (с погрешностью на время обновления картинки),...
        preexam_time = 0;                                                 // ...заранее убедившись что после сна уже точно покажем слайд экзамена.
    }
    if (preexam_time < 25000)                                             // Если времени до экзамена уже нет (с погрешностью обновления дисплея), значит экзамен начался...
        *p_sleep_length = ft_exam(&preexam_check);                        // показываем Экзаминационное предупреждение...
}                                                                         // а после экзамена снимаем запрет проверки отмены экзамена
 
