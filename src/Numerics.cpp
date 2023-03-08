/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 18:13:11 by iguscett         ###   ########.fr       */
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
	
	if (code == RPL_UMODEIS || code == RPL_CHANNELMODEIS)
		add_to_vector(buf, cmd.getCmdUser()->getClient());
	else if (code != OWN_NICK_RPL
		&& code != JOINED_CHANNEL)
		add_to_vector(buf, server_name);
	add_to_vector(buf, numeric_response(code, cmd, server_name, param));
	cmd.getCmdUser()->getRet().insert(cmd.getCmdUser()->getRet().end(), buf.begin(), buf.end());
}

std::vector<unsigned char> numeric_response(int num_code, Command cmd, const std::string server_name, std::vector<unsigned char> param)//std::vector<unsigned char> param)
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
		case RPL_UMODEIS:
		{
			std::vector<unsigned char> modes = cmd.getCmdUser()->getUserModes();
			return (RPL_UMODEISmsg(RPL_UMODEIS, cmd.getCmdUser()->getNick(), modes));
		}
		case RPL_CHANNELMODEIS:
		{
			return (RPL_CHANNELMODEISmsg(RPL_CHANNELMODEIS, cmd.getCmdUser()->getNick(), param));
		}
		case RPL_TOPIC:
		{
			return (RPL_TOPICmsg(RPL_TOPIC, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[1], param));
		}
		case RPL_INVITING:
		{
			std::vector<unsigned char> v;
			v.push_back(' ');
			add_to_v(v, param);
			add_to_vector(v, static_cast<std::string>("\r\n"));
			return (RPL_INVITINGmsg(RPL_INVITING, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[1], v));
		}
		case RPL_NAMREPLY:
		{
			return (RPL_NAMREPLYmsg(RPL_NAMREPLY, cmd.getCmdUser()->getNick(), param));
		}
		case RPL_ENDOFNAMES:
		{
			return (RPL_ENDOFNAMESmsg(RPL_ENDOFNAMES, cmd.getCmdUser()->getNick(), param));
		}
		case RPL_YOUREOPER:
		{
			return (RPL_YOUREOPERmsg(RPL_YOUREOPER, cmd.getCmdUser()->getNick()));
		}
		case ERR_NOSUCHNICK:
		{
			return (ERR_NOSUCHNICKmsg(ERR_NOSUCHNICK, cmd.getCmdUser()->getNick(), param));
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
			return (ERR_NONICKNAMEGIVENmsg(ERR_NONICKNAMEGIVEN));
		}
		case ERR_ERRONEUSNICKNAME:
		{
			return (ERR_ERRONEUSNICKNAMEmsg(ERR_ERRONEUSNICKNAME, cmd.getCmdUser()->getNick(), cmd.getParsedCmd()[1]));
		}
		case ERR_NICKNAMEINUSE:
		{
			return (ERR_NICKNAMEINUSEmsg(ERR_NICKNAMEINUSE, param));
		}
		case ERR_USERNOTINCHANNEL:
		{
			return (ERR_USERNOTINCHANNELmsg(ERR_USERNOTINCHANNEL, cmd.getCmdUser()->getUserName(), cmd.getCmdUser()->getNick(), param));
		}
		case ERR_NOTONCHANNEL:
		{
			return (ERR_NOTONCHANNELmsg(ERR_NOTONCHANNEL, cmd.getCmdUser()->getNick(), param));
		}
		case ERR_USERONCHANNEL:
		{
			return (ERR_USERONCHANNELmsg(ERR_USERONCHANNEL, cmd.getCmdUser()->getUserName(), cmd.getCmdUser()->getNick(), param));
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
		case ERR_NOPRIVILEGES:
		{
			return (ERR_NOPRIVILEGESmsg(ERR_NOPRIVILEGES, cmd.getCmdUser()->getNick()));
		}
		case  ERR_CHANOPRIVSNEEDED:
		{
			return (ERR_CHANOPRIVSNEEDEDmsg(ERR_CHANOPRIVSNEEDED, cmd.getCmdUser()->getNick(), param));	
		}
		case  ERR_NOOPERHOST:
		{
			return (ERR_NOOPERHOSTmsg(ERR_NOOPERHOST, cmd.getCmdUser()->getNick()));
		}
		case ERR_USERSDONTMATCH:
		{
			return (ERR_USERSDONTMATCHmsg(ERR_USERSDONTMATCH, cmd.getCmdUser()->getNick()));
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

void push_to_buf(int code, User *user, const std::vector<unsigned char> param)
{
	std::vector<unsigned char> buf = to_vector(":");
	
	if (code == RPL_UMODEIS
		|| code == RPL_CHANNELMODEIS
		|| code == MODE_MESSAGE)
		add_to_vector(buf, user->getClient());
	else if ((code != OWN_NICK_RPL
		&& code != JOINED_CHANNEL))
		add_to_vector(buf, server_name);
	add_to_vector(buf, numeric_response(code, user, param));
	user->getRet().insert(user->getRet().end(), buf.begin(), buf.end());
}

std::vector<unsigned char> numeric_response(int num_code, User *user, std::vector<unsigned char> param)
{
	switch (num_code)
	{
		case ERR_NOTONCHANNEL:
		{
			return (ERR_NOTONCHANNELmsg(ERR_NOTONCHANNEL, user->getNick(), param));
		}
		case ERR_NOSUCHNICK:
		{
			return (ERR_NOSUCHNICKmsg(ERR_NOSUCHNICK, user->getNick(), param));
		}
		case RPL_CHANNELMODEIS:
		{
			return (RPL_CHANNELMODEISmsg(RPL_CHANNELMODEIS, user->getNick(), param));
		}
		case RPL_BANLIST:
		{
			return (RPL_BANLISTmsg(RPL_BANLIST, param));
		}
		case RPL_ENDOFBANLIST:
		{
			return (RPL_ENDOFBANLISTmsg(RPL_ENDOFBANLIST, param));
		}
		case ERR_UNKNOWNMODE:
		{
			return (ERR_UNKNOWNMODEmsg(ERR_UNKNOWNMODE, user->getNick(), param));
		}
		case ERR_INVALIDKEY:
		{
			return (ERR_INVALIDKEYmsg(num_code, param));
		}
		case MODE_MESSAGE:
		{
			return (MODE_MESSAGEmsg(to_vector(" MODE"), param));
		}
	}
	// to clean
	return (ERR_NOSUCHNICKmsg(ERR_NOSUCHNICK, user->getNick(), param));
}
