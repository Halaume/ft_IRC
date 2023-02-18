/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 21:38:56 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <vector>
#include <string>

#include "../inc/utils.hpp" 
#include "../inc/Command.hpp"



// pimp intro message
# define RPL_WELCOMEmsg(code, client)                   concat_resp(code, client,       to_vector(" :Welcome to gim irc\r\n"))
# define RPL_YOURHOSTmsg(code, client, server_name)     concat_resp(code, client,       to_vector(" :Your host is " + server_name + ", running version 1.0\r\n"))
# define RPL_CREATEDmsg(code, client, date_and_time)    concat_resp(code, client,       to_vector(" :This server creation date and time is " + date_and_time +"\r\n"))
# define RPL_MYINFOmsg(code, client, server_name)       concat_resp(code, client,       to_vector(" :" + server_name + " 1.0\r\n")) // add user modes and channel modes

# define ERR_NONICKNAMEGIVENmsg(code)                   concat_resp(code,               to_vector(" :No nickname given\r\n"))
# define ERR_ERRONEUSNICKNAMEmsg(code, client, nick)    concat_resp(code, client, nick, to_vector(" :Erroneus nickname\r\n"))
# define ERR_NICKNAMEINUSEmsg(code, client, nick)       concat_resp(code, client, nick, to_vector(" :Nickname is already in use\r\n"))
# define ERR_NEEDMOREPARAMSmsg(code, client, cmd)       concat_resp(code, client, cmd,  to_vector(" :Not enough parameters\r\n"))
# define ERR_ALREADYREGISTEREDmsg(code, client)         concat_resp(code, client,       to_vector(" :You may not reregister\r\n"))
# define ERR_PASSWDMISMATCHmsg(code, client)            concat_resp(code, client,       to_vector(" :Password incorrect\r\n"))

// class Command;

enum numerics {
    RPL_WELCOME             = 1,
    RPL_YOURHOST            = 2,
    RPL_CREATED             = 3,
    RPL_MYINFO              = 4,
    
    ERR_NONICKNAMEGIVEN     = 431,
    ERR_ERRONEUSNICKNAME    = 432,
    ERR_NICKNAMEINUSE       = 433,
	ERR_NEEDMOREPARAMS      = 461,
    ERR_ALREADYREGISTERED   = 462,
    ERR_PASSWDMISMATCH      = 464
};

std::vector<unsigned char>      numeric_response(int num_code, Command cmd, std::string server); //std::vector<unsigned char> param);

#endif
