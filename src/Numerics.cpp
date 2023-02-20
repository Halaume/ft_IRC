/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 21:33:04 by iguscett         ###   ########.fr       */
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

// TODO : if no client name, put nick name
std::vector<unsigned char> push_to_buf(int error, Command &cmd, std::vector<unsigned char> &param)
{
	std::vector<unsigned char> buf, generic_nickname;
	std::string server_name = "mig.42.fr";
	
	generic_nickname.push_back('*');
	// if (my_compare(generic_nickname, _cmd_user->getClient()))
	// {
		add_to_vector(buf, ":" + server_name);
	// }
	// else // to verify
	// {
		// add_to_vector(_cmd_buf, ":");
		// add_to_vector(_cmd_buf, _cmd_user->getClient());
		// add_to_vector(_cmd_buf, "!");
		// add_to_vector(_cmd_buf, _cmd_user->getUserName());
		// add_to_vector(_cmd_buf, "@" + server_name);
	// }
	add_to_vector(buf, numeric_response(error, cmd, server_name, param));

	std::vector<unsigned char>::size_type m;
	std::cout << "2:\n";
	for (m = 0; m < buf.size(); m++)
		std::cout << buf[m];
	std::cout << "\n";

	return (buf);
}

std::vector<unsigned char> numeric_response(int num_code, Command cmd, std::string server_name, std::vector<unsigned char> param)//std::vector<unsigned char> param)
{
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
		case ERR_TOOMANYCHANNELS:
		{
			return (ERR_TOOMANYCHANNELSmsg(ERR_TOOMANYCHANNELS, cmd.getCmdUser()->getClient(), param));
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
		case ERR_BADCHANMASK:
		{
			return (ERR_BADCHANMASKmsg(ERR_BADCHANMASK, param));
		}
	}

	// to clean
	return (ERR_NEEDMOREPARAMSmsg(ERR_NEEDMOREPARAMS, cmd.getCmdUser()->getClient(), cmd.getParsedCmd()[0]));
}