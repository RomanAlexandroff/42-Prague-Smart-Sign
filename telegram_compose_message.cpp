/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   telegram_compose_message.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raleksan <r.aleksandroff@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:20:00 by raleksan          #+#    #+#             */
/*   Updated: 2024/10/21 17:20:00 by raleksan         ###   ########.fr       */
/*                                                                            */
/*                                                                            */
/*   These functions are for creating Telegram messages from the Sign to      */
/*   the User.                                                                */
/*                                                                            */
/* ************************************************************************** */

#include "42-Prague-Smart-Sign.h"

static String ft_about_intra_exams(String output)
{
    output += "I am writing to report an exceptional situation! Code: INTRA 404\n\n";
    output += "After several tries, I was unable to get any exam information from the Intra server. ";
    output += "Instead of dates and times of exams, I simply receive an empty page. ";
    output += "If it is caused by a problem on my side, I am unaware of it.\n\n";
    output += "Anyhow, now I cannot inform students about exams, so if there is one today, ";
    output += "you probably should notify them yourself. ";
    output += "I will try to get the exam information in a few hours. ";
    output += "If you do not get this message again, it means that I was successful.";
    return (output);
}

static String ft_about_intra_token(String output)
{
    output += "I am writing to report an exceptional situation! Code: INTRA 401\n\n";
    output += "After several tries, I was unable to get the access token from the Intra server. ";
    output += "From the experience, it has been probably caused by a faulty or expired SECRET token. ";
    output += "Another possible reason is that they changed the Intra server response layout ";
    output += "and I cannot see the access token, because now it is located somewhere else.\n\n";
    output += "Anyhow, now I cannot inform students about exams, so if there is one today, ";
    output += "you probably should notify them yourself. ";
    output += "I will try to get the access token in a few hours. ";
    output += "If you do not get this message again, it means that I was successful.";
    return (output);
}

static String ft_about_intra_server(String output)
{
    output += "I am writing to report an exceptional situation! Code: INTRA 503\n\n";
    output += "After several retries, I was unable to connect to the Intra server. ";
    output += "Since you see this message, the problem is probably not with the Wi-Fi connection, ";
    output += "but on the Intra server's side. Likely, you can hardly do anything but wait. ";
    output += "I cannot inform students about exams, so if there is one today, ";
    output += "you probably should notify them yourself.\n\n";
    output += "I will try to connect to Intra in a few hours. ";
    output += "If you do not get this message again, it means that I was successful.";
    return (output);
}

static String ft_about_dead_battery(String output)
{
    output += "I need your assistance! ";
    output += "My battery is dead, so I am stopping all the processes and turning off. ";
    output += "Please, charge me and I will turn back on again.";
    return (output);
}

static String ft_about_low_battery(String output)
{
    output += "I need your assistance! ";
    output += "My battery is low. I am currently sitting on 3% ";
    output += "and it keeps on draining! Please, charge me as soon as possible.";
    return (output);
}

static String ft_about_expired_secret(String output, int8_t days_left)
{
    output += "I need your assistance! ";
    output += "My SECRET token ";
    if (days_left > 1)
        output += "expires in " + String(days_left) + " days. ";
    if (days_left == 1)
        output += "expires tomorrow! ";
    if (days_left <= 0)
        output += "has already expired!!! ";
    output += "And without it I would not be able to do my job. ";
    output += "Please, retreive a new SECRET token from the Intra API page ";
    output += "and send it to me in this chat. ";
    output += "Your message should start with a forward slash (/), ";
    output += "so I could recognise it.\n\n";
    output += "Here is an example:\n";
    output += "/s-xxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
    output += "After it is done, I will leave you in pease for another month =)";
    return (output);
}

String  ft_compose_message(uint16_t subject, int8_t days_left)
{
    String  output;

    if (rtc_g.from_name.isEmpty())
        rtc_g.from_name = "User";
    output = "Dear " + rtc_g.from_name;
    output += ", ";
    if (subject == SECRET_EXPIRED)
        output = ft_about_expired_secret(output, days_left);
    else if (subject == LOW_BATTERY)
        output = ft_about_low_battery(output);
    else if (subject == DEAD_BATTERY)
        output = ft_about_dead_battery(output);
    else if (subject == INTRA_NO_SERVER)
        output = ft_about_intra_server(output);
    else if (subject == INTRA_NO_TOKEN)
        output = ft_about_intra_token(output);
    else if (subject == INTRA_NO_INFO)
        output = ft_about_intra_exams(output);
    return (output);
}
 
