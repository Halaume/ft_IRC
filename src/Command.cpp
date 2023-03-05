/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/05 18:33:39 by madelaha         ###   ########.fr       */
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

std::string server_name = "mig.42.fr";

std::vector<unsigned char> no_param;

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

void	Command::sendToChan(Server & my_server, std::vector<Channel>::iterator chan, std::vector<unsigned char> msg)
{
	for (std::list<User *>::iterator itc = chan->getUserListbg(); itc != chan->getUserListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			my_server.getEv().data.fd = (*itc)->getfd();
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;
		}
}

/*''''''''''''''''''''''''''''''''''''
				Register TODO: error msg and disconnect
''''''''''''''''''''''''''''''''''''''*/
int Command::register_user(Server &my_server)
{
	std::vector<unsigned char> v;
	
	if (_cmd_user->getPasswd() != my_server.getPasswd())
	{
		_cmd_user->setPassBeforeNickUser(PASS_CONNECTION_ERROR);
		return (push_to_buf(ERR_PASSWDMISMATCH, *this, no_param), 1);
	}
	v = _cmd_user->getNick();
	std::list<User>::iterator itu = my_server.findUserNick(_cmd_user->getNick());
	if (itu->getUserMask() != _cmd_user->getUserMask())
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
	std::list<User>::iterator itu = my_server.findUserNick(_parsedCmd[1]);
	if (_cmd_user->isNickValid(_parsedCmd[1]) == false)
		return (push_to_buf(ERR_ERRONEUSNICKNAME, *this, _parsedCmd[1]), 1); 
	else if (itu != my_server.getUsersend() && _cmd_user->getRegistered() == true
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
// 				USER
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

int Command::_fun_JOIN(Server &my_server)
{
	(void)my_server;
	std::vector<std::vector<unsigned char> > channels;
	std::vector<std::vector<unsigned char> > keys;
	std::vector<unsigned char> param;

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
			if (my_server.findChan(channels[it]) == NULL)
				return (push_to_buf(ERR_NOSUCHCHANNEL, *this, channels[it]), 1);
			else if (my_server.findChan(channels[it])->getMode('k') == true // to verify in irssi
				&& (keys_size == 0 || (keys_size > 0 && keys_size >= it && my_compare(keys[it], my_server.findChan(channels[it])->getChanPassword()))))
				return (push_to_buf(ERR_BADCHANNELKEY, *this, channels[it]), 1);
			else if (my_server.findChan(channels[it])->getMode('l') == true // to verify in irssi
				&& my_server.findChan(channels[it])->getNbUsers() >= my_server.findChan(channels[it])->getNbUsersLimit())
				return (push_to_buf(ERR_CHANNELISFULL, *this, channels[it]), 1);
			else if (_cmd_user->getNbChan() >= MAX_NB_CHAN) // to verify in irssi
				return (push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]), 1);
			else if (my_server.findChan(channels[it])->getMode('b') == true // dont forget to set mode b when banning a user // to verify in irssi
				&& my_server.findChan(channels[it])->isUserBanned(&(*_cmd_user)))
				return (push_to_buf(ERR_BANNEDFROMCHAN, *this, channels[it]), 1);
			else if (my_server.findChan(channels[it])->getMode('i') == true	// voir pour channel operator // to verify in irssi
				&& my_server.findChan(channels[it])->isUserInvited(&(*_cmd_user)) == false)
				return (push_to_buf(ERR_INVITEONLYCHAN, *this, channels[it]), 1);
			if (my_server.findChan(channels[it])->isUserInChannel(_cmd_user) == false)
				my_server.findChan(channels[it])->addUser(&(*_cmd_user));
			push_to_buf(JOINED_CHANNEL, *this, channels[it]);
			param = rpl_topic(channels[it], my_server.findChan(channels[it])->getTopic());
			push_to_buf(RPL_TOPIC, *this, param);
			param = rpl_name(my_server.findChan(channels[it]));
			push_to_buf(RPL_NAMREPLY, *this, param);
			return (push_to_buf(RPL_ENDOFNAMES, *this, channels[it]), 1);
		}
	}
	return (0);
}

