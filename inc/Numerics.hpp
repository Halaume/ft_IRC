/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/16 18:36:11 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <vector>
#include <string>

#include "../inc/utils.hpp"
#include "../inc/Command.hpp"

# define ERR_NONICKNAMEGIVENmsg(code)                   concat_resp(code,               to_vector(" :No nickname given\r\n"))
# define ERR_NICKNAMEINUSEmsg(code, client, nick)       concat_resp(code, client, nick, to_vector(" :Nickname is already in use\r\n"))
# define ERR_NEEDMOREPARAMSmsg(code, client, cmd)       concat_resp(code, client, cmd,  to_vector(" :Not enough parameters\r\n"))
# define ERR_ALREADYREGISTEREDmsg(code, client)         concat_resp(code, client,       to_vector(" :You may not reregister\r\n"))
# define ERR_PASSWDMISMATCHmsg(code, client)            concat_resp(code, client,       to_vector(" :Password incorrect\r\n"))

// class Command;

enum numerics {
    ERR_NONICKNAMEGIVEN     = 431,
    ERR_NICKNAMEINUSE       = 433,
	ERR_NEEDMOREPARAMS      = 461,
    ERR_ALREADYREGISTERED   = 462,
    ERR_PASSWDMISMATCH      = 464
};

std::vector<unsigned char>      numeric_response(int num_code, Command cmd); //std::vector<unsigned char> param);

#endif
