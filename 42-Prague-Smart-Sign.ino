
#include "42-Prague-Smart-Sign.h"

void  setup(void)                                   // Настраиваем всё, что нужно при любом включении
{
    #ifdef DEBUG
        ft_serial_init();
    #endif
    ft_spiffs_init();
    #if !defined(XIAO_ESP32C3)
        ft_battery_init();                          // активируем измерение заряда аккумулятора
    #endif
    ft_display_init();                              // активируем связь с дисплеем
    ft_power_down_recovery();                       // проверяем от чего проснулись и реагируем соответственно
    #if !defined(XIAO_ESP32C3)
        ft_battery_check();                             // проверяем актуальный заряд аккумулятора
    #endif
//    ft_buttons_init();                              // активируем кнопки управления
}

static void  ft_pathfinder(void)                    // Решаем в каком режиме будем работать: Экзамен или Номер Кластера
{
    unsigned int  sleep_length;

    if (rtc_g.exam_status)
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
 
