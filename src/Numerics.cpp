/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 21:39:08 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <ctime>
#include <time.h>


#include "../inc/Command.hpp"
#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"

std::vector<unsigned char> numeric_response(int num_code, Command cmd, std::string server_name)//std::vector<unsigned char> param)
{

	// std::cout << "date:" << buf << std::endl;
	
	switch (num_code)
	{
		case RPL_WELCOME:
		{
			return (RPL_WELCOMEmsg(RPL_WELCOME, cmd.getCmdUser()->getClient()));
		}
		case RPL_YOURHOST:
		{
			return (RPL_YOURHOSTmsg(RPL_YOURHOST, cmd.getCmdUser()->getClient(), server_name));
		}
		case RPL_CREATED:
		{
			char       time_buf[80];
			strftime(time_buf, sizeof(time_buf), "%Y-%m-%d @ %X", &tstruct);
			std::string date_and_time(time_buf);
			return (RPL_CREATEDmsg(RPL_CREATED, cmd.getCmdUser()->getClient(), date_and_time));
		}
		case RPL_MYINFO:
		{
			return (RPL_MYINFOmsg(RPL_MYINFO, cmd.getCmdUser()->getClient(), server_name));
		}
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