void	Command::do_chan(std::vector<unsigned char> dest, Server &my_server, std::vector<unsigned char> msg)
{
	std::vector<unsigned char>::iterator	it = dest.begin();
	Channel*								chan;
	bool									is_op = false;

	chan = NULL;
	if (dest[0] != '#')
	{
		for (it = it + 1; it != dest.end(); it++)
		{
			if (*it == '#')
				chan = my_server.findChan(std::vector<unsigned char>(it, dest.end()));
			else if (*it == '@' || *it == '+')
				is_op = true;
			else if (*it != '@' || *it != '+')
				return ;//MAYBE le commentaire en dessous, a voir
		}
	}
	else
		return;
/*	else
	{
		dest.clear();
		dest = this->_parsedCmd[0];
		insert_all(dest, "ERRCANNOTSENDTOCHAN\r\n");
		this->_cmd_user->setRet(ret);
		return ;
	}*/
	if (chan != &(*my_server.getChannel().end()) && is_op)
	{
		for (std::list<User *>::iterator itc = chan->getOpListbg(); itc != chan->getOpListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;
		}
	}
	else
	{
		for (std::list<User *>::iterator itc = chan->getUserListbg(); itc != chan->getUserListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;
		}
	}
}


// /*''''''''''''''''''''''''''''''''''''
// 				QUIT
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_QUIT(Server &my_server)
{
	my_server.delUser(*(this->_cmd_user));
	return (2);
}


// /*''''''''''''''''''''''''''''''''''''
// 				RESTART
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_RESTART(Server &my_server)
{
	if (!this->_cmd_user->getOperator())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		insert_all(this->_cmd_user->getRet(), " :Permission Denied- You're not an IRC operator\r\n");
		return (1);
	}
	free_fun(my_server);
	my_server.init(my_server.getArgv());
	return (0);
}


// /*''''''''''''''''''''''''''''''''''''
// 				PRIVMSG
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_PRIVMSG(Server &my_server)
{
	//RFC2812
	/*Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY*/
	if (this->_parsedCmd.size() < 3)
	{
		insert_all(this->_cmd_user->getRet(), ":No text to send\r\n");
		return (1);
	}
//PAS SUR DE CELLE CI
	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] != ':')
	{
		this->_cmd_user->setRet(this->_parsedCmd[0]);// RET SHOULD BE <TARGET>
		insert_all(this->_cmd_user->getRet(), " :Duplicate recipients. No message delivered\r\n");
		return (1);
	}
	std::vector<unsigned char>	receiver = _parsedCmd[1];

	if (this->_parsedCmd[2][0] == ':')
		this->_cmd_user->setRet(std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end()));
	else
		this->_cmd_user->setRet(this->_parsedCmd[2]);
	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
	while (it != this->_parsedCmd.end())
	{
		this->_cmd_user->getRet().push_back(' ');
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), it->begin(), it->end());
	}
	if (*(receiver.begin()) == '+' || *(receiver.begin()) == '&' || *(receiver.begin()) == '@' || *(receiver.begin()) == '%' || *(receiver.begin()) == '~' || *(receiver.begin()) == '#')
		do_chan(receiver, my_server, this->_cmd_user->getRet());
	else
	{
		std::list<User>::iterator	itu = my_server.findUser(receiver);
		if (itu == my_server.getUsers().end())
		{
			this->_cmd_user->setRet(this->_parsedCmd[0]);
			insert_all(this->_cmd_user->getRet(), " ERR_NOSUCHNICK\r\n");
		}
	}
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				NOTICE
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_NOTICE(Server &my_server)
{
	if (_parsedCmd.size() < 3)
		return (0);
	if (_parsedCmd.size() > 3 && _parsedCmd[2][0] != ':')
		return (0);
	
	std::vector<unsigned char>	receiver = _parsedCmd[1];
	std::vector<unsigned char>	ret;
	
	if (_parsedCmd[2][0] == ':')
		ret = std::vector<unsigned char>(_parsedCmd[2].begin() + 1, _parsedCmd[2].end());
	else
		ret = _parsedCmd[2];
	
	std::vector<std::vector<unsigned char> >::iterator	it = _parsedCmd.begin() + 3;
	while (it != _parsedCmd.end())
	{
		ret.push_back(' ');
		ret.insert(ret.end(), it->begin(), it->end());
	}
	
	if (*(receiver.begin()) == '+' || *(receiver.begin()) == '&' || *(receiver.begin()) == '@' || *(receiver.begin()) == '%' || *(receiver.begin()) == '~' || *(receiver.begin()) == '#')
		do_chan(receiver, my_server, ret);
	else
	{
		std::list<User>::iterator	itu = my_server.findUser(receiver);
		if (itu == my_server.getUsers().end())
			return (0) ;
		else
			my_server.sendto(itu->getfd(), ret);
	}
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				OPER
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_OPER(Server &my_server)
{
	std::vector<unsigned char> adminop = to_vector("admin");
	
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
			return (push_to_buf(RPL_YOUREOPER, *this, no_param),
					push_to_buf(RPL_UMODEIS, *this, mode), 1);
		}
	}
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				ERROR
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				MODE
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_MODE(Server &my_server)
{
	std::vector<unsigned char> ret;

	std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[1]);
	if (itu == my_server.getUsers().end())
	{
		if (itc == my_server.getChannel().end())
		{
			return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[1]), 1);
		}
		insert_all(ret, "ERR_NOSUCHNICK\r\n");
		this->_cmd_user->setRet(ret);
		return (0);
	}
	
	if (_parsedCmd[1] != _cmd_user->getNick())
		return (push_to_buf(ERR_USERSDONTMATCH, *this, no_param), 1);

	//if (itc->getModes().find('i')->second
	
	(void)my_server;
	return (0);
}

