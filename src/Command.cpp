/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 17:50:51 by ghanquer         ###   ########.fr       */
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

Command::Command(void):  _globalCmd(), _parsedCmd(), _cmd_fd_user(), _cmd_buf(), _cmd_size(0), _error(0), _cmd_user(NULL)
{
}

Command::Command(const Command &copy): _globalCmd(copy._globalCmd), _parsedCmd(copy._parsedCmd), _cmd_fd_user(copy._cmd_fd_user) \
, _cmd_buf(copy._cmd_buf), _cmd_size(copy._cmd_size), _error(copy._error), _cmd_user(copy._cmd_user)
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
	this->_parsedCmd = cmd;
}
void	Command::setUser(User* usr)
{
	this->_cmdUser = usr;
}

void Command::setCmdUser(Server &my_server)
{
	_cmd_user = my_server.findUser(_cmd_fd_user);
}

void Command::push_to_buf(int error, std::vector<unsigned char> cmd)
{
	(void)error; (void)cmd;
	_cmd_buf.clear();
	add_to_vector(&_cmd_buf, ":" + server_name + " ");
	add_to_vector(&_cmd_buf, _cmd_user->getUserName());
	add_to_vector(&_cmd_buf, " ");
	add_to_vector(&_cmd_buf, numeric_response(ERR_NEEDMOREPARAMS, cmd));
	// add_to_vector(&_cmd_buf, cmd);
	// add_to_vector(&_cmd_buf, ERR_NEEDMOREPARAMSthis->_cmdUser.getRet());

	std::vector<unsigned char>::size_type m;
	std::cout << "2:\n";
	for (m = 0; m < _cmd_buf.size(); m++)
		std::cout << _cmd_buf[m];
	std::cout << "\n";

}

