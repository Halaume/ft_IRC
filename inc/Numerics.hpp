/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 15:45:46 by iguscett         ###   ########.fr       */
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


// pimp intro message 
// add nick of user to RPL_WELCOME?
# define RPL_WELCOMEmsg(code, nick)                   	concat_resp(code, nick,               	to_vector(" :Welcome to gim irc\r\n"))
# define RPL_YOURHOSTmsg(code, nick, server_name)     	concat_resp(code, nick,               	to_vector(" :Your host is " + server_name + ", running version 1.0\r\n"))
# define RPL_CREATEDmsg(code, nick, date_and_time)    	concat_resp(code, nick,               	to_vector(" :This server creation date and time is " + date_and_time +"\r\n"))
# define RPL_MYINFOmsg(code, nick, server_name)       	concat_resp(code, nick,               	to_vector(" :" + server_name + " 1.0\r\n")) // add user modes and chan modes

# define RPL_UMODEISmsg(code, nick, usermodes)			concat_resp(code, nick, usermodes)

# define RPL_CHANNELMODEISmsg(code, nick, chan_modes)	concat_resp(code, nick, chan_modes)
# define RPL_TOPICmsg(code, nick, chan_topic)         	concat_resp(code, nick, chan_topic)
# define RPL_INVITINGmsg(code, nick, nick2, chan)     	concat_resp(code, nick, nick2, chan)
# define RPL_NAMREPLYmsg(code, nick, all_the_rest)    	concat_resp(code, nick, all_the_rest)
# define RPL_ENDOFNAMESmsg(code, nick, chan)          	concat_resp(code, nick, chan,         	to_vector(" :End of /NAMES list\r\n"))
# define RPL_BANLISTmsg(code, all)						concat_resp(code, all)
# define RPL_ENDOFBANLISTmsg(code, channel)				concat_resp(code, channel,				to_vector(" :End of channel ban list\r\n"))
# define RPL_YOUREOPERmsg(code, client)                 concat_resp(code, client,               to_vector(" :You are now an IRC operator\r\n"))


# define ERR_NOSUCHNICKmsg(code, nick, no_such_nick)  	concat_resp(code, nick, no_such_nick, 	to_vector(" :No such nick\r\n"))
# define ERR_NOSUCHCHANNELmsg(code, nick, chan)        	concat_resp(code, nick, chan,         	to_vector(" :No such chan\r\n"))
# define ERR_TOOMANYCHANNELSmsg(code, nick, chan)    	concat_resp(code, nick, chan,         	to_vector(" :You have joined too many chans\r\n"))
# define ERR_NONICKNAMEGIVENmsg(code, nick)           	concat_resp(code, nick,               	to_vector(" :No nickname given\r\n"))
# define ERR_ERRONEUSNICKNAMEmsg(code, nick, errnick) 	concat_resp(code, nick, errnick,      	to_vector(" :Erroneus nickname\r\n"))
# define ERR_NICKNAMEINUSEmsg(code, nick)             	concat_resp(code, nick, nick,         	to_vector(" :Nickname is already in use\r\n"))
# define ERR_NOTONCHANNELmsg(code, client, chan)		concat_resp(code, client, chan,       	to_vector(" :You're not on that chan\r\n"))
# define ERR_USERONCHANNELmsg(code, client, nick, chan) concat_resp(code, client, nick, chan, 	to_vector(" :is already on chan\r\n"))
# define ERR_NEEDMOREPARAMSmsg(code, nick, cmd)       	concat_resp(code, nick, cmd,			to_vector(" :Not enough parameters\r\n"))
# define ERR_ALREADYREGISTEREDmsg(code, nick)         	concat_resp(code, nick,					to_vector(" :You may not reregister\r\n"))
# define ERR_PASSWDMISMATCHmsg(code, nick)            	concat_resp(code, nick,					to_vector(" :Password incorrect\r\n"))
	
