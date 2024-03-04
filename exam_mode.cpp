
#include "42-Prague-Smart-Sign.h"

void  ft_exam_mode(unsigned int* p_sleep_length)
{
    ft_wifi_connect();          // по рассписанию в режиме номера кластера И за 1 час до экзамена в режиме экзамена           
    ft_fetch_exams();           // по рассписанию в режиме номера кластера И за 1 час до экзамена в режиме экзамена
}
 