/*''''''''''''''''''''''''''''''''''''
				CAP > to delete?
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_CAP(Server &my_server)
{
	std::cout << "CAP COMMAND REALIZED : nothing to do here\n";
	(void)my_server;
}

/*''''''''''''''''''''''''''''''''''''
				PASS
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_PASS(Server &my_server)
{
	(void)my_server;
	// 1 verify if host is already registered
	// 2 verify enough arguments
	// 3 ver

	std::cerr << "PASS COMMAND REALIZED\n";
	std::vector<unsigned char> v;
	
	if (this->_parsedCmd.size() < 2 && my_server.findUser(_cmd_fd_user)->getRegistered() == false)
	{
		std::cout << "PASS not enough params\n";
		push_to_buf(ERR_NEEDMOREPARAMS, _parsedCmd[0]);
		return;
	}
	this->_cmdUser->setRet(this->_parsedCmd[0]);
	// else if (my_server.findUser(_cmd_fd_user)->getRegistered())
	// {
	// 	std::cout << "PASS already registered\n";
	// 	_error = 462;
	// 	return ;
	// }
	// my_server.findUser(_cmd_fd_user)->setPasswd(_parsedCmd[1]);
	// std::cout << "PASS found user passwd:|" << my_server.findUser(_cmd_fd_user)->getPasswd() << "|\n";
	// if (my_server.findUser(_cmd_fd_user)->getPasswd() != my_server.getPasswd())
	// {
	// 	std::cout << "PASS wrong password\n";
	// 	_error = 464;
	// 	return;
	// }
}

void	Command::_fun_NICK(Server &my_server)
{
	(void)my_server;
}

//TODO Si rate, vider toute la cmd stocker
void	Command::_fun_USER(void)
{
	if (this->_parsedCmd.size() < 5)
	{
		this->_cmdUser->setRet(this->_parsedCmd[0]);
		insert_all(this->_cmdUser->getRet(), " :Not enough parameters\r\n");
		return ;
	}
	if (this->_cmdUser->getRegistered())
	{
		insert_all(this->_cmdUser->getRet(), ":You may not reregister\r\n");
		return ;
	}
	this->_cmdUser->setUserName(this->_parsedCmd[1]);
	//	this->_cmdUser.setMode(this->_parsedCmd[2]);Some weird thing to do : RFC 2812/3.1.3
	this->_cmdUser->setRealName(this->_parsedCmd[4]);
	this->_cmdUser->setRegistered(true);
}

std::vector<std::vector<unsigned char> >	Command::getCommand(void) const
{
	return (this->_parsedCmd);
}

void	Command::_fun_JOIN(Server &my_server)
{
	//RFC 2813/4.2.1
	//TODO IF User == serverOp --> User = chanOp
	if (this->_parsedCmd.size() < 2)
	{
		this->_cmdUser->setRet(this->_parsedCmd[0]);
		insert_all(this->_cmdUser->getRet(), " :Not enough parameters\r\n");
		return ;
	}
	std::vector<std::vector<unsigned char> >	chan = splitOnComa(this->_parsedCmd[1]);


	std::vector<std::vector<unsigned char> >::iterator	it = chan.begin();
	std::vector<std::vector<unsigned char> >			passwd;
	std::vector<std::vector<unsigned char> >::iterator	itpasswd;

	if (this->_parsedCmd.size() == 3)
	{
		passwd = splitOnComa(this->_parsedCmd[2]);
		itpasswd = passwd.begin();
	}
	while (it != chan.end())
	{
		Channel	tmp = my_server.findChan(*it);
		it++;
		if (itpasswd != passwd.end())
		{
			tmp.addUser(this->_cmdUser, my_server, *itpasswd);
			itpasswd++;
		}
		else
			tmp.addUser(this->_cmdUser, my_server);
	}
}

void	Command::_fun_QUIT(Server &my_server)
{
// 	if (this->_parsedCmd.size() > 1)
// 	{
// 		ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
// 		i++;
// 		while (i < _parsedCmd.size())
// 		{
// 			ret.push_back(' ');
// 			ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
// 			i++;
// 		}
// 	}
	
	for (std::vector<Channel *>::iterator itc = _cmdUser->getChannelsbg(); itc != _cmdUser->getChannelsend(); itc++)
	{
		for (std::list<User *>::const_iterator itu = (*itc)->getUsrListbg(); itu != (*itc)->getUsrListend(); itu++)
		this->_cmdUser->getChannels().erase(itc);
	}
	
	my_server.getUsers().remove(*(this->_cmdUser));
	close(_cmdUser->getfd());
}


void	Command::_fun_RESTART(Server &my_server)
{
	if (!this->_cmdUser->getOperator())
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		insert_all(this->_cmdUser->getRet(), " :Permission Denied- You're not an IRC operator");
		return ;
	}
	free_fun(my_server);
	this->_parsedCmd.erase(this->_parsedCmd.begin(), this->_parsedCmd.end());
	my_server.init(my_server.getArgv());
}

void	Command::do_chan(std::vector<unsigned char> dest, Server &my_server, std::vector<unsigned char> msg)
{
	std::vector<unsigned char>::iterator	it = dest.begin();
	Channel									chan;
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
/*	else
	{
		dest.clear();
		dest = this->_parsedCmd[0];
		insert_all(dest, "ERRCANNOTSENDTOCHAN\r\n");
		my_server.sendto(this->_cmdUser.getfd(), dest);
		return ;
	}*/
	if (chan != *my_server.getChannel().end() && is_op)
	{
		for (std::list<User *>::iterator	itc = chan.getOpListbg(); itc != chan.getOpListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;
		}
	}
	else
	{
		for (std::list<User *>::iterator	itc = chan.getUsrListbg(); itc != chan.getUsrListend(); itc++)
		{
			(*itc)->setRet(msg);
			my_server.getEv().events = EPOLLOUT | EPOLLET;
			if (epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_MOD, (*itc)->getfd(), &my_server.getEv()) == - 1)
				return ;
		}
	}
}

void	Command::_fun_PRIVMSG(Server &my_server)
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
		insert_all(this->_cmdUser->getRet(), ":No text to send\r\n");
		return ;
	}
//PAS SUR DE CELLE CI
	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] != ':')
	{
		this->_cmdUser->setRet(this->_parsedCmd[0]);// RET SHOULD BE <TARGET>
		insert_all(this->_cmdUser->getRet(), " :Duplicate recipients. No message delivered\r\n");
		return ;
	}
	std::vector<unsigned char>	receiver = this->_parsedCmd[1];

	if (this->_parsedCmd[2][0] == ':')
		this->_cmdUser->setRet(std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end()));
	else
		this->_cmdUser->setRet(this->_parsedCmd[2]);
	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
	while (it != this->_parsedCmd.end())
	{
		this->_cmdUser->getRet().push_back(' ');
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), it->begin(), it->end());
	}
	if (*(receiver.begin()) == '+' || *(receiver.begin()) == '&' || *(receiver.begin()) == '@' || *(receiver.begin()) == '%' || *(receiver.begin()) == '~' || *(receiver.begin()) == '#')
		do_chan(receiver, my_server, this->_cmdUser->getRet());
	else
	{
		std::list<User>::iterator	itu = my_server.findUser(receiver);
		if (itu == my_server.getUsers().end())
		{
			this->_cmdUser->setRet(this->_parsedCmd[0]);
			insert_all(this->_cmdUser->getRet(), " ERR_NOSUCHNICK\r\n");
		}
	}
}

