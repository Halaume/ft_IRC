/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 16:17:40 by madelaha         ###   ########.fr       */
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
		(*itc)->getRet().insert((*itc)->getRet().end(), msg.begin(), msg.end());
		my_server.getEv().events = EPOLLOUT | EPOLLET;
		my_server.getEv().data.fd = (*itc)->getfd();
		if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &(my_server.getEv())) == - 1)
			my_server.delUser(*itc);
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
	std::list<User>::iterator itu = my_server.findUser(_cmd_user->getNick());
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
	std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
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
			my_server.addNewChannel(channels[it]);
			if (my_server.findChan(channels[it]) == NULL)
				return (0);
			my_server.findChan(channels[it])->addUser(_cmd_user);
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
	std::vector<Channel>::iterator								chan;
	bool									is_op = false;

	std::cerr << "JE SUIS UN CHANNEL" << std::endl;
	if (dest[0] != '#')
	{
		for (; it != dest.end(); it++)
		{
			if (*it == '#')
			{
				chan = my_server.findExistingChan(std::vector<unsigned char>(it, dest.end()));
				break ;
			}
			else if (*it == '@' || *it == '+')
			{
				std::cerr << "Setting op as true" << std::endl;
				is_op = true;
			}
			else if (*it != '@' || *it != '+')
				return ;
		}
	}
	else
		chan = my_server.findExistingChan(std::vector<unsigned char>(dest.begin(), dest.end()));
	if (chan == my_server.getChannelsend())
	{
		push_to_buf(ERR_NOSUCHNICK, *this, this->_parsedCmd[1]);
		return ;
	}
	std::vector<unsigned char> ret;
	std::vector<std::vector<unsigned char> >::size_type i;
	unsigned char text[] = " PRIVMSG ";
	int j = 0;

	ret = this->_cmd_user->getClient();
	ret.insert(ret.begin(), ':');
	// ret.push_back('!');
	// for (i = 0; i < chan->getChanName().size(); i++)
	// 	ret.insert(ret.end(), this->_cmd_user->getUserNamebg(), this->_cmd_user->getUserNameend());
	// ret.push_back('@');
	// for (i = 0; i < this->_cmd_user->getUserMask().size(); i++)
	// 		ret.push_back(this->_cmd_user->getUserMask()[i]);
	while (text[j])
		ret.push_back(text[j++]);
	for (i = 0; i < chan->getChanName().size(); i++)
		ret.push_back(chan->getChanName()[i]);
	ret.push_back(' ');
	ret.push_back(':');
	msg.insert(msg.begin(), ret.begin(), ret.end());
	if (is_op)
	{
				for (std::list<User *>::iterator itc = chan->getOpListbg(); itc != chan->getOpListend(); itc++)
		{
			(*itc)->getRet().insert((*itc)->getRet().begin(), msg.begin(), msg.end());
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			my_server.getEv().data.fd = (*itc)->getfd();
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				my_server.delUser(*itc);
		}
	}
	else
	{
		for (std::list<User *>::iterator itc = chan->getUserListbg(); itc != chan->getUserListend(); itc++)
		{
			(*itc)->getRet().insert((*itc)->getRet().begin(), msg.begin(), msg.end());
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			my_server.getEv().data.fd = (*itc)->getfd();
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				my_server.delUser(*itc);
		}
	}
}


