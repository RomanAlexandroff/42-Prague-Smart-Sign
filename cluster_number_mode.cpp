
#include "42-Prague-Smart-Sign.h"

static unsigned int  ft_time_till_wakeup(void)                                                          // counts exact time till the next wakeup
{
    unsigned int  result;

    if (rtc_g.hour == 18)
        result = 12 * 3600000 - (rtc_g.minute * 60000);                                                 // 12 hours till next wakeup
    if (rtc_g.hour % 3 == 0 && rtc_g.hour >= 6 && rtc_g.hour <= 15)
        result = 3 * 3600000 - (rtc_g.minute * 60000);                                                  // 3 hours till next wakeup
    if (result < 10)                                                                                    // нельзя уходить в сон на 0 миллисекунд
        result = 10;
    return (result);                                                                                    // the programm will never reach here. This return is here only to make compiler happy.
}

void  ft_cluster_number_mode(unsigned int* p_sleep_length)                                              // Начинаем работу в режиме Номера Кластера и первым делом проверяем...
{                                                                                                       // ...нет ли сегодня экзамена, а если есть, то во сколько часов и минут
    RTC_DATA_ATTR static bool display_cluster;                                                          // По-умолчанию, компилятор всегда инициализирует static bool переменные значением false

    ft_wifi_connect();                                                                                  // Подключаемся к вай-фай...
    ft_get_time();                                                                                      // ...в Интернете узнаём актуальную дату и время...
    if (!ft_fetch_exams())                                                                              // ...в Интре узнаём экзамены на актуальную дату и во сколько начало и конец
    {
        ft_display_cluster_number(INTRA_ERROR);                                                         // Если ошибка получения данных из Интры, показываем ошибку на дисплее...
        rtc_g.exam_state = false;                                                                       // ...убеждаемся что после перезагрузки снова попадём сюда...
        display_cluster = false;                                                                        // ...разрешаем вывести дефолтный номер кластера, если ошибка исправится и экзамена не будет...
        *p_sleep_length = 1800000;                                                                      // ...настраиваем таймер сна чтобы проснуться через 30 минут и попробовать опять...
        return;                                                                                         // ...досрочно выходим чтобы позже попробовать снова
    }
    if (rtc_g.exam_state)                                                                               // Если мы узнали, что экзамен сегодня есть, то...
    {                                                                                                   // ...добавляем к номеру кластера точное время начала экзамена...
        ft_display_cluster_number(EXAM_DAY);
        display_cluster = false;                                                                        // ...разрешаем вывести дефолтный номер кластера после экзамена...
        *p_sleep_length = ft_time_till_event(rtc_g.exam_start_hour - 1, rtc_g.exam_start_minutes);      // ...отсчитываем оставшееся время (миллисекунды) до "1 час до начала экзамена"...
        return;                                                                                         // ...и засыпаем так, чтобы проснуться ровно за 1 час до начала экзамена
    }                                                                                                   // После сна мы перейдём в режим Экзамен
    else                                                                                                // Если мы узнали, что экзамена сегодня нет, то...
        *p_sleep_length = ft_time_till_wakeup();                                                        // ... рассчитываем время (миллисекунды) так, чтобы проспать ровно 3 часа
    if (g.battery <= 15)                                                                                // Если уровень заряда аккумулятора слишком низок, то...
    {
        ft_display_cluster_number(LOW_BATTERY);                                                         // ...не показываем дефолтные иконки, а показываем предупреждение о низком заряде. Затем... 
        display_cluster = false;                                                                        // ...разрешаем вывести дефолтный номер кластера, когда аккумулятор подзарядят...
        return;                                                                                         // ...И не страшно, что он будет мерцать при обновлении - привлечёт больше внимания.
    }
    if (!display_cluster)                                                                               // Если мы НЕ вывели на дисплей номер кластера во время одного из прошлых просыпаний...
    {
        ft_display_cluster_number(DEFAULT);                                                             // ...то выводим номер кластера...
        display_cluster = true;                                                                         // ...и запрещаем его выводить в следующие разы, чтобы не моргать дисплеем лишний раз
    }                                                                                                   // Выйдя из этой функции здесь, мы уснём ровно на 3 часа... 
}                                                                                                       // ...и после сна снова попадём в режим Номер Кластера для следующей проверки
 
