/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:01:36 by raleksan          #+#    #+#             */
/*   Updated: 2024/04/09 13:01:38 by raleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globals.h"

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(SPI_SS_PIN, DC_PIN, RST_PIN, BUSY_PIN));
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

RTC_DATA_ATTR struct rtc_global_variables rtc_g;
 
