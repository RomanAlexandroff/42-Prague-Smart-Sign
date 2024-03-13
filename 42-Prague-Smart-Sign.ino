
#include "42-Prague-Smart-Sign.h"

void  setup(void)                                   // Настраиваем всё, что нужно при любом включении
{
    #ifdef DEBUG
        Serial.begin(115200);
        ft_delay(50);
        DEBUG_PRINTF("\n\nDEVICE START\nversion %f\n", float(SOFTWARE_VERSION));
    #endif
    ft_eeprom_init();                               // активизируем ячейку памяти для данных состояния аккумулятора
    ft_battery_init();                              // активируем измерение заряда аккумулятора
    ft_display_init();                              // активируем связь с дисплеем
    ft_power_down_recovery();                       // проверяем от чего проснулись и реагируем соответственно
    ft_battery_check();                             // проверяем актуальный заряд аккумулятора
    ft_buttons_init();                              // активируем кнопки управления
}

static void  ft_pathfinder(void)                    // Решаем в каком режиме будем работать: Экзамен или Номер Кластера
{
    unsigned int  sleep_length;

    if (rtc_g.exam_state)
        ft_exam_mode(&sleep_length);                // режим Экзамен отвечает за отрезок времени от "1 час до начала экзамена" до "конец экзамена"
    else
        ft_cluster_number_mode(&sleep_length);      // за всё остальное время отвечает режим Номер Кластера
    ft_go_to_sleep(sleep_length);                   // Каким бы путём ни прошла программа за время своей работы, она всегда окончится ЗДЕСЬ
    DEBUG_PRINTF("  ---- This message will never be printed out", "");
}

void  loop(void)
{
    ft_pathfinder();
}
 