// /*''''''''''''''''''''''''''''''''''''
// 				QUIT
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_QUIT(Server &my_server)
{
	my_server.delUser(this->_cmd_user);
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
	std::vector<unsigned char>	msg;
	std::list<User>::iterator	itu;
	if (this->_parsedCmd.size() < 3)
	{
		insert_all(this->_cmd_user->getRet(), ":No text to send\r\n");
		return (1);
	}
	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] != ':')
	{
		this->_cmd_user->setRet(this->_parsedCmd[1]);
		insert_all(this->_cmd_user->getRet(), " :Duplicate recipients. No message delivered\r\n");
		return (1);
	}


	if (this->_parsedCmd[2][0] == ':')
	{
		msg = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
		for (std::vector<unsigned char>::size_type i = 3; i != this->_parsedCmd.size(); i++)
		{
			msg.push_back(' ');
			msg.insert(msg.end(), this->_parsedCmd[i].begin(), this->_parsedCmd[i].end());
		}
	}
	else
		msg = this->_parsedCmd[2];
	msg.push_back('\r');
	msg.push_back('\n');
	std::vector<std::vector<unsigned char> >	target = splitOnComa(this->_parsedCmd[1]);
	for (std::vector<std::vector<unsigned char> >::iterator it = target.begin(); it != target.end(); it++)
	{
		std::vector<unsigned char> tmp2 = msg;
		if (*((*it).begin()) == '+' || *((*it).begin()) == '&' || *((*it).begin()) == '@' || *((*it).begin()) == '%' || *((*it).begin()) == '~' || *((*it).begin()) == '#')
			do_chan(*it, my_server, tmp2);
		else
		{
			itu = my_server.findUser(*it);
			if (itu == my_server.getUsersend())
				push_to_buf(ERR_NOSUCHNICK, *this, *it);
		
			std::vector<unsigned char>	tmp = msg;
			tmp.push_back(':');
			tmp.insert(tmp.begin() + 1, this->_cmd_user->getNickbg(), this->_cmd_user->getNickend());
			unsigned char text[] = " PRIVMSG ";
			for (int j = 0; text[j]; j++)
				tmp.push_back(text[j]);
			tmp.insert(tmp.end(), itu->getNickbg(), itu->getNickend());
			tmp.push_back(' ');
			tmp.push_back(':');
			tmp2.insert(tmp2.begin(), tmp.begin(), tmp.end());
			itu->setRet(tmp2);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			my_server.getEv().data.fd = itu->getfd();
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, itu->getfd(), &my_server.getEv()) == - 1)
				my_server.delUser(&(*itu));
		}
	}
	return (1);
}