# define ERR_CHANNELISFULLmsg(code, nick, chan)         concat_resp(code, nick, chan,			to_vector(" :Cannot join chan (+l)\r\n"))
# define ERR_UNKNOWNMODEmsg(code, nick, modechar)       concat_resp(code, nick, modechar,       to_vector(" :is unknown mode char to me\r\n"))
# define ERR_INVITEONLYCHANmsg(code, nick, chan)      	concat_resp(code, nick, chan,			to_vector(" :Cannot join chan (+i)\r\n"))
# define ERR_BANNEDFROMCHANmsg(code, nick, chan)      	concat_resp(code, nick, chan,			to_vector(" :Cannot join chan (+b)\r\n"))
# define ERR_BADCHANNELKEYmsg(code, nick, chan)         concat_resp(code, nick, chan,			to_vector(" :Cannot join chan (+k)\r\n"))
# define ERR_BADCHANMASKmsg(code, chan)               	concat_resp(code, chan,					to_vector(" :Bad chan Mask\r\n")) // add nick?
# define ERR_CHANOPRIVSNEEDEDmsg(code, nick, chan)    	concat_resp(code, nick, chan,			to_vector(" :You're not channel operator\r\n"))
# define ERR_NOOPERHOSTmsg(code, nick)                  concat_resp(code, nick,                 to_vector(" :No O-lines for your host\r\n"))

# define ERR_UMODEUNKNOWNFLAGmsg(code, nick)   			concat_resp(code, nick,					to_vector(" :Unknown MODE flag\r\n"))
# define ERR_USERSDONTMATCHmsg(code, nick)				concat_resp(code, nick,					to_vector(" :Cant change mode for other users\r\n"))
# define ERR_INVALIDKEYmsg(code, chan)                  concat_resp(code, chan,                 to_vector(" :Key is wrong\r\n"))

// # define OWN_NICK_RPLmsg(nick, user_name, mask, new_nick)   concat_resp(add_to_vector(add_to_vector(add_to_vector(add_to_vector(nick, "!"), user_name), "@"), mask), to_insert("NICK"), add_to_vector(new_nick, "\r\n"))
# define OWN_NICK_RPLmsg(nick, user_name, mask, nickold)concat_nick_rpl(nick, user_name, mask, nickold)
# define JOINED_CHANNELmsg(client, chan)                concat_resp(client, to_vector("JOIN"), chan)
# define MODE_MESSAGEmsg(mode, all)                     concat_resp(mode, all)

// # define JOINED_chanmsg(client, chan)               concat_resp(client, to_vector("JOIN"), add_to_vector(chan, static_cast<std::string>("\r\n")))
// class Command;

enum numerics {
    RPL_WELCOME             = 1,
    RPL_YOURHOST            = 2,
    RPL_CREATED             = 3,
    RPL_MYINFO              = 4,
    
	RPL_UMODEIS				= 221,

	RPL_CHANNELMODEIS		= 324,
    RPL_TOPIC               = 332,
    RPL_INVITING            = 341,
    RPL_NAMREPLY            = 353,
    RPL_ENDOFNAMES          = 366,
	RPL_BANLIST				= 367,
	RPL_ENDOFBANLIST		= 368,
    RPL_YOUREOPER           = 381,

    
    ERR_NOSUCHNICK          = 401, //////////////// HHHHHHHHHHHERE
    ERR_NOSUCHCHANNEL     	= 403,
    ERR_TOOMANYCHANNELS     = 405,
    ERR_NONICKNAMEGIVEN     = 431,
    ERR_ERRONEUSNICKNAME    = 432,
    ERR_NICKNAMEINUSE       = 433,
    ERR_NOTONCHANNEL       	= 442,
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
    ERR_CHANOPRIVSNEEDED    = 482,
    ERR_NOOPERHOST          = 491,


	ERR_UMODEUNKNOWNFLAG	= 501,
	ERR_USERSDONTMATCH		= 502,
    ERR_INVALIDKEY          = 525,

    OWN_NICK_RPL            = 1000,
    JOINED_CHANNEL          = 1001,
    MODE_MESSAGE            = 1002
};

std::vector<unsigned char>      numeric_response(int, Command, const std::string, std::vector<unsigned char>);
std::vector<unsigned char>      numeric_response(int, User*, std::vector<unsigned char>);

#endif
