
#include "42-Prague-Smart-Sign.h"

static void  setup(void)
{
    #ifdef DEBUG
        Serial.begin(115200);
        ft_delay(50);
        DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
    #endif
    ft_eeprom_init();                               // активизируем ячейку памяти для данных состояния аккумулятора
    ft_battery_init();                              // активируем измерение заряда аккумулятора
    ft_display_init();                              // активируем связь с дисплеем
    ft_power_down_recovery();                       // проверяем от чего проснулись и реагируем
    ft_battery_check();                             // проверяем актуальный заряд аккумулятора
    ft_buttons_init();                              // активируем кнопки управления
}

static void  ft_pathfinder(void)                    // решаем в каком режиме будем работать: Экзамен или Номер Кластера
{
    unsigned int  sleep_length;

    if (rtc_g.exam_state)
        ft_exam_mode(&sleep_length);                // режим Экзамен отвечает только за отрезок времени 1 час до начала экзамена и до конца экзамена
    else
        ft_cluster_number_mode(&sleep_length);      // за всё остальное время отвечает режим Номер Кластера
    ft_go_to_sleep(sleep_length);
    DEBUG_PRINTF("This message will never be printed out", "");
}

void  loop(void) { /* NOTHING IS HERE */ }
 
