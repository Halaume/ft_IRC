/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/01 17:43:20 by ghanquer         ###   ########.fr       */
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

int Command::register_user(Server &my_server, User &user)
{
	(void)my_server;
	std::vector<unsigned char> v;
	// std::vector<unsigned char> new_nick;

	if (my_compare(_cmd_user->getPasswd(), my_server.getPasswd()))
	{
		user.setPassBeforeNickUser(PASS_CONNECTION_ERROR);
		return (push_to_buf(ERR_PASSWDMISMATCH, *this, no_param), 1);
	}

	std::list<User>::iterator itu = my_server.findUserNick(_parsedCmd[1]);
	if (itu != my_server.getUsersend())
		std::cout << "SAAAAAAAAMMMEEE NAME OKKKKKKKK\n";
	// 	return (push_to_buf(ERR_NICKNAMEINUSE, *this, no_param), 1);

	std::cout << "number of conenctions from user:" << my_server.nbConnections(user) << std::endl;// change to ip check?

	_cmd_user->setRegistered(true);
	push_to_buf(RPL_WELCOME, *this, no_param);
	push_to_buf(RPL_YOURHOST, *this, no_param);
	push_to_buf(RPL_CREATED, *this, no_param);
	push_to_buf(RPL_MYINFO, *this, no_param);
	return (1);
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
  PASS TODO:exit and close connection when password mismatches on registration?
  ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_PASS(Server &my_server, User &user)
{
	(void)my_server;
	std::vector<unsigned char> v;

	if ((_parsedCmd.size() < 2 || _parsedCmd[1].empty() == true) && _cmd_user->getRegistered() == false)
		return (push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param), 1);
	else if (_cmd_user->getRegistered())
		return (push_to_buf(ERR_ALREADYREGISTERED, *this, no_param), 1);
	_cmd_user->setPasswd(_parsedCmd[1]);
	_cmd_user->setPassStatus(PASSWORD_SET);
	if (user.getPassBeforeNickUser() == WAITING_FOR_PASS)
		user.setPassBeforeNickUser(PASS_ORDER_OK);
	return (0);
}

/*''''''''''''''''''''''''''''''''''''
  NICK
  ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_NICK(Server &my_server, User &user)
{
	std::vector<unsigned char> ret;
	int to_send = 0;

	if (user.getPassBeforeNickUser() == WAITING_FOR_PASS)
	{
		user.setPassBeforeNickUser(PASS_ORDER_ERROR);
		return (0);
	}
	if (_parsedCmd.size() < 2)
		return (push_to_buf(ERR_NONICKNAMEGIVEN, *this, no_param), 1);
	std::list<User>::iterator itu = my_server.findUserNick(_parsedCmd[1]);
	if (_cmd_user->isNickValid(_parsedCmd[1]) == false) // add ctrl+G to invalid chars
		return (push_to_buf(ERR_ERRONEUSNICKNAME, *this, no_param), 1);
	else if (itu != my_server.getUsersend() && _cmd_user->getRegistered() == true
			&& my_compare(_cmd_user->getNick(), _parsedCmd[1]))
		return (push_to_buf(ERR_NICKNAMEINUSE, *this, no_param), 1);
	if (_cmd_user->getRegistered() == true)
	{
		push_to_buf(OWN_NICK_RPL, *this, _parsedCmd[1]);
		to_send = 1;
	}
	_cmd_user->setNick(_parsedCmd[1]);
	if (user.getPassBeforeNickUser() == PASS_USER_OK)
		return (register_user(my_server, user)); // si pas encore enregistré ajouter _ 1 2 etc pour permettre
	else
		user.setPassBeforeNickUser(PASS_NICK_OK);
	return (to_send);
}

// /*''''''''''''''''''''''''''''''''''''
// 				USER
// ''''''''''''''''''''''''''''''''''''''*/
int Command::_fun_USER(Server &my_server, User &user)
{
	std::vector<unsigned char> ret;

	if (user.getPassBeforeNickUser() == WAITING_FOR_PASS)
	{
		user.setPassBeforeNickUser(PASS_ORDER_ERROR);
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
		if (user.getPassBeforeNickUser() == PASS_NICK_OK)
			return (register_user(my_server, user)); // to finish -> same user name/mask and nick -> iterate nick -> set a limit of connections?
		else
			user.setPassBeforeNickUser(PASS_USER_OK);
	}
	return (0);
}

