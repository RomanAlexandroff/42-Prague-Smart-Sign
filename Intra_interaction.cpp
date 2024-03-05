
#include "42-Prague-Smart-Sign.h"


// isEmpty() проверяет String переменные по количеству знаков; если знаков нет, он возвращает true

// indexOf() ищет указанные слова или фразы в Stringе и возвращает индекс первого появления. Если ничего не найдено, возвращает -1.

// substring() извлекает часть строки. Требуется начальный индекс и индекс окончания (исключительно - то есть символ по индексу
// окончания не будет включён в результат). Возвращает новую строку, которая содержит извлеченную часть.

// length() проверяет String переменные по количеству знаков и выдаёт количество знаков в формате int



static uint8_t  ft_number_of_exams(String* p_server_message)
{
    int8_t  i;
    uint8_t count;

    i = -1;
    count = 0;
    while ((i = *p_server_message.indexOf("begin_at", i + 1)) != -1)
        count++;
    return (count);
}

static void  ft_clean_data(String server_message, String* p_exam_begin, String* p_exam_end)
{
    int message_length;

    message_length = server_message.length();
    if (!message_length)
    {
        DEBUG_PRINTF("\nError. Server message is empty.\n", "");
        return;
    }
    if (message_length == 2)
    {
        DEBUG_PRINTF("\nAs of now, no exams are planned for today\n", "");
        rtc_g.exam_state = false;
        return;
    }
    rtc_g.exam_state = true;
    rtc_g.exams_number = ft_number_of_exams(&server_message);
    while (client.available())
    {
        server_message = client.readStringUntil('\n');
        if (server_message.startsWith("\"begin_at\":\"")) 
        {
            *p_exam_begin = server_message.substring(24, 28);
            DEBUG_PRINTF("Exam starts at: " + *p_exam_begin);
        }
        if (server_message.startsWith("\"end_at\":\"")) 
        {
            *p_exam_end = server_message.substring(24, 28);
            DEBUG_PRINTF("Exam ends at: " + *p_exam_end);
            break;
        }
        if (server_message.startsWith("}]")) 
        {
            DEBUG_PRINTF("Data search finished");
            break;
        }
    }
}

static void  ft_get_data_load(const String token, String* p_server_message)
{
    String  query;

    query = "/exams?filter[location]=" + CLUSTER_ID + "&range[begin_at]=" + 
            rtc_g.year + "-" + rtc_g.month + "-" + rtc_g.day + "T05:00:00.000Z," +
            rtc_g.year + "-" + rtc_g.month + "-" + rtc_g.day + "T22:00:00.000Z";
    client.print(String("GET /v2/campus/" + CAMPUS_ID + query + " HTTP/1.1\r\n" +
               "Host: api.intra.42.fr\r\n" +
               "Authorization: Bearer " + token + "\r\n" +
               "Connection: close\r\n\r\n"));
    DEBUG_PRINTF("Request for Exam Data sent\n", "");
    while (client.connected())
    {
        *p_server_message = client.readStringUntil('\n');
        if (*p_server_message == "\r")
        {
            DEBUG_PRINTF("Headers received\n", "");
            break;
        }
    }
}

static void  ft_get_token(const String* p_token)                                            // Получаем токен безопасности
{
    String  data;
    String  server_message;

    data = "grant_type=client_credentials&client_id=" + UID + "&client_secret=" + SECRET;   // формируем запрос о токене серверу
    client.print(String("POST /oauth/token HTTP/1.1\r\n" +                                  // авторизируемся через oAuth портал сервера
               "Host: api.intra.42.fr\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + data.length() + "\r\n" +
               "Connection: close\r\n\r\n" + data));                                        // отправляем запрос серверу
    DEBUG_PRINTF("Request for an Access Token sent\n", "");
    while (client.connected())
    {
        server_message = client.readStringUntil('\n');                                      // убеждаемся что мы получили какой-то ответ
        if (server_message == "\r")
        {
            DEBUG_PRINTF("Headers received\n", "");
            break;
        }
    }
    while (client.available())
    {
        server_message = client.readStringUntil('\n');
        if (server_message.startsWith("{\"access_token\":\""))                              // ищем в полученном ответе расположение токена 
        {
            *p_token = server_message.substring(17, 80);                                    // выделяем токен из сообщения
            DEBUG_PRINTF("Access Token: " + *p_token);
            break;
        }
    }
}

void  ft_fetch_exams(void)                                              // Получаем данные о экзаменах
{
    String        server_message;
    const String  token;
    String        exam_begin;
    String        exam_end;

    if (WiFi.status() != WL_CONNECTED)                                  // убеждаемся что мы подключены к вай-фай
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("Failed to obtain Intra data due to Wi-Fi connection issues\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    WiFiClientSecure client;                                            // запускаем клиента безопасного соединения
    if (!client.connect("https://api.intra.42.fr", 443))                // подключаемся к удалённому серверу
    {
        DEBUG_PRINTF("Intra server connection FAILED\n", "");
        return;
    }
    ft_get_token(&token);                                               // запрашиваем токен безопасности с которым сможем получить данные с сервера
    ft_get_data_load(token, &server_message);                           // запрашиваем сервер о данных экзаменов за сегодняшний день
    ft_clean_data(server_message, &exam_begin, &exam_end);              // из полученного пакета информации вытаскиваем только то, что нам нужно
    client.stop();                                                      // закрываем клиента безопасного соединения
}                                                                       // выходим в место откуда нас звали
 