void Command::message_to_user(Server &my_server, User *user, std::vector<unsigned char> msg)
{
	user->setRet(msg);
	my_server.getEv().events = EPOLLOUT | EPOLLET;
	my_server.getEv().data.fd = user->getfd();
	if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, user->getfd(), &my_server.getEv()) == - 1)
		return ;
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
		
	if (channel->getMode('i') == true && !channel->isOp(_cmd_user))	// a tester / verifier
		return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[2]), 1);
		
	std::cout << "Is user invited?" << channel->isUserInvited(my_server.findUserPtrNick(_parsedCmd[1])) << std::endl;
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

// /*''''''''''''''''''''''''''''''''''''
// 				TOPIC
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_TOPIC(Server &my_server)
{
	//std::vector<unsigned char> 		ret;
	//std::list<User *>::iterator		Usrlst = itc->getUserListbg();

	//ret = _parsedCmd[0];
	if (_parsedCmd.size() < 2)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	
	std::vector<Channel>::iterator channel =  my_server.findExistingChan(_parsedCmd[1]);
	if (channel == my_server.getChannelsend())
		return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[1]), 1);
	
	std::list<User *>::iterator	user = channel->findUser(_cmd_user->getNick());
	if (user == channel->getUserListend())
		return (push_to_buf(ERR_NOTONCHANNEL, *this, _parsedCmd[1]), 1);
	


	
	// std::vector<unsigned char>	Topic = channel->getTopic();
	// if (_parsedCmd.size() == 2)
	// {
	// 	if (Topic.size() > 0)
	// 		return (push_to_buf(RPL_TOPIC, *this, _parsedCmd[2]), 1);
	// 	// else
	// 	// 	insert_all(ret, " RPL_NOTOPIC\r\n");
	// 	// this->_cmd_user->setRet(ret);
	// 	// return (1) ;
	// }
	
	// else if (_parsedCmd.size() == 3 && _parsedCmd[2].size() == 1 && _parsedCmd[2][0] == ':')
	// {
	// 	Topic.clear();
	// 	channel->setTopic(Topic);
	// 	return (push_to_buf(RPL_TOPIC, *this, _parsedCmd[2]), 1);
	// 	//insert_all(ret, " RPL_TOPIC\r\n");
	// 	//this->_cmd_user->setRet(ret);
	// 	//sendToChan(my_server, itc, ret);
	// }
	// else
	// {
	// 	Topic.clear();
	// 	std::vector<std::vector<unsigned char> >::iterator iterator = _parsedCmd.begin();
	// 	if (*(iterator->begin()) == ':')
	// 		Topic.insert(Topic.end(), iterator->begin() + 1, iterator->end());
	// 	iterator++;
	// 	Topic.push_back(' ');
	// 	while (iterator != _parsedCmd.end())
	// 	{
	// 		iterator++;
	// 		Topic.insert(Topic.end(), iterator->begin(), iterator->end());
	// 		Topic.push_back(' ');
	// 	}
	// 	itc->setTopic(Topic);
	// 	sendToChan(my_server, itc, ret);
	// }
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				KICK
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_KICK(Server &my_server)
{
	std::vector<Channel>::iterator channel =  my_server.findExistingChan(_parsedCmd[1]);

	if (_parsedCmd.size() < 3 || _parsedCmd[2].empty())
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
		
	if (channel == my_server.getChannelsend())
		return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[1]), 1);
	
	if (!channel->isOp(this->_cmd_user))
		return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[1]), 1);
	
	std::list<User *>::iterator	user = channel->findUser(_cmd_user->getNick());
	if (user == channel->getUserListend())
		return (push_to_buf(ERR_NOTONCHANNEL, *this, _parsedCmd[1]), 1);
	
	int verif = 0;
	for (std::vector<unsigned char>::size_type i = 2; i < _parsedCmd.size(); i++)
	{
		if (channel->isUserInChannel(my_server.findUserPtrNick(_parsedCmd[i])) == false)
		{
			push_to_buf(ERR_USERNOTINCHANNEL, *this, _parsedCmd[2]);
			verif = 1;
		}
		else
			channel->delUserLst(my_server.findUserPtrNick(_parsedCmd[i]));
	}
	return (verif);
}