int Command::_fun_JOIN(Server &my_server)
{
	(void)my_server;
	std::vector<std::vector<unsigned char> > channels;
	std::vector<std::vector<unsigned char> > keys;
	// std::vector<std::vector<unsigned char> > keys;

	//RFC 2813/4.2.1
	/*Numeric Replies:
	  ERR_NEEDMOREPARAMS		OK
	  ERR_BANNEDFROMCHAN		OK
	  ERR_INVITEONLYCHAN		OK
	  ERR_BADCHANNELKEY		OK
	  ERR_CHANNELISFULL		OK
	  ERR_BADCHANMASK			OK
	  ERR_NOSUCHCHANNEL		OK	
	  ERR_TOOMANYCHANNELS		OK
	  RPL_TOPIC*/
	//RPL_TOPIC pour le new User et RPL_NAMREPLY Pour tout les users du chan (Nouvel utilisateur inclut)


	if (_parsedCmd.size() < 2)
	{
		push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param); // sendto
		return (1);
	}
	reparseChannelsKeys(_parsedCmd[1], &channels);
	if (_parsedCmd.size() > 2)
		reparseChannelsKeys(_parsedCmd[2], &keys);
	std::vector<std::vector<unsigned char> >::size_type keys_size = keys.size();
	for (std::vector<std::vector<unsigned char> >::size_type it = 0; it < channels.size(); ++it)
	{

		if (channels[it].empty() == false && channels[it][0] != '#' && channels[it][0] != '&') // ajouter fonciton qui checke s il y a pas le char 07 ^G
			push_to_buf(ERR_BADCHANMASK, *this, channels[it]);
		else if (_cmd_user->getNbChan() >= MAX_NB_CHAN)
			push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]);
		else if (my_server.channelExists(channels[it]) == false)
		{

			Channel new_channel(channels[it]);
			new_channel.addUser(&(*_cmd_user));
			my_server.addNewChannel(new_channel);
			// send messages to accept user RPL
		}
		else
		{
			// protect findchan
			if (my_server.findChan(channels[it]) == NULL)
				push_to_buf(ERR_NOSUCHCHANNEL, *this, channels[it]); // sendto
			else if (my_server.findChan(channels[it])->isUserInChannel(&(*_cmd_user)))
			{
				// user is already in channel -> not sure about this one -> del?
			}
			else if (my_server.findChan(channels[it])->getMode('k') == true // check if channel mode k
					&& (keys_size == 0 || (keys_size > 0 && keys_size >= it && my_compare(keys[it], my_server.findChan(channels[it])->getChanPassword()))))
				push_to_buf(ERR_BADCHANNELKEY, *this, channels[it]); // sendto
			else if (my_server.findChan(channels[it])->getMode('l') == true // check if channel mode l
					&& my_server.findChan(channels[it])->getNbUsers() >= my_server.findChan(channels[it])->getNbUsersLimit())
				push_to_buf(ERR_CHANNELISFULL, *this, channels[it]);
			else if (_cmd_user->getNbChan() >= MAX_NB_CHAN)
				push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]);
			else if (my_server.findChan(channels[it])->getMode('b') == true // dont forget to set mode b when banning a user
					&& my_server.findChan(channels[it])->isUserBanned(&(*_cmd_user)))
				push_to_buf(ERR_BANNEDFROMCHAN, *this, channels[it]);
			else if (my_server.findChan(channels[it])->getMode('i') == true	// voir pour channel operator
					&& my_server.findChan(channels[it])->isUserInvited(&(*_cmd_user)) == false)
				push_to_buf(ERR_INVITEONLYCHAN, *this, channels[it]);
			else
				my_server.findChan(channels[it])->addUser(&(*_cmd_user));
		}
	}
	return (1);
}

int	Command::_fun_QUIT(Server &my_server)//TODO a revoir
{
	(void)my_server;
	for (std::vector<Channel *>::iterator itc = _cmd_user->getChannelsbg(); itc != _cmd_user->getChannelsend(); itc++)
	{
		for (std::list<User *>::const_iterator itu = (*itc)->getUserListbg(); itu != (*itc)->getUserListend(); itu++)
			this->_cmd_user->getChannels().erase(itc);
	}
	return (0);
}

int	Command::_fun_RESTART(Server &my_server)
{
	if (!this->_cmd_user->getOperator())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		insert_all(this->_cmd_user->getRet(), " :Permission Denied- You're not an IRC operator");
		return (1);
	}
	free_fun(my_server);
	this->_parsedCmd.erase(this->_parsedCmd.begin(), this->_parsedCmd.end());
	my_server.init(my_server.getArgv());
	return (0);
}

void	Command::do_chan(std::vector<unsigned char> dest, Server &my_server, std::vector<unsigned char> msg)
{
	std::vector<unsigned char>::iterator	it = dest.begin();
	Channel*								chan;
	bool									is_op = false;

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
	std::vector<unsigned char> ret;
	std::vector<std::vector<unsigned char> >::size_type i;
	unsigned char text[] = " PRIVMSG ";
	int j = 0;
	
	
	if (chan != &(*my_server.getChannel().end()) && is_op)
	{
		ret = this->_cmd_user->getNick();
		ret.insert(ret.begin(), ':');
		ret.push_back('!');
		for (i = 0; i < chan->getChanName().size(); i++)
			ret.insert(ret.end(), this->_cmd_user->getUserName().begin(), this->_cmd_user->getUserName().end());
		ret.push_back('@');
		for (i = 0; i < this->_cmd_user->getUserMask().size(); i++)
			ret.push_back(this->_cmd_user->getUserMask()[i]);
		while (text[j])
			ret.push_back(text[j++]);
		for (i = 0; i < chan->getChanName().size(); i++)
			ret.push_back(chan->getChanName()[i]);
		msg.insert(msg.begin(), ret.begin(), ret.end());
		msg.push_back('\r');
		msg.push_back('\n');
		for (std::list<User *>::iterator itc = chan->getOpListbg(); itc != chan->getOpListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			my_server.getEv().data.fd = (*itc)->getfd();
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
			my_server.getEv().data.fd = (*itc)->getfd();
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;//TODO delUser into pas return
		}
	}
}

int	Command::_fun_PRIVMSG(Server &my_server)
{
	std::vector<unsigned char>	msg;
	std::list<User>::iterator	itu;
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
	std::vector<unsigned char>	receiver = this->_parsedCmd[1];

	if (this->_parsedCmd[2][0] == ':')
		msg = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
	else
		msg = this->_parsedCmd[2];
	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
	while (it != this->_parsedCmd.end())
	{
		msg.push_back(' ');
		msg.insert(this->_cmd_user->getRet().end(), it->begin(), it->end());
	}
	msg.push_back('\r');
	msg.push_back('\n');
	if (*(receiver.begin()) == '+' || *(receiver.begin()) == '&' || *(receiver.begin()) == '@' || *(receiver.begin()) == '%' || *(receiver.begin()) == '~' || *(receiver.begin()) == '#')
		do_chan(receiver, my_server, msg);
	else
	{
		itu = my_server.findUser(this->_parsedCmd[1]);
		if (itu == my_server.getUsers().end())
		{
			this->_cmd_user->setRet(this->_parsedCmd[0]);
			insert_all(this->_cmd_user->getRet(), " ERR_NOSUCHNICK\r\n");
			return (1);
		}
		std::vector<unsigned char>	tmp;
		tmp.push_back(':');
		tmp.insert(tmp.begin() + 1, this->_cmd_user->getNick().begin(), this->_cmd_user->getNick().end());
		tmp.push_back(' ');
		insert_all(tmp, "PRIVMSG");
		tmp.push_back(' ');
		tmp.insert(tmp.end(), itu->getNick().begin(), itu->getNick().end());
		tmp.push_back(' ');
		tmp.push_back(':');
		msg.insert(msg.begin(), tmp.begin(), tmp.end());
		msg.push_back('\r');
		msg.push_back('\n');
		itu->setRet(msg);
		my_server.getEv().events = EPOLLOUT | EPOLLET;
		my_server.getEv().data.fd = itu->getfd();
		if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, itu->getfd(), &my_server.getEv()) == - 1)
			return (0);//TODO delUser into pas return
	}
	return (0);
}

