/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 21:13:10 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <vector>
#include <string>
#include "../inc/utils.hpp" 
#include "../inc/Command.hpp"

void push_to_buf(int error, Command &cmd, const std::vector<unsigned char> &param);
void push_to_buf(int code, User *user, const std::vector<unsigned char> &param);

# define RPL_WELCOMEmsg(code, client)                           concat_resp(code, client,                   to_vector(" :Welcome to gim irc\r\n"))
# define RPL_YOURHOSTmsg(code, client, server_name)             concat_resp(code, client,                   to_vector(" :Your host is " + server_name + ", running version 1.0\r\n"))
# define RPL_CREATEDmsg(code, client, date_and_time)            concat_resp(code, client,                   to_vector(" :This server creation date and time is " + date_and_time +"\r\n"))
# define RPL_MYINFOmsg(code, client, server_name)               concat_resp(code, client,                   to_vector(" " + server_name + " 1.0\r\n")) // add user modes and channel modes
# define RPL_UMODEISmsg(code, nick, mode)                       concat_resp(code, nick, mode)
# define RPL_YOUREOPERmsg(code, client)                         concat_resp(code, client,                   to_vector(" :You are now an IRC operator\r\n"))

# define RPL_CHANNELMODEISmsg(code, nick, chan_modes)	        concat_resp(code, nick, chan_modes)
# define RPL_TOPICmsg(code, nick, channel, topic)               concat_resp(code, nick, channel,            to_vector(" :" + to_string(topic) + "\r\n"))
# define RPL_INVITINGmsg(code, nick, nick2, chan)     	        concat_resp(code, nick, nick2, chan)
# define RPL_NAMREPLYmsg(code, nick, all_the_rest)              concat_resp(code, nick, all_the_rest)
# define RPL_ENDOFNAMESmsg(code, nick, channel)                 concat_resp(code, nick, channel,            to_vector(" :End of /NAMES list\r\n"))
# define RPL_BANLISTmsg(code, all)						        concat_resp(code, all)
# define RPL_ENDOFBANLISTmsg(code, channel)				        concat_resp(code, channel,				    to_vector(" :End of channel ban list\r\n"))
# define RPL_YOUREOPERmsg(code, client)                         concat_resp(code, client,                   to_vector(" :You are now an IRC operator\r\n"))

# define ERR_NOSUCHCHANNELmsg(code, client, channel)            concat_resp(code, client, channel,          to_vector(" :No such channel\r\n"))
# define ERR_NOSUCHNICKmsg(code, client, nick)				    concat_resp(code, client, nick,             to_vector(" :No such nick/channel\r\n"))
# define ERR_TOOMANYCHANNELSmsg(code, client, channel)          concat_resp(code, client, channel,          to_vector(" :You have joined too many channels\r\n"))
# define ERR_NONICKNAMEGIVENmsg(code)                           concat_resp(code,                           to_vector(" :No nickname given\r\n"))
# define ERR_ERRONEUSNICKNAMEmsg(code, client, nick)            concat_resp(code, client, nick,             to_vector(" :Erroneus nickname\r\n"))
# define ERR_NICKNAMEINUSEmsg(code, nick)                       concat_resp(code, nick, nick,               to_vector(" :Nickname is already in use\r\n"))
# define ERR_USERNOTINCHANNELmsg(code, client, nick, channel)   concat_resp(code, client, nick, channel,    to_vector(" :They aren't on that channel\r\n"))
# define ERR_NOTONCHANNELmsg(code, client, channel)             concat_resp(code, client, channel,          to_vector(" :You're not on that channel\r\n"))
# define ERR_USERONCHANNELmsg(code, client, nick, channel)      concat_resp(code, client, nick, channel,    to_vector(" :is already on channel\r\n"))
# define ERR_NEEDMOREPARAMSmsg(code, client, cmd)               concat_resp(code, client, cmd,              to_vector(" :Not enough parameters\r\n"))
# define ERR_ALREADYREGISTEREDmsg(code, client)                 concat_resp(code, client,                   to_vector(" :You may not reregister\r\n"))
# define ERR_PASSWDMISMATCHmsg(code, client)                    concat_resp(code, client,                   to_vector(" :Password incorrect\r\n"))
   