void	Command::_fun_OPER(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_MODE(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_TOPIC(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_KICK(Server &my_server)
{
	std::vector<Channel>::iterator	tmp = my_server.findExistingChan(this->_parsedCmd[1]);

	if (this->_parsedCmd.size() < 3)
	{
		insert_all(this->_cmdUser->getRet(), " ERR_NEEDMOREPARAM\r\n");
		my_server.sendto(this->_cmdUser->getfd(), this->_cmdUser->getRet());
		return ;
	}

	if (tmp == my_server.getChannel().end())
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		insert_all(this->_cmdUser->getRet(), " ERR_NOSUCHCHANNEL\r\n");
		return ;
	}

	if (!tmp->isOp(this->_cmdUser))
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), this->_parsedCmd[1].begin(), this->_parsedCmd[1].end());
		insert_all(this->_cmdUser->getRet(), " :You're not channel operator\r\n");
		return ;
	}

	std::list<User *>::iterator		Usrlst = tmp->getUsrListbg();
	while (Usrlst != tmp->getUsrListend() && *Usrlst != this->_cmdUser)
		Usrlst++;
	if (Usrlst == tmp->getUsrListend())
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmdUser->getRet(), " ERR_NOTONCHANNEL\r\n");
		return ;
	}
	Usrlst = tmp->getUsrListbg();
	while (Usrlst != tmp->getUsrListend() && !(**Usrlst == this->_parsedCmd[2]))
		Usrlst++;
	if (Usrlst == tmp->getUsrListend())
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmdUser->getRet(), " ERR_USERNOTINCHANNEL\r\n");
		return ;
	}
	if (tmp->isOp(*Usrlst))
	{
		this->_cmdUser->setRet(this->_cmdUser->getUserName());
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
		insert_all(this->_cmdUser->getRet(), " ERR_CANNOTKICKADMIN\r\n");
		return ;
	}

	for (std::vector<std::vector<unsigned char> >::iterator it = this->_parsedCmd.begin() + 3; it != this->_parsedCmd.end(); it++)
		this->_cmdUser->getRet().insert(this->_cmdUser->getRet().end(), it->begin(), it->end());
	tmp->getUsrList().erase(Usrlst);
	//Aucune idee de quoi envoyer a ce user comme notification qu'il a ete virer

}

void	Command::_fun_INVITE(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_KILL(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_PONG(Server &my_server)
{
//	std::vector<User>::iterator	it = my_server.getUsers().begin();
	(void)my_server;
}

void	Command::answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING"};
	int i = 0;

	if (this->_parsedCmd.size() == 0)
		return ;
	while (i < 15 && my_compare(this->_parsedCmd[0], options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
			this->_fun_CAP(my_server);
			break;
		case 1:
			this->_fun_USER();
			break;
		case 2:
			this->_fun_PASS(my_server);
			break;
		case 3:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_JOIN(my_server);
			break;
		case 4:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_PRIVMSG(my_server);
			break;
		case 5:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_OPER(my_server);
			break;
		case 6:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_QUIT(my_server);
			break;
		case 7:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_ERROR(my_server);
			break;
		case 8:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_MODE(my_server);
			break;
		case 9:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_TOPIC(my_server);
			break;
		case 10:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_KICK(my_server);
			break;
		case 11:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_INVITE(my_server);
			break;
		case 12:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_KILL(my_server);
			break;
		case 13:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_RESTART(my_server);
			break;
		case 14:
			if (!this->_cmdUser->getRegistered())
				break;
			this->_fun_PONG(my_server);
			break;
		default:
			break;
	}
	this->_parsedCmd.clear();
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
	this->_parsedCmd = val;
}

void Command::setCmdFdUser(int fd)
{
	_cmd_fd_user = fd;	
}