// /*''''''''''''''''''''''''''''''''''''
// 				KILL
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_KILL(Server &my_server)
{
	if (this->_parsedCmd.size() < 3)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	
	if (!this->_cmd_user->getOperator())
		return (push_to_buf(ERR_NOPRIVILEGES, *this, no_param), 1);
	
	std::list<User>::iterator	Usr = my_server.findUserNick(this->_parsedCmd[1]);
	if (Usr == my_server.getUsersend())
		return (1) ;
	for (unsigned int i = 0; i < Usr->getChannels().size(); i++)
		Usr->getChannels()[i]->delUser(Usr->getfd());
	my_server.getUsers().erase(Usr);
	return (0);
}

// /*''''''''''''''''''''''''''''''''''''
// 				PONG
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_PONG(void)
{
	this->_cmd_user->setRet(to_vector("PONG Awesome_Irc "));
	for (unsigned int i = 1; i < this->_parsedCmd.size(); i++)
	{
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), this->_parsedCmd[i].begin(), this->_parsedCmd[i].end());
		if (i + 1 != this->_parsedCmd.size())
			this->_cmd_user->getRet().push_back(' ');
	}
	this->_cmd_user->getRet().push_back('\r');
	this->_cmd_user->getRet().push_back('\n');
	return (0);
}


int Command::answer(Server &my_server)
{
    std::cout << "___Command\n";
    print_vector2("Answer", _parsedCmd);
    if (_cmd_user->getPassBeforeNickUser() == PASS_ORDER_ERROR
		|| _cmd_user->getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
        return (0);
    std::string    options[] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING", "NOTICE"};
    int i = 0;
    if (_parsedCmd.size() == 0)
        return (0);
    while (i < 16 && (my_compare(_parsedCmd[0],options[i]) != 0))
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
		case 4:
			return (_fun_PRIVMSG(my_server));
			break;
		case 5:
			return (_fun_OPER(my_server));
			break;
		case 6:
			return (_fun_QUIT(my_server));
			break;
		case 7:
			return (_fun_ERROR(my_server));
			break;
		case 8:
			return (_fun_MODE(my_server));
			break;
		case 9:
			return (_fun_TOPIC(my_server));
			break;
		case 10:
			return (_fun_KICK(my_server));
			break;
		case 11:
			return (_fun_INVITE(my_server));
			break;
		case 12:
			return (_fun_KILL(my_server));
			break;
		case 13:
			return (_fun_RESTART(my_server));
			break;
		case 14:
			return (_fun_PONG());
			break;
		case 15:
			return (_fun_NOTICE(my_server));
			break;
	
        default:
            return (0);
            break;
    }
    // _parsedCmd.clear();
    return (0);
}

unsigned char	Command::getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type i, std::vector<std::vector<unsigned char> >::size_type j)
{
	return (_parsedCmd[i][j]);
}

std::vector<std::vector<unsigned char> >	Command::getParsedCmd()
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


// void	Command::setUser(User * usr)
// {
// 	this->_cmd_user = usr;
// }

std::vector<std::vector<unsigned char> >& Command::getRet()
{
	return (_ret);
}