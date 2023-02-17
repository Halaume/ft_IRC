/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/17 11:14:31 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>


#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"

std::vector<unsigned char> numeric_response(int num_code, Command cmd)//std::vector<unsigned char> param)
{
	switch (num_code)
	{
		case ERR_NONICKNAMEGIVEN:
		{
			return (ERR_NONICKNAMEGIVENmsg(ERR_NONICKNAMEGIVEN));
		}
		case ERR_ERRONEUSNICKNAME:
		{
			return (ERR_ERRONEUSNICKNAMEmsg(ERR_ERRONEUSNICKNAME, cmd.getCmdUser()->getClient(), cmd.getCmdUser()->getNick()));
		}
		case ERR_NICKNAMEINUSE:
		{
			return (ERR_NICKNAMEINUSEmsg(ERR_NICKNAMEINUSE, cmd.getCmdUser()->getClient(), cmd.getCmdUser()->getNick()));
		}
		case ERR_NEEDMOREPARAMS:
		{
			return (ERR_NEEDMOREPARAMSmsg(ERR_NEEDMOREPARAMS, cmd.getCmdUser()->getClient(), cmd.getParsedCmd()[0]));
		}
		case ERR_ALREADYREGISTERED:
		{
			return (ERR_ALREADYREGISTEREDmsg(ERR_ALREADYREGISTERED, cmd.getCmdUser()->getClient()));
		}
		case ERR_PASSWDMISMATCH:
		{
			return (ERR_PASSWDMISMATCHmsg(ERR_PASSWDMISMATCH, cmd.getCmdUser()->getClient()));
		}
	}

	// to clean
	return (ERR_NEEDMOREPARAMSmsg(ERR_NEEDMOREPARAMS, cmd.getCmdUser()->getClient(), cmd.getParsedCmd()[0]));
}