/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/04 12:51:02 by ghanquer         ###   ########.fr       */
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

void push_to_buf(int code, Command &cmd, std::vector<unsigned char> param)
{
	std::vector<unsigned char> buf;
	std::string server_name = "mig.42.fr";
	std::string ddots = ":";
	
	if (code == OWN_NICK_RPL || code == JOINED_CHANNEL)
		add_to_vector(buf, ddots);
	else
		add_to_vector(buf, ddots + server_name);
	add_to_vector(buf, numeric_response(code, cmd, server_name, param));
	cmd.getCmdUser()->getRet().insert(cmd.getCmdUser()->getRet().end(), buf.begin(), buf.end());
}

std::vector<unsigned char> numeric_response(int num_code, Command cmd, std::string server_name, std::vector<unsigned char> param)//std::vector<unsigned char> param)
{
	switch (num_code)
	{
		case RPL_WELCOME:
		{
			return (RPL_WELCOMEmsg(RPL_WELCOME, cmd.getCmdUser()->getNick()));
		}
		case RPL_YOURHOST:
		{
			return (RPL_YOURHOSTmsg(RPL_YOURHOST, cmd.getCmdUser()->getNick(), server_name));
		}
		case RPL_CREATED:
		{
			char       time_buf[80];
			strftime(time_buf, sizeof(time_buf), "%Y-%m-%d @ %X", &tstruct);
			std::string date_and_time(time_buf);
			return (RPL_CREATEDmsg(RPL_CREATED, cmd.getCmdUser()->getNick(), date_and_time));
		}
		case RPL_MYINFO:
		{
			return (RPL_MYINFOmsg(RPL_MYINFO, cmd.getCmdUser()->getNick(), server_name));
		}
		case RPL_TOPIC:
		{
			return (RPL_TOPICmsg(RPL_TOPIC, cmd.getCmdUser()->getNick(), param));
		}
		case RPL_NAMREPLY:
		{
			return (RPL_NAMREPLYmsg(RPL_NAMREPLY, cmd.getCmdUser()->getNick(), param));
		}
		case RPL_ENDOFNAMES:
		{
			return (RPL_ENDOFNAMESmsg(RPL_ENDOFNAMES, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_NOSUCHNICK:
		{
			return (ERR_NOSUCHNICKmsg(ERR_NOSUCHNICK, cmd.getCmdUser()->getClient(), param));
		}
		case ERR_NOSUCHCHANNEL:
		{
			return (ERR_NOSUCHCHANNELmsg(ERR_NOSUCHCHANNEL, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_TOOMANYCHANNELS:
		{
			return (ERR_TOOMANYCHANNELSmsg(ERR_TOOMANYCHANNELS, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_NONICKNAMEGIVEN:
		{
			return (ERR_NONICKNAMEGIVENmsg(ERR_NONICKNAMEGIVEN, cmd.getCmdUser()->getNick()));
		}
		case ERR_ERRONEUSNICKNAME:
		{
			return (ERR_ERRONEUSNICKNAMEmsg(ERR_ERRONEUSNICKNAME, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[1]));
		}
		case ERR_NICKNAMEINUSE:
		{
			return (ERR_NICKNAMEINUSEmsg(ERR_NICKNAMEINUSE, param));
		}
		case ERR_NOTONCHANNEL:
		{
			return (ERR_NOTONCHANNELmsg(ERR_NOTONCHANNEL, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_NEEDMOREPARAMS:
		{
			return (ERR_NEEDMOREPARAMSmsg(ERR_NEEDMOREPARAMS, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[0]));
		}
		case ERR_ALREADYREGISTERED:
		{
			return (ERR_ALREADYREGISTEREDmsg(ERR_ALREADYREGISTERED, cmd.getCmdUser()->getNick()));
		}
		case ERR_PASSWDMISMATCH:
		{
			return (ERR_PASSWDMISMATCHmsg(ERR_PASSWDMISMATCH, cmd.getCmdUser()->getNick()));
		}
		case ERR_CHANNELISFULL:
		{
			return (ERR_CHANNELISFULLmsg(ERR_CHANNELISFULL, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_INVITEONLYCHAN:
		{
			return (ERR_INVITEONLYCHANmsg(ERR_INVITEONLYCHAN, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_BANNEDFROMCHAN:
		{
			return (ERR_BANNEDFROMCHANmsg(ERR_BANNEDFROMCHAN, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_BADCHANNELKEY:
		{
			return (ERR_BADCHANNELKEYmsg(ERR_BADCHANNELKEY, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_BADCHANMASK:
		{
			return (ERR_BADCHANMASKmsg(ERR_BADCHANMASK, param));
		}
		case OWN_NICK_RPL:
		{
			return (OWN_NICK_RPLmsg(cmd.getCmdUser()->getNick(), cmd.getCmdUser()->getUserName(),cmd.getCmdUser()->getUserMask(), param));
		}
		case JOINED_CHANNEL:
		{
			std::vector<unsigned char> channel = param;
			add_to_vector(channel, static_cast<std::string>("\r\n"));
			return (JOINED_CHANNELmsg(cmd.getCmdUser()->getClient(), channel));
		}
	}
	// to clean
	return (ERR_NEEDMOREPARAMSmsg(ERR_NEEDMOREPARAMS, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[0]));
}