// /*''''''''''''''''''''''''''''''''''''
// 				NOTICE
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_NOTICE(Server &my_server) // TODO Reprendre privmsg
{
	(void)my_server;
	// std::vector<unsigned char>	msg;
	// std::list<User>::iterator	itu;
	// if (this->_parsedCmd.size() < 3)
	// 	return (1);
		
	// if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] != ':')
	// 	return (1);

	// std::vector<unsigned char>	*it = this->_parsedCmd[1];
	// if (this->_parsedCmd[2][0] == ':')
	// {
	// 	msg = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
	// 	for (std::vector<unsigned char>::size_type i = 3; i != this->_parsedCmd.size(); i++)
	// 	{
	// 		msg.push_back(' ');
	// 		msg.insert(msg.end(), this->_parsedCmd[i].begin(), this->_parsedCmd[i].end());
	// 	}
	// }
	
	// else
	// 	msg = this->_parsedCmd[2];
	// msg.push_back('\r');
	// msg.push_back('\n');
	
	// if (*(*it.begin()) == '+' || *(*it.begin()) == '&' || *(*it.begin()) == '@' || *(*it.begin()) == '%' || *(*it.begin()) == '~' || *(*it.begin()) == '#')
	// 	return (do_chan(*it, my_server, msg), 1);
		
	// else
	// {
	// 	itu = my_server.findUser(this->_parsedCmd[1]);
	// 	if (itu == my_server.getUsersend())
	// 		return (1);
	
	// 	std::vector<unsigned char>	tmp;
	// 	tmp.push_back(':');
	// 	tmp.insert(tmp.begin() + 1, this->_cmd_user->getNickbg(), this->_cmd_user->getNickend());
	// 	unsigned char text[] = " NOTICE ";
	// 	for (int j = 0; text[j]; j++)
	// 		tmp.push_back(text[j]);
	// 	tmp.insert(tmp.end(), itu->getNickbg(), itu->getNickend());
	// 	tmp.push_back(' ');
	// 	tmp.push_back(':');
	// 	msg.insert(msg.begin(), tmp.begin(), tmp.end());
	// 	itu->setRet(msg);
	// 	my_server.getEv().events = EPOLLOUT | EPOLLET;
	// 	my_server.getEv().data.fd = itu->getfd();
	// 	if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, itu->getfd(), &my_server.getEv()) == - 1)
	// 		my_server.delUser(&(*itu));
	// }
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
	if (_parsedCmd.size() < 2)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	
	std::vector<Channel>::iterator channel =  my_server.findExistingChan(_parsedCmd[1]);
	if (channel == my_server.getChannelsend())
		return (push_to_buf(ERR_NOSUCHCHANNEL, *this, _parsedCmd[1]), 1);
	
	std::list<User *>::iterator	user = channel->findUser(_cmd_user->getNick());
	if (user == channel->getUserListend())
		return (push_to_buf(ERR_NOTONCHANNEL, *this, _parsedCmd[1]), 1);
	
	if (channel->getMode('t') == true && !channel->isOp(this->_cmd_user))
		return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[1]), 1);

	std::vector<unsigned char>	Topic = channel->getTopic();
	if (_parsedCmd.size() == 2)
	{
		if (Topic.size() > 0)
			return (push_to_buf(RPL_TOPIC, *this, Topic), 1);
	}
	
	else if (_parsedCmd.size() >= 3 && _parsedCmd[2][0] != ':')
		return (0);

	else if (_parsedCmd.size() == 3 && _parsedCmd[2].size() == 1 && _parsedCmd[2][0] == ':')
	{
		Topic.clear();
		channel->setTopic(Topic);
		return (push_to_buf(RPL_TOPIC, *this, Topic), 1);
	}

	else
	{
		Topic.clear();
		std::vector<std::vector<unsigned char> >::iterator iterator = _parsedCmd.begin() + 2;
		if (*(iterator->begin()) == ':')
			Topic.insert(Topic.begin(), iterator->begin() + 1, iterator->end());
		iterator++;
		Topic.push_back(' ');
		while (iterator != _parsedCmd.end())
		{
			Topic.insert(Topic.end(), iterator->begin(), iterator->end());
			Topic.push_back(' ');
			iterator++;
		}
		channel->setTopic(Topic);
		return (push_to_buf(RPL_TOPIC, *this, Topic), 1);
	}
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
	
	// if (!channel->isOp(this->_cmd_user))
	// 	return (push_to_buf(ERR_CHANOPRIVSNEEDED, *this, _parsedCmd[1]), 1);
	
	std::list<User *>::iterator	user = channel->findUser(_cmd_user->getNick());
	if (user == channel->getUserListend())
		return (push_to_buf(ERR_NOTONCHANNEL, *this, _parsedCmd[1]), 1);
	
	int verif = 0;
	for (std::vector<unsigned char>::size_type i = 2; i < _parsedCmd.size(); i++)
	{
		if (channel->isUserInChannel(my_server.findUserPtrNick(_parsedCmd[i])) == false)
		{
			push_to_buf(ERR_USERNOTINCHANNEL, *this, _parsedCmd[i]);
			verif = 1;
		}
		else
		{
			channel->delUserLst(my_server.findUserPtrNick(_parsedCmd[i]));
			std::vector<unsigned char> chanName = channel->getChanName();
			std::vector<unsigned char> kickMsg;
			kickMsg.push_back(':');
			std::vector<unsigned char> client = this->_cmd_user->getClient();
			kickMsg.insert(kickMsg.end(), client.begin(), client.end());
			insert_all(kickMsg, " KICK ");
			kickMsg.insert(kickMsg.end(), chanName.begin(), chanName.end());
			kickMsg.push_back(' ');
			kickMsg.insert(kickMsg.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
			kickMsg.push_back('\r');
			kickMsg.push_back('\n');
			sendToChan(my_server,channel, kickMsg);
		}
	}
	return (verif);
}

// /*''''''''''''''''''''''''''''''''''''
// 				KILL
// ''''''''''''''''''''''''''''''''''''''*/
int	Command::_fun_KILL(Server &my_server)
{
	if (this->_parsedCmd.size() < 3)
	if (!this->_cmd_user->getOperator())
		return (push_to_buf(ERR_NOPRIVILEGES, *this, no_param), 1);
	if (this->_parsedCmd.size() < 3)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);

	std::list<User>::iterator	Usr = my_server.findUser(this->_parsedCmd[1]);
	if (Usr == my_server.getUsersend())
		return (0) ;
	my_server.delUser(&(*Usr));
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

int	Command::_fun_PART(Server &my_server)
{
	std::vector<unsigned char>	ret;

	if (this->_parsedCmd.size() < 3)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);

	std::vector<std::vector<unsigned char> >	chans = splitOnComa(this->_parsedCmd[1]);
	std::vector<Channel>::iterator	itc;
	std::vector<std::vector<unsigned char> >::size_type	i = 0;

	for (std::vector<std::vector<unsigned char> >::iterator it = chans.begin(); it != chans.end(); it++, i++)
	{
		itc = my_server.findExistingChan(chans[i]);
		if (itc == my_server.getChannelsend())
			push_to_buf(ERR_NOSUCHCHANNEL, *this, chans[i]);
		else if (itc->getUserList().size() == 1)
		{
			std::vector<unsigned char> chanName = itc->getChanName();
			std::vector<unsigned char> partMsg;
			partMsg.push_back(':');
			std::vector<unsigned char> client = this->_cmd_user->getClient();
			partMsg.insert(partMsg.end(), client.begin(), client.end());
			insert_all(partMsg, " PART ");
			partMsg.insert(partMsg.end(), chanName.begin(), chanName.end());
			partMsg.push_back('\r');
			partMsg.push_back('\n');
			sendToChan(my_server, itc, partMsg);

			my_server.delChan(itc);
		}
		else if (itc->findUser(_cmd_user->getNick()) == itc->getUserListend())
			push_to_buf(ERR_NOTONCHANNEL, *this, itc->getChanName());
		else
		{
			std::vector<unsigned char> chanName = itc->getChanName();
			itc->delUserLst(this->_cmd_user);
			std::vector<unsigned char> partMsg;
			partMsg.push_back(':');
			std::vector<unsigned char> client = this->_cmd_user->getClient();
			partMsg.insert(partMsg.end(), client.begin(), client.end());
			insert_all(partMsg, " PART ");
			partMsg.insert(partMsg.end(), chanName.begin(), chanName.end());
			partMsg.push_back('\r');
			partMsg.push_back('\n');
			sendToChan(my_server, itc, partMsg);
		}
	}
	return (1);
}


int Command::answer(Server &my_server)
{
	std::cout << "___Command\n";
	print_vector2("Answer", _parsedCmd);
	if (_cmd_user->getPassBeforeNickUser() == PASS_ORDER_ERROR || _cmd_user->getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
		return (0);
	std::string    options[] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING", "NOTICE", "PART"};
	int i = 0;
	if (_parsedCmd.size() == 0)
		return (0);
	while (i < 17 && my_compare(_parsedCmd[0], options[i]) != 0)
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
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_JOIN(my_server));
			break;
		case 4:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_PRIVMSG(my_server));
			break;
		case 5:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_OPER(my_server));
			break;
		case 6:
			return (_fun_QUIT(my_server));
			break;
		case 7:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_ERROR(my_server));
			break;
		case 8:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_MODE(my_server));
			break;
		case 9:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_TOPIC(my_server));
			break;
		case 10:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_KICK(my_server));
			break;
		case 11:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_INVITE(my_server));
			break;
		case 12:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_KILL(my_server));
			break;
		case 13:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_RESTART(my_server));
			break;
		case 14:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_PONG());
			break;
		case 15:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_NOTICE(my_server));
			break;
		case 16:
			if (!this->_cmd_user->getRegistered())
				break ;
			return (_fun_PART(my_server));
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
