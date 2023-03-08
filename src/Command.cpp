/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 17:00:43 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/User.hpp"
#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"



Command::Command(void):  _globalCmd(), _parsedCmd(), _cmd_user(NULL), _cmd_buf(), _error(0)
{
}

Command::Command(const Command &copy): _globalCmd(copy._globalCmd), _parsedCmd(copy._parsedCmd), _cmd_user(copy._cmd_user) \
, _cmd_buf(copy._cmd_buf), _error(copy._error)
{
}

Command::~Command(void)
{
}

Command &	Command::operator=(const Command & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

void	Command::setCommand(std::vector<std::vector<unsigned char> > cmd)
{
	_parsedCmd = cmd;
}
void	Command::setUser(User* usr)
{
	_cmd_user = usr;
}

/*''''''''''''''''''''''''''''''''''''
				Register
''''''''''''''''''''''''''''''''''''''*/
int Command::register_user(Server &my_server)
{
	std::vector<unsigned char> v;
	
	if (my_compare(_cmd_user->getPasswd(), my_server.getPasswd()))
	{
		_cmd_user->setPassBeforeNickUser(PASS_CONNECTION_ERROR);
		return (push_to_buf(ERR_PASSWDMISMATCH, *this, no_param), 1);
	}
	v = _cmd_user->getNick();
	std::list<User>::iterator user = my_server.findUserNick(_cmd_user->getNick());
	if (user->getUserMask() != _cmd_user->getUserMask())
		return (push_to_buf(ERR_NICKNAMEINUSE, *this, v), 1);
	if (my_server.nbConnectionsWithSameNick(*_cmd_user) > 1)
	{
		if (_cmd_user->createNewNick(my_server) == 1)
			return (push_to_buf(ERR_NICKNAMEINUSE, *this, v), 1); // ERROR msg and disconnect
	}
	_cmd_user->setRegistered(true);
	push_to_buf(RPL_WELCOME, *this, no_param);
	push_to_buf(RPL_YOURHOST, *this, no_param);
	push_to_buf(RPL_CREATED, *this, no_param);
	push_to_buf(RPL_MYINFO, *this, no_param);
	return (1);
}

/*''''''''''''''''''''''''''''''''''''
				PASS TP
''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_PASS(void)
{
	std::vector<unsigned char> v;
	
	if ((_parsedCmd.size() < 2 || _parsedCmd[1].empty() == true) && _cmd_user->getRegistered() == false)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	else if (_cmd_user->getRegistered())
		return (push_to_buf(ERR_ALREADYREGISTERED, *this, no_param), 1);
	_cmd_user->setPasswd(_parsedCmd[1]);
	_cmd_user->setPassStatus(PASSWORD_SET);
	if (_cmd_user->getPassBeforeNickUser() == WAITING_FOR_PASS)
		_cmd_user->setPassBeforeNickUser(PASS_ORDER_OK);
	return (0);
}

/*''''''''''''''''''''''''''''''''''''
				NICK
''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_NICK(Server &my_server)
{
	std::vector<unsigned char> ret;
	int to_send = 0;
	
	if (_cmd_user->getPassBeforeNickUser() == WAITING_FOR_PASS)
	{
		_cmd_user->setPassBeforeNickUser(PASS_ORDER_ERROR);
		return (0);
	}
	if (_parsedCmd.size() < 2 || _parsedCmd[1].empty() == true)
		return (push_to_buf(ERR_NONICKNAMEGIVEN, *this, no_param), 1);
	std::list<User>::iterator user = my_server.findUserNick(_parsedCmd[1]);
	if (_cmd_user->isNickValid(_parsedCmd[1]) == false)
		return (push_to_buf(ERR_ERRONEUSNICKNAME, *this, _parsedCmd[1]), 1); 
	else if (user != my_server.getUsersend() && _cmd_user->getRegistered() == true
			&& my_compare(_cmd_user->getNick(), _parsedCmd[1]))
		return (push_to_buf(ERR_NICKNAMEINUSE, *this, _parsedCmd[1]), 1);
	if (_cmd_user->getRegistered() == true)
	{
		push_to_buf(OWN_NICK_RPL, *this, _parsedCmd[1]);
		to_send = 1;
	}
	_cmd_user->setNick(_parsedCmd[1]);
	if (_cmd_user->getPassBeforeNickUser() == PASS_USER_OK)
		return (register_user(my_server));
	else
		_cmd_user->setPassBeforeNickUser(PASS_NICK_OK);
	return (to_send);
}

// /*''''''''''''''''''''''''''''''''''''
// 				USER TODO : quand on se conencte avec un nick deja utilise via telnet un nouveau nick  ca ne se connecte pa autmatiquement
// ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_USER(Server &my_server)
{
	std::vector<unsigned char> ret;
	
	if (_cmd_user->getPassBeforeNickUser() == WAITING_FOR_PASS)
	{
		_cmd_user->setPassBeforeNickUser(PASS_ORDER_ERROR);
		return (0);
	}
	if (_parsedCmd.size() < 5 || _parsedCmd[4].empty() == true)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	if (_cmd_user->getRegistered())
		return (push_to_buf(ERR_ALREADYREGISTERED, *this, no_param), 1);
	else if (_parsedCmd[4].empty() == false && _parsedCmd[4][0] == ':')
	{
		std::vector<unsigned char> real_name = concat_real_name(_globalCmd);
		if (real_name.size() == 0)
			return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);			
		_cmd_user->setUserName(_parsedCmd[1]);
		_cmd_user->setRealName(real_name);
		_cmd_user->setUserMask(_parsedCmd[3]);
		if (_cmd_user->getPassBeforeNickUser() == PASS_NICK_OK)
			return (register_user(my_server));
		else
			_cmd_user->setPassBeforeNickUser(PASS_USER_OK);
	}
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				JOIN
// ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_JOIN(Server &my_server)
{
	(void)my_server;
	std::vector<std::vector<unsigned char> > channels;
	std::vector<std::vector<unsigned char> > keys;
	std::vector<unsigned char> param;
	Channel * channel;

	if (_parsedCmd.size() < 2)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	reparseChannelsKeys(_parsedCmd[1], &channels);
	if (_parsedCmd.size() > 2)
		reparseChannelsKeys(_parsedCmd[2], &keys);
	std::vector<std::vector<unsigned char> >::size_type keys_size = keys.size();
	for (std::vector<std::vector<unsigned char> >::size_type it = 0; it < channels.size(); ++it)
	{
		if (channels[it].empty() == false && ((channels[it][0] != '#' && channels[it][0] != '&') || contains_ctrl_g(channels[it]) == true))
			return (push_to_buf(ERR_BADCHANMASK, *this, channels[it]), 1);
		else if (_cmd_user->getNbChan() >= MAX_NB_CHAN) // verifier si on quitte un channel si on arrive a rerentrer
			return (push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]), 1);
		else if (my_server.channelExists(channels[it]) == false)
		{
			Channel new_channel(channels[it]);
			new_channel.addUser(_cmd_user);
			my_server.addNewChannel(new_channel);
			if (my_server.findChan(channels[it]) == NULL)
				return (0);
			push_to_buf(JOINED_CHANNEL, *this, channels[it]);
			param = rpl_topic(channels[it], my_server.findChan(channels[it])->getTopic());
			push_to_buf(RPL_TOPIC, *this, param);
			param = rpl_name(my_server.findChan(channels[it]));
			push_to_buf(RPL_NAMREPLY, *this, param);
			return (push_to_buf(RPL_ENDOFNAMES, *this, channels[it]), 1);
		}
		else
		{
			channel = my_server.findChan(channels[it]); 
			if (channel == NULL)
				return (push_to_buf(ERR_NOSUCHCHANNEL, *this, channels[it]), 1);
			else if (channel->getMode('k') == true // to verify in irssi
				&& (keys_size == 0 || (keys_size > 0 && keys_size >= it && my_compare(keys[it], channel->getChanPassword()))))
				return (push_to_buf(ERR_BADCHANNELKEY, *this, channels[it]), 1);
			else if (channel->getMode('l') == true // to verify in irssi
				&& channel->getNbUsers() >= channel->getNbUsersLimit())
				return (push_to_buf(ERR_CHANNELISFULL, *this, channels[it]), 1);
			else if (_cmd_user->getNbChan() >= MAX_NB_CHAN) // to verify in irssi
				return (push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]), 1);
			else if (channel->isUserBanned(&(*_cmd_user))) // to verify in irssi
				return (push_to_buf(ERR_BANNEDFROMCHAN, *this, channels[it]), 1);
			else if (channel->getMode('i') == true	// voir pour channel operator // to verify in irssi
				&& (channel->isUserInvited(&(*_cmd_user)) == false
				&& channel->isOp(*_cmd_user) == false))
				return (push_to_buf(ERR_INVITEONLYCHAN, *this, channels[it]), 1);
			if (channel->isUserInChannel(_cmd_user) == false)
				channel->addUser(&(*_cmd_user));
			push_to_buf(JOINED_CHANNEL, *this, channels[it]);
			param = rpl_topic(channels[it], channel->getTopic());
			push_to_buf(RPL_TOPIC, *this, param);
			param = rpl_name(channel);
			push_to_buf(RPL_NAMREPLY, *this, param);
			return (push_to_buf(RPL_ENDOFNAMES, *this, channels[it]), 1);
		}
	}
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				OPER
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_OPER(Server &my_server)
{
	std::vector<unsigned char> adminop = to_vector("admin");
	std::vector<unsigned char> all_modes;
	
	if (_parsedCmd.size() < 3)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	
	if (_parsedCmd[1] != adminop)
		return (push_to_buf(ERR_NOOPERHOST, *this, no_param), 1);
	else 
	{
		std::vector<unsigned char> mode = to_vector(" +o\r\n"); 
		if (_parsedCmd[2] != my_server.getPasswd())
			return (push_to_buf(ERR_PASSWDMISMATCH, *this, no_param), 1);
		else
		{
			_cmd_user->setOperator(true);
			push_to_buf(RPL_YOUREOPER, *this, no_param);
			push_to_buf(RPL_UMODEIS, *this, mode);
			return (1);
		}
	}
	return (0);
}

std::vector<unsigned char> Command::concat_parsedCmd(std::vector<std::vector<unsigned char> >::size_type i)
{
	std::vector<unsigned char> ret;
	
	for (;i < _parsedCmd.size(); i++)
	{
		for (std::vector<unsigned char>::size_type j = 0; j < _parsedCmd[i].size(); j++)
			ret.push_back(_parsedCmd[i][j]);
	}
	return (ret);
}

// /*''''''''''''''''''''''''''''''''''''
// 				MODE
// ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_MODE(Server &my_server)
{
	std::vector<unsigned char> all_modes;
	Channel *channel;

	if (_parsedCmd.size() < 2 || _parsedCmd[1].empty())
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	if (_parsedCmd[1][0] != '#' && _parsedCmd[1][0] != '&'
		&& _cmd_user->isNickValid(_parsedCmd[1]) == true)
	{
		if (my_server.findUserPtrNick(_parsedCmd[1]) == NULL)
			return (push_to_buf(ERR_NOSUCHNICK, *this, _parsedCmd[1]), 1);
		else if (_cmd_user->getNick() != _parsedCmd[1])
			return (push_to_buf(ERR_USERSDONTMATCH, *this, no_param), 1);
		if (_parsedCmd.size() < 3 || _parsedCmd[2].empty())
			return (push_to_buf(RPL_UMODEIS, *this, _cmd_user->getUserModes()), 1);
		all_modes = concat_parsedCmd(2);
		return (_cmd_user->modesMessage(all_modes, true));
	}
	else if (_parsedCmd[1][0] == '#' || _parsedCmd[1][0] == '&')
	{
		channel = my_server.findChan(_parsedCmd[1]);
		if (my_server.findChan(_parsedCmd[1]) == NULL)
			return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[1]), 1);
		if (_parsedCmd.size() < 3 || _parsedCmd[2].empty())
			return (push_to_buf(RPL_CHANNELMODEIS, *this, my_server.findChan(_parsedCmd[1])->getChannelModes()), 1);
		if (_parsedCmd.size() >= 3 && !channel->isOp(_cmd_user) && !_cmd_user->getOperator())
			return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[1]), 1);
		return (channel->modesMessage(my_server, _cmd_user, _parsedCmd, true));
	}
	return (0);
}



// /*''''''''''''''''''''''''''''''''''''
// 				INVITE
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_INVITE(Server &my_server)
{
	if (_parsedCmd.size() < 3)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	std::vector<Channel>::iterator channel =  my_server.findExistingChan(_parsedCmd[2]);
	if (channel == my_server.getChannelsend())
		return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[2]), 1);
	std::list<User *>::iterator	user = channel->findUser(_cmd_user->getNick());
	if (user == channel->getUserListend())
		return (push_to_buf(ERR_NOTONCHANNEL, *this, _parsedCmd[2]), 1);
	user = channel->findUser(_parsedCmd[1]);
	if (user != channel->getUserListend())
		return (push_to_buf(ERR_USERONCHANNEL, *this, _parsedCmd[1]), 1);
	if (channel->getMode('i') == true && !channel->isOp(_cmd_user))	// a tester / verifier // voir si rajouter les OPERATEURS
		return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[2]), 1);
	// std::cout << "Is user invited?" << channel->isUserInvited(my_server.findUserPtrNick(_parsedCmd[1])) << std::endl;
	if (my_server.findUserPtrNick(_parsedCmd[1]) != NULL && channel->isUserInvited(my_server.findUserPtrNick(_parsedCmd[1])) == false)
	{
		std::vector<unsigned char> v;
		std::vector<unsigned char> param = channel->getChanName();
		std::vector<unsigned char> invite_msg = to_vector(":");
		v = concat_resp(_cmd_user->getClient(), to_vector("INVITE"), my_server.findUserPtrNick(_parsedCmd[1])->getNick());
		add_to_v(invite_msg, v);
		v = to_vector(" ");
		add_to_v(v, param);
		param = to_vector(" \r\n");
		add_to_v(v, param);
		add_to_v(invite_msg, v);
		channel->addUserToInvite(my_server.findUserPtrNick(_parsedCmd[1]));
		message_to_user(my_server, my_server.findUserPtrNick(_parsedCmd[1]), invite_msg);
		return (push_to_buf(RPL_INVITING, *this, _parsedCmd[2]), 1);
	}
	return (0);
}

int Command::answer(Server &my_server)
{
    if (_cmd_user->getPassBeforeNickUser() == PASS_ORDER_ERROR
		|| _cmd_user->getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
        return (0);
    std::string    options[] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING", "NOTICE"};
    int i = 0;
    if (_parsedCmd.size() == 0)
        return (0);
    while (i < 16 && my_compare(_parsedCmd[0], options[i]) != 0)
        i++;
    switch (i)
    {
        case 0:
            return (_fun_PASS());
            break;
        case 1:
            return (_fun_NICK(my_server));
            break;
        case 2:
            return (_fun_USER(my_server));
            break;
		case 3:
            return (_fun_JOIN(my_server));
            break;
		case 5:
			return (_fun_OPER(my_server));
			break;
		case 8:
            return (_fun_MODE(my_server));
            break;
		case 11:
            return (_fun_INVITE(my_server));
            break;
        default:
            return (0);
            break;
    }
    return (0);
}

// Getters
unsigned char Command::getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type i, std::vector<std::vector<unsigned char> >::size_type j)
{
	return (_parsedCmd[i][j]);
}

std::vector<std::vector<unsigned char> > Command::getParsedCmd()
{
	return (_parsedCmd);
}

// Setters
void	Command::setParsedCmd(std::vector<std::vector<unsigned char> > val)
{
	_parsedCmd = val;
}

std::vector<std::vector<unsigned char> > Command::getCommand(void) const
{
	return (_parsedCmd);
}

User* Command::getCmdUser(void) const
{
	return (_cmd_user);
}

std::vector<std::vector<unsigned char> >& Command::getRet()
{
	return (_ret);
}