# define ERR_CHANNELISFULLmsg(code, client, channel)            concat_resp(code, client, channel,          to_vector(" :Cannot join channel (+l)\r\n"))
# define ERR_UNKNOWNMODEmsg(code, nick, modechar)               concat_resp(code, nick, modechar,           to_vector(" :is unknown mode char to me\r\n"))
# define ERR_INVITEONLYCHANmsg(code, client, channel)           concat_resp(code, client, channel,          to_vector(" :Cannot join channel (+i)\r\n"))
# define ERR_BANNEDFROMCHANmsg(code, client, channel)           concat_resp(code, client, channel,          to_vector(" :Cannot join channel (+b)\r\n"))
# define ERR_BADCHANNELKEYmsg(code, client, channel)            concat_resp(code, client, channel,          to_vector(" :Cannot join channel (+k)\r\n"))
# define ERR_BADCHANMASKmsg(code, channel)                      concat_resp(code, channel,                  to_vector(" :Bad Channel Mask\r\n")) // add client?

# define ERR_NOPRIVILEGESmsg(code, nick)                        concat_resp(code, nick,                     to_vector(" :Permission Denied- You're not an IRC operator\r\n"))
# define ERR_CHANOPRIVSNEEDEDmsg(code, nick, channel)           concat_resp(code, nick, channel,            to_vector(" :You're not channel operator\r\n"))
# define ERR_NOOPERHOSTmsg(code, nick)                          concat_resp(code, nick,                     to_vector(" :No O-lines for your host\r\n"))
# define ERR_UMODEUNKNOWNFLAGmsg(code, nick)   			        concat_resp(code, nick,					    to_vector(" :Unknown MODE flag\r\n"))
# define ERR_USERSDONTMATCHmsg(code, client)                    concat_resp(code, client,                   to_vector(" :Cant change mode for other users\r\n"))
# define ERR_INVALIDKEYmsg(code, chan)                          concat_resp(code, chan,                     to_vector(" :Key is wrong\r\n"))
# define OWN_NICK_RPLmsg(nick, user_name, mask, nickold)        concat_nick_rpl(nick, user_name, mask, nickold)
# define JOINED_CHANNELmsg(client, channel)                     concat_resp(client, to_vector("JOIN"), channel)
# define MODE_MESSAGEmsg(mode, all)                             concat_resp(mode, all)

enum numerics {
    
    RPL_WELCOME             = 1,
    RPL_YOURHOST            = 2,
    RPL_CREATED             = 3,
    RPL_MYINFO              = 4,
    
    RPL_UMODEIS             = 221,
        
	RPL_CHANNELMODEIS		= 324,
    RPL_TOPIC               = 332,
    RPL_INVITING            = 341,
    RPL_NAMREPLY            = 353,
    RPL_ENDOFNAMES          = 366,
	RPL_BANLIST				= 367,
	RPL_ENDOFBANLIST		= 368,
    RPL_YOUREOPER           = 381,
    
    ERR_NOSUCHNICK          = 401, 
    ERR_NOSUCHCHANNEL       = 403,
    ERR_TOOMANYCHANNELS     = 405,
    ERR_NONICKNAMEGIVEN     = 431,
    ERR_ERRONEUSNICKNAME    = 432,
    ERR_NICKNAMEINUSE       = 433,
    ERR_USERNOTINCHANNEL    = 441,
    ERR_NOTONCHANNEL        = 442,
    ERR_USERONCHANNEL       = 443,
	ERR_NEEDMOREPARAMS      = 461,
    ERR_ALREADYREGISTERED   = 462,
    ERR_PASSWDMISMATCH      = 464, 
    ERR_CHANNELISFULL       = 471,
    ERR_UNKNOWNMODE         = 472,
    ERR_INVITEONLYCHAN      = 473,
    ERR_BANNEDFROMCHAN      = 474,
    ERR_BADCHANNELKEY       = 475,
    ERR_BADCHANMASK         = 476,
    ERR_NOPRIVILEGES        = 481,
    ERR_CHANOPRIVSNEEDED    = 482,
    ERR_NOOPERHOST          = 491,
    
	ERR_UMODEUNKNOWNFLAG	= 501,
    ERR_USERSDONTMATCH      = 502,
    ERR_INVALIDKEY          = 525,
    
    OWN_NICK_RPL            = 1000,
    JOINED_CHANNEL          = 1001,
    MODE_MESSAGE            = 1002
};

std::vector<unsigned char>      numeric_response(int, Command, std::string, std::vector<unsigned char>);
std::vector<unsigned char>      numeric_response(int, User*, std::vector<unsigned char>);

#endif
