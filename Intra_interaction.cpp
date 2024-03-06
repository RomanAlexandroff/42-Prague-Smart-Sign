
#include "42-Prague-Smart-Sign.h"


// isEmpty() проверяет String переменные по количеству знаков; если знаков нет, он возвращает true

// indexOf() ищет указанные слова или фразы в Stringе и возвращает индекс первого появления. Если ничего не найдено, возвращает -1.

// substring() извлекает часть строки. Требуется начальный индекс и индекс окончания (исключительно - то есть символ по индексу
// окончания не будет включён в результат). Возвращает новую строку, которая содержит извлеченную часть.

// length() проверяет String переменные по количеству знаков и выдаёт количество знаков в формате int



static uint8_t  ft_number_of_exams(String server_message)                            // Считаем количество экзаменов упомянутых в сообщении сервера
{
    int8_t  i;
    uint8_t count;

    i = -1;
    count = 0;
    while ((i = server_message.indexOf("begin_at", i + 1)) != -1)
        count++;
    return (count);
}

static void  ft_clean_data(String server_message)                                       // Из полученного сообщения сервера вытаскиваем нужную нам информацию
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
    rtc_g.exams_number = ft_number_of_exams(server_message);                             // Считаем количество экзаменов упомянутых в сообщении сервера
    while (client.available())
    {
        server_message = client.readString();                                        // читаем сообщение сервера до самого конца
        if (server_message.startsWith("\"begin_at\":\"")) 
        {
            rtc_g.exam_start_hour = server_message.substring(24, 26).toInt() + TIME_ZONE;
            rtc_g.exam_start_minutes = server_message.substring(27, 29).toInt();
            DEBUG_PRINTF("Exam starts at: ", "");
            DEBUG_PRINTF("%d:", rtc_g.exam_start_hour);
            DEBUG_PRINTF("%d\n", rtc_g.exam_start_minutes);
        }
        if (server_message.startsWith("\"end_at\":\"")) 
        {
            rtc_g.exam_end_hour = server_message.substring(22, 24).toInt() + TIME_ZONE;
            rtc_g.exam_end_minutes = server_message.substring(25, 27).toInt();
            DEBUG_PRINTF("Exam starts at: ", "");
            DEBUG_PRINTF("%d:", rtc_g.exam_end_hour);
            DEBUG_PRINTF("%d\n", rtc_g.exam_end_minutes);
            break;
        }
    }
}

static void  ft_get_data_load(String token, String* p_server_message)                // Запрашиваем и получаем данные экзаменов
{
    String  query;

    query = "/exams?filter[location]=" + String(CLUSTER_ID) + "&range[begin_at]=" +                             // формируем запрос на конкретную информацию...
            String(rtc_g.year) + "-" + String(rtc_g.month) + "-" + String(rtc_g.day) + "T05:00:00.000Z," +      // об экзаменах об экзаменах в конкретном кластере
            String(rtc_g.year) + "-" + String(rtc_g.month) + "-" + String(rtc_g.day) + "T22:00:00.000Z";
    client.print("GET /v2/campus/" + String(CAMPUS_ID) + query + " HTTP/1.1\r\n" +                               // отсылаем запрос
               "Host: api.intra.42.fr\r\n" +
               "Authorization: Bearer " + token + "\r\n" +
               "Connection: close\r\n\r\n");
    DEBUG_PRINTF("Request for Exam Data sent\n", "");
    while (client.connected())
    {
        *p_server_message = client.readString();                                            // прописываем сообщение сервера в переменную server_message
        if (p_server_message->length() > 0)                                                 // убеждаемся что мы получили какой-то ответ
        {
            DEBUG_PRINTF("Response received: \n%s\n\n", p_server_message->c_str());         // выписываем ответ в серийный монитор на случай...
            break;                                                                          // если нужна будет визуальная проверка
        }
    }
}

static void  ft_get_token(String* p_token)                                            // Запрашиваем и получаем токен безопасности
{
    String  data;
    String  server_reply;

    data = "grant_type=client_credentials&client_id=" +
           String(UID) + "&client_secret=" + String(SECRET);                                // формируем запрос о токене серверу
    client.print("POST /oauth/token HTTP/1.1\r\n" +                                         // авторизируемся через oAuth портал сервера
               "Host: api.intra.42.fr\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + String(data.length()) + "\r\n" +
               "Connection: close\r\n\r\n" + data);                                        // отправляем запрос серверу
    DEBUG_PRINTF("Request for an Access Token sent\n", "");
    while (client.connected())
    {
        server_reply = client.readString();                                                 // убеждаемся что мы получили какой-то ответ
        if (server_reply.length() > 0)
        {
            DEBUG_PRINTF("Response received: \n%s\n\n", server_reply.c_str());              // выписываем ответ в серийный монитор на случай...
            break;                                                                          // если нужна будет визуальная проверка
        }
    }
    while (client.available())
    {
        server_reply = client.readString();
        if (server_reply.startsWith("{\"access_token\":\""))                              // ищем в полученном ответе расположение токена 
        {
            *p_token = server_reply.substring(17, 80);                                    // выделяем токен из сообщения
            DEBUG_PRINTF("Access Token: %s\n\n", p_token->c_str());
            break;
        }
    }
}

void  ft_fetch_exams(void)                                              // Получаем данные о экзаменах
{
    String        server_message;
    String        token;

    if (WiFi.status() != WL_CONNECTED)                                  // убеждаемся что мы подключены к вай-фай
        WiFi.reconnect();
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_PRINTF("Failed to obtain Intra data due to Wi-Fi connection issues\n", "");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return;
    }
    if (!client.connect("https://api.intra.42.fr", 443))                // подключаемся к удалённому серверу
    {
        DEBUG_PRINTF("Intra server connection FAILED\n", "");           // ! Здесь нехватает обработки ошибки - сообщение об этом нужно вывести на экран !
        return;
    }
    ft_get_token(&token);                                               // запрашиваем токен безопасности с которым сможем получить данные с сервера
    ft_get_data_load(token, &server_message);                           // запрашиваем сервер о данных экзаменов за сегодняшний день
    ft_clean_data(server_message);                                      // из полученного пакета информации вытаскиваем только то, что нам нужно
    client.stop();                                                      // закрываем клиента безопасного соединения
}                                                                       // выходим в место откуда нас звали
 
