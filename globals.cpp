/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:36 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:38 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globals.h"

#if defined(ESP32_ORIGINAL) || defined(ESP32_S2)
    GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT/2> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN));  //reduced display buffer
#else //ESP32_C3 || XIAO_ESP32C3
    GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN));    //full display buffer
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

RTC_DATA_ATTR struct RTC_GlobalsManagement rtc_g;

struct GlobalsManagement g = {
    .battery = 0
};
 
