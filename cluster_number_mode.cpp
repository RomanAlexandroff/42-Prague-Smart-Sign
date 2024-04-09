
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_wakeup(void)                                                          // counts exact time till the next wakeup
{
    const uint8_t wakeup_hour[] = {6, 9, 12, 15, 18};                                                   // Array to store the hours to wake up and check exams status
    uint8_t       i;

    if (rtc_g.hour >= 18)                                                                               // if need to sleep over midnight, this is how to handle it
        return ((wakeup_hour[0] + 24 - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000));
    i = 0;    
    while ((wakeup_hour[i] - rtc_g.hour) <= 0)                                                          // in all other cases we can simply look up for the next hour to wake up
        i++;
    return ((wakeup_hour[i] - rtc_g.hour) * 3600000 - (rtc_g.minute * 60000));
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)                                              // Начинаем работу в режиме Номера Кластера и первым делом проверяем...
{                                                                                                       // ...нет ли сегодня экзамена, а если есть, то во сколько часов и минут
    uint8_t i;
    bool    intra_connected;
    int8_t  days_left;

    i = 0;
    intra_connected = false;
    ft_wifi_connect();                                                                                  // Подключаемся к вай-фай и...
    if (!ft_get_time())                                                                                 // ...в интернете узнаём актуальную дату и время...
    {                                                                                                   // ...если не получилось узнать актуальную дату и время, то...
        *p_sleep_length = 60000;                                                                        // ...настраиваем уход в сон на 1 минут, чтобы проснувшись поробовать опять.
        rtc_g.exam_status = false;                                                                      // Убеждаемся что после сна снова попадём сюда, чтобы пропробовать узнать актуальную дату и время
        DEBUG_PRINTF("Retrying in 1 minute\n", "");
        return;                                                                                         // БЕЗ ТОЧНОГО ВРЕМЕНИ И ДАТЫ РАБОТА УСТРОЙСТВА НЕВОЗМОЖНА, поэтому пробуем их получить пока аккумулятор ни сядет
    }
    while (!intra_connected && i < 3)                                                                   // В Интре узнаём экзамены на актуальную дату и во сколько начало и конец экзамена
    {                                                                                                   // если не получится подключиться сразу, то есть ещё две попытки (суммарно 3 попытки подключения)
        delay (i * 300000);                                                                             // первая попытка произайдёт сразу, вторая - через 5 минут, третья - через 10 минут
        DEBUG_PRINTF("Fetching exams data — try #%d\n\n", i + 1);
        intra_connected = ft_fetch_exams();                                                             // как только подтвердится успешное подключение к Интре, продолжаем работу программы
        i++;
        if (!intra_connected)
            DEBUG_PRINTF("\nRetrying in %d minutes\n\n", i * 5);
    }
    if (!intra_connected && rtc_g.hour <= 18)                                                           // Если после 3 попыток не удалось подключиться к Интре, то это нужно показать на дисплее, но...
    {                                                                                                   // если эта ошибка случилась после 18:59, то это уже проблема завтрашнего дня
        ft_display_cluster_number(INTRA_ERROR);                                                         // Показываем ошибку на дисплее...
        rtc_g.exam_status = false;                                                                      // ...убеждаемся что после сна снова попадём сюда и повторим попытки подключения.
    }
    ft_telegram_check();
    if (rtc_g.exam_status)                                                                               // Если мы успешно подключились к Интре и узнали, что экзамен сегодня есть, то...
    {
        if (rtc_g.hour >= (rtc_g.exam_start_hour - 1) && rtc_g.hour <= rtc_g.exam_end_hour)             // если оказалось, что уже наступило время экзамена, то
            *p_sleep_length = 10;                                                                       // без промедления перезагружаемся в режим экзамена
        else
        {
            ft_display_cluster_number(EXAM_DAY);                                                            // ...добавляем к номеру кластера точное время начала экзамена...
            *p_sleep_length = ft_time_till_event(rtc_g.exam_start_hour - 1, rtc_g.exam_start_minutes);      // ...отсчитываем оставшееся время (миллисекунды) до "1 час до начала экзамена"...
        }
        return;                                                                                         // ...и засыпаем так, чтобы проснуться ровно за 1 час до начала экзамена
    }                                                                                                   // После сна мы перейдём в режим Экзамен
    else                                                                                                // Если мы узнали, что экзамена сегодня нет, то...
        *p_sleep_length = ft_time_till_wakeup();                                                        // ... рассчитываем время (миллисекунды) так, чтобы проспать ровно 3 часа
    days_left = ft_expiration_counter();
    if (days_left <= 3 && days_left != -128)
    {
        ft_display_cluster_number(SECRET_EXPIRED);
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(SECRET_EXPIRED, days_left), "");
        return;
    }
    if (g.battery <= 15)                                                                                // Если уровень заряда аккумулятора слишком низок, то...
    {
        ft_display_cluster_number(LOW_BATTERY);                                                         // ...показываем предупреждение о низком заряде и...
        bot.sendMessage(rtc_g.chat_id, ft_compose_message(LOW_BATTERY, 0), "");
        return;                                                                                         // ...заранее выходим из фунцкии, чтобы не переписать предупреждение дефолтными иконками
    }
    ft_display_cluster_number(DEFAULT);                                                                 // Если это хороший день без экзаменов и проблем, то к номеру кластера добавляем дефолтные иконки
}                                                                                                       // Выйдя из этой функции здесь, мы уснём ровно на 3 часа (или на 12 если уже вечер)... 
                                                                                                        // ...и после сна снова попадём в режим Номер Кластера для следующей проверки
