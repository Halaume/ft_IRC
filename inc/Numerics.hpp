/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 17:34:03 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <vector>
#include <string>

#include "../inc/utils.hpp"
#include "../inc/Command.hpp"

# define ERR_NEEDMOREPARAMSmsg(code, client, cmd)         concat_resp(code, client, cmd, to_vector(" :Not enough parameters\r\n"))
# define ERR_ALREADYREGISTEREDmsg(code, client)           concat_resp(code, client,      to_vector(" :You may not reregister\r\n")) 

// class Command;

enum numerics {
	ERR_NEEDMOREPARAMS      = 461,
    ERR_ALREADYREGISTERED   = 462
};

std::vector<unsigned char>      numeric_response(int num_code, Command cmd); //std::vector<unsigned char> param);

#endif