int	Command::_fun_OPER(Server &my_server)
{
	std::vector<unsigned char> ret;

	if (_parsedCmd.size() == 3)
	{
		std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
		if (itu == my_server.getUsers().end())
		{
			push_to_buf(ERR_PASSWDMISMATCH, *this, no_param);
			return (1);
		}
		else 
		{
			if (_parsedCmd[2] != my_server.getPassword())
			{
				push_to_buf(ERR_PASSWDMISMATCH, *this, no_param);
				return (1);
			}
		}
	}
	push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param);
	return (1);
}

int	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;	
	return (0);
}

int	Command::_fun_MODE(Server &my_server)
{
	std::vector<unsigned char> ret;

	std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
	if (itu == my_server.getUsers().end())
	{
		insert_all(ret, "ERR_NOSUCHNICK\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}
	return (0);
}


int	Command::_fun_INVITE(Server &my_server)
{
	std::vector<unsigned char>	ret;

	if (this->_parsedCmd.size() > 3)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[2]);
	if (itc == my_server.getChannel().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	std::list<User *>::iterator	itu = itc->findUser(_cmd_user->getUserName());
	if (itu == itc->getUserListend())
	{
		insert_all(ret, " ERR_NOTONCHANNEL\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	itu = itc->findUser(_parsedCmd[1]);
	if (itu != itc->getUserListend())
	{
		insert_all(ret, " ERR_USERONCHANNEL\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	if (itc->getModes().find('i')->second && !itc->isOp(_cmd_user))
	{
		insert_all(ret, " ERR_CHANOPRIVSNEEDED\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	insert_all(ret, " RPL_INVITING\r\n");
	this->_cmd_user->setRet(ret);
	return (1);
}

int	Command::_fun_TOPIC(Server &my_server)
{
	std::vector<unsigned char> 		ret;
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[1]);
	std::list<User *>::iterator		Usrlst = itc->getUserListbg();

	ret = _parsedCmd[0];
	if (_parsedCmd.size() < 2)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	if (itc == my_server.getChannel().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	while (Usrlst != itc->getUserListend() && *Usrlst != this->_cmd_user)
		Usrlst++;
	if (Usrlst == itc->getUserListend())
	{
		ret = _cmd_user->getUserName();
		ret.insert(ret.end(), _parsedCmd[2].begin(), _parsedCmd[2].end());
		insert_all(ret, " ERR_NOTONCHANNEL\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	if (itc->isOp(*Usrlst) == false)
	{
		insert_all(ret, " ERR_CHOPRIVSNEEDED\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}

	std::vector<unsigned char>	Topic = itc->getTopic();
	if (_parsedCmd.size() == 2)
	{
		if (Topic.size() > 0)
			insert_all(ret, " RPL_TOPIC\r\n");
		else
			insert_all(ret, " RPL_NOTOPIC\r\n");
		this->_cmd_user->setRet(ret);
		return (1);
	}
	else if (_parsedCmd.size() == 3 && _parsedCmd[2].size() == 1 && _parsedCmd[2][0] == ':')
	{
		Topic.clear();
		itc->setTopic(Topic);
		insert_all(ret, " RPL_TOPIC\r\n");
		this->_cmd_user->setRet(ret);
		sendToChan(my_server, itc, ret);
		return (0);
	}
	else
	{
		Topic.clear();
		std::vector<std::vector<unsigned char> >::iterator iterator = _parsedCmd.begin();
		if (*(iterator->begin()) == ':')
			Topic.insert(Topic.end(), iterator->begin() + 1, iterator->end());
		iterator++;
		Topic.push_back(' ');
		while (iterator != _parsedCmd.end())
		{
			iterator++;
			Topic.insert(Topic.end(), iterator->begin(), iterator->end());
			Topic.push_back(' ');
		}
		itc->setTopic(Topic);
		sendToChan(my_server, itc, ret);
		return (0);
	}
}

int	Command::_fun_KICK(Server &my_server)
{
	std::vector<Channel>::iterator	tmp = my_server.findExistingChan(this->_parsedCmd[1]);

	if (this->_parsedCmd.size() < 3)
	{
		insert_all(this->_cmd_user->getRet(), " ERR_NEEDMOREPARAM\r\n");
		return (1);
	}

	if (tmp == my_server.getChannel().end())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		insert_all(this->_cmd_user->getRet(), " ERR_NOSUCHCHANNEL\r\n");
		return (1);
	}

	if (!tmp->isOp(this->_cmd_user))
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), this->_parsedCmd[1].begin(), this->_parsedCmd[1].end());
		insert_all(this->_cmd_user->getRet(), " :You're not channel operator\r\n");
		return (1);
	}

	std::list<User *>::iterator		Usrlst = tmp->getUserListbg();
	while (Usrlst != tmp->getUserListend() && *Usrlst != this->_cmd_user)
		Usrlst++;
	if (Usrlst == tmp->getUserListend())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmd_user->getRet(), " ERR_NOTONCHANNEL\r\n");
		return (1);
	}
	Usrlst = tmp->getUserListbg();
	while (Usrlst != tmp->getUserListend() && !(**Usrlst == this->_parsedCmd[2]))
		Usrlst++;
	if (Usrlst == tmp->getUserListend())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmd_user->getRet(), " ERR_USERNOTINCHANNEL\r\n");
		return (1);
	}
	if (tmp->isOp(*Usrlst))
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmd_user->getRet(), " ERR_CANNOTKICKADMIN\r\n");
		return (1);
	}

	for (std::vector<std::vector<unsigned char> >::iterator it = this->_parsedCmd.begin() + 3; it != this->_parsedCmd.end(); it++)
		this->_cmd_user->getRet().insert(this->_cmd_user->getRet().end(), it->begin(), it->end());
	tmp->getUserList().erase(Usrlst);
	//Aucune idee de quoi envoyer a ce user comme notification qu'il a ete virer
	//Maybe ERROR :you have been kicked jsp
	return (0);
}

int	Command::_fun_KILL(Server &my_server)
{
	if (!this->_cmd_user->getOperator())
	{
		this->_cmd_user->setRet(this->_cmd_user->getUserName());
		insert_all(this->_cmd_user->getRet(), " :Permission Denied- You're not an IRC operator\r\n");
		return (1);
	}
	if (this->_parsedCmd.size() < 3)
	{
		push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param);
		return (1);
	}
	std::list<User>::iterator	Usr = my_server.findUserNick(this->_parsedCmd[1]);
	if (Usr == my_server.getUsersend())
		return (1);
	for (unsigned int i = 0; i < Usr->getChannels().size(); i++)
		Usr->getChannels()[i]->delUser(Usr->getfd());
	my_server.getUsers().erase(Usr);
	return (0);
}


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

int	Command::_fun_NOTICE(Server &my_server)
{
	(void)my_server;
	return (0);
}


int Command::answer(Server &my_server, User &user)
{
	std::cout << "___Command\n";
	print_vector2("Answer", _parsedCmd);
	if (user.getPassBeforeNickUser() == PASS_ORDER_ERROR || user.getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
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
			return (_fun_PASS(my_server, user));
			break;
		case 1:
			return (_fun_NICK(my_server, user));
			break;
		case 2:
			return (_fun_USER(my_server, user));
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
			if (!this->_cmd_user->getRegistered())
				break ;
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
		default:
			return (0);
			break;
	}
	// _parsedCmd.clear();
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
