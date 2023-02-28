/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 17:50:35 by madelaha         ###   ########.fr       */
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


void Command::push_to_buf(int error, std::vector<unsigned char> cmd)
{
	(void)error; (void)cmd;
	_cmd_buf.clear();
	add_to_vector(&_cmd_buf, ":" + server_name + " ");
	add_to_vector(&_cmd_buf, _cmd_user->getNickName());
	add_to_vector(&_cmd_buf, " ");
	add_to_vector(&_cmd_buf, numeric_response(ERR_NEEDMOREPARAMS, cmd));
	// add_to_vector(&_cmd_buf, cmd);
	// add_to_vector(&_cmd_buf, ERR_NEEDMOREPARAMSmsg);

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

	std::cout << "PASS COMMAND REALIZED\n";
	std::vector<unsigned char> v;
	
	if (this->_parsedCmd.size() < 2 && my_server.findUser(_cmd_fd_user)->getRegistered() == false)
	{
		std::cout << "PASS not enough params\n";
		push_to_buf(ERR_NEEDMOREPARAMS, _parsedCmd[0]);
		return;
	}
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
void	Command::_fun_USER(Server &my_server)
{
	std::vector<unsigned char> ret;

	if (_parsedCmd.size() < 5)
	{
		ret = _parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	if (_cmdUser->getRegistered())
	{
		insert_all(ret, ":You may not reregister\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	_cmdUser->setUserName(_parsedCmd[1]);
	//	this->_cmdUser->setMode(_parsedCmd[2]);Some weird thing to do : RFC 2812/3.1.3
	_cmdUser->setRealName(_parsedCmd[4]);
	_cmdUser->setRegistered(true);
}


void	Command::_fun_JOIN(Server &my_server)
{
	//RFC 2813/4.2.1
	//TODO IF User == serverOp --> User = chanOp
	std::vector<unsigned char> ret;

	if (_parsedCmd.size() < 2)
	{
		ret = _parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	std::vector<std::vector<unsigned char> >	chan = splitOnComa(_parsedCmd[1]);


	std::vector<std::vector<unsigned char> >::iterator	it = chan.begin();
	std::vector<std::vector<unsigned char> >			passwd;
	std::vector<std::vector<unsigned char> >::iterator	itpasswd;

	if (_parsedCmd.size() == 3)
	{
		passwd = splitOnComa(_parsedCmd[2]);
		itpasswd = passwd.begin();
	}
	while (it != chan.end())
	{
		Channel	tmp = my_server.findChan(*it);
		it++;
		if (itpasswd != passwd.end())
		{
			tmp.addUser(_cmdUser, my_server, *itpasswd);
			itpasswd++;
		}
		else
			tmp.addUser(_cmdUser, my_server);
	}
}


void	Command::_fun_RESTART(Server &my_server)
{
	if (!_cmdUser->getOperator())
	{
		std::vector<unsigned char> msg = _cmdUser->getNickName();
		insert_all(msg, " :Permission Denied- You're not an IRC operator");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}
	free_fun(my_server);
	_parsedCmd.erase(_parsedCmd.begin(), _parsedCmd.end());
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
		my_server.sendto(this->_cmdUser->getfd(), dest);
		return ;
	}*/
	if (chan != *my_server.getChannel().end() && is_op)
	{
		for (std::list<User *>::iterator	itc = chan.getOpListbg(); itc != chan.getOpListend(); itc++)
			my_server.sendto((*itc)->getfd(), msg);
	}
	else
	{
		for (std::list<User *>::iterator	itc = chan.getUsrListbg(); itc != chan.getUsrListend(); itc++)
			my_server.sendto((*itc)->getfd(), msg);
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
	std::vector<unsigned char> ret;
	if (_parsedCmd.size() < 3)
	{
		insert_all(ret, ":No text to send\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
//PAS SUR DE CELLE CI
	if (_parsedCmd.size() > 3 && _parsedCmd[2][0] != ':')
	{
		ret = _parsedCmd[0];// RET SHOULD BE <TARGET>
		insert_all(ret, " :Duplicate recipients. No message delivered\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	std::vector<unsigned char>	receiver = _parsedCmd[1];

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
		{
			ret = _parsedCmd[0];
			insert_all(ret, " ERR_NOSUCHNICK\r\n");
			my_server.sendto(_cmdUser->getfd(), ret);
		}
		else
			my_server.sendto(itu->getfd(), ret);
	}
}


void	Command::_fun_NOTICE(Server &my_server)
{
	if (_parsedCmd.size() < 3)
		return ;
	if (_parsedCmd.size() > 3 && _parsedCmd[2][0] != ':')
		return ;
	
	std::vector<unsigned char>	receiver = _parsedCmd[1];
	std::vector<unsigned char> ret;
	
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
			return ;
		else
			my_server.sendto(itu->getfd(), ret);
	}
}

void	Command::_fun_OPER(Server &my_server)
{
	std::vector<unsigned char> ret;
	
	if (_parsedCmd.size() == 3)
	{
		std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
		if (itu == my_server.getUsers().end())
		{
			insert_all(ret, " ERR_PASSWMISMTCH\r\n");
			my_server.sendto(_cmdUser->getfd(),ret);
			return ;
		}
		else 
		{
			if (_parsedCmd[2] != my_server.getPassword())
			{
				insert_all(ret, " ERR_PASSWMISMTCH\r\n");
				my_server.sendto(_cmdUser->getfd(),ret);
				return ;
			}
		}
	}
	insert_all(ret, " ERR_PASSWMISMATCH\r\n");
	my_server.sendto(_cmdUser->getfd(), ret);
	return ;
}

void	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;	
}

void	Command::_fun_MODE(Server &my_server)
{
	std::vector<unsigned char> ret;

	std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[1]);
	if (itu == my_server.getUsers().end())
	{
		if (itc == my_server.getChannel().end())
		{
			insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
			my_server.sendto(_cmdUser->getfd(), ret);
			return ;
		}
		insert_all(ret, "ERR_NOSUCHNICK\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	if (_parsedCmd[1] != _cmdUser->getNickName())
	{
		insert_all(ret, " ERR_USERSDONTMATCH\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}

	//if (itc->getModes().find('i')->second 

	
	
	
	(void)my_server;
}


void	Command::_fun_INVITE(Server &my_server)
{
	std::vector<unsigned char>	ret;

	if (_parsedCmd.size() > 3)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[2]);
	if (itc == my_server.getChannel().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	std::list<User *>::iterator	itu = itc->findUser(_cmdUser->getNickName());
	if (itu == itc->getUsrListend())
	{
		insert_all(ret, " ERR_NOTONCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	itu = itc->findUser(_parsedCmd[1]);
	if (itu != itc->getUsrListend())
	{
		insert_all(ret, " ERR_USERONCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	if (itc->getModes().find('i')->second && !itc->isOp(_cmdUser))
	{
		insert_all(ret, " ERR_CHANOPRIVSNEEDED\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	insert_all(ret, " RPL_INVITING\r\n");
	my_server.sendto(_cmdUser->getfd(), ret);
	return ;
}

void	Command::_fun_QUIT(Server &my_server)
{
	std::vector<unsigned char> ret;
	unsigned long i = 1;
	
	if (_parsedCmd.size() > 1)
	{
		ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
		i++;
		while (i < _parsedCmd.size())
		{
			ret.push_back(' ');
			ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
			i++;
		}
	}
	
	for (std::vector<Channel *>::iterator itc = _cmdUser->getChannelsbg(); itc != _cmdUser->getChannelsend(); itc++)
	{
		for (std::list<User *>::iterator itu = (*itc)->getUsrListbg(); itu != (*itc)->getUsrListend(); itu++)
			my_server.sendto((*itu)->getfd(), ret);
		_cmdUser->getChannels().erase(itc);
	}
	my_server.getUsers().remove(*_cmdUser);
	close(_cmdUser->getfd());
	
}


void	Command::_fun_TOPIC(Server &my_server)
{
	std::vector<unsigned char> 		ret;
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[1]);
	std::list<User *>::iterator		Usrlst = itc->getUsrListbg();

	ret = _parsedCmd[0];
	if (_parsedCmd.size() < 2)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	if (itc == my_server.getChannel().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	while (Usrlst != itc->getUsrListend() && *Usrlst != this->_cmdUser)
        Usrlst++;
    if (Usrlst == itc->getUsrListend())
    {
        ret = _cmdUser->getNickName();
        ret.insert(ret.end(), _parsedCmd[2].begin(), _parsedCmd[2].end());
        insert_all(ret, " ERR_NOTONCHANNEL\r\n");
        my_server.sendto(_cmdUser->getfd(), ret);
        return ;
    }
	
	if (itc->isOp(*Usrlst) == false)
	{
		insert_all(ret, " ERR_CHOPRIVSNEEDED\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	
	std::vector<unsigned char>	Topic = itc->getTopic();
	if (_parsedCmd.size() == 2)
	{
		if (Topic.size() > 0)
			insert_all(ret, " RPL_TOPIC\r\n");
		else
			insert_all(ret, " RPL_NOTOPIC\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		return ;
	}
	else if (_parsedCmd.size() == 3 && _parsedCmd[2].size() == 1 && _parsedCmd[2][0] == ':')
	{
		Topic.clear();
		itc->setTopic(Topic);
		insert_all(ret, " RPL_TOPIC\r\n");
		my_server.sendto(_cmdUser->getfd(), ret);
		for (std::list<User *>::iterator itu = itc->getUsrListbg(); itu != itc->getUsrListend(); itu++)
			my_server.sendto((*itu)->getfd(), ret);
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
		for (std::list<User *>::iterator itu = itc->getUsrListbg(); itu != itc->getUsrListend(); itu++)
			my_server.sendto((*itu)->getfd(), ret);
	}
}

void	Command::_fun_KICK(Server &my_server)
{
	std::vector<Channel>::iterator	tmp = my_server.findExistingChan(_parsedCmd[1]);
	std::vector<unsigned char>		msg;

	if (_parsedCmd.size() < 3)
	{
		insert_all(msg, " ERR_NEEDMOREPARAM\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}

	if (tmp == my_server.getChannel().end())
	{
		msg = _cmdUser->getNickName();
		insert_all(msg, " ERR_NOSUCHCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}

	if (!tmp->isOp(_cmdUser))
	{
		msg = _cmdUser->getNickName();
		msg.insert(msg.end(), _parsedCmd[1].begin(), _parsedCmd[1].end());
		insert_all(msg, " :You're not channel operator\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}

	std::list<User *>::iterator		Usrlst = tmp->getUsrListbg();
	while (Usrlst != tmp->getUsrListend() && *Usrlst != _cmdUser)
		Usrlst++;
	if (Usrlst == tmp->getUsrListend())
	{
		msg = _cmdUser->getNickName();
		msg.insert(msg.end(), _parsedCmd[2].begin(), _parsedCmd[2].end());
		insert_all(msg, " ERR_NOTONCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}
	Usrlst = tmp->getUsrListbg();
	while (Usrlst != tmp->getUsrListend() && !(**Usrlst == _parsedCmd[2])) // TODO A CHANGER
		Usrlst++;
	if (Usrlst == tmp->getUsrListend())
	{
		msg = _cmdUser->getNickName();
		msg.insert(msg.end(), _parsedCmd[2].begin(), _parsedCmd[2].end());
		insert_all(msg, " ERR_USERNOTINCHANNEL\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}
	if (tmp->isOp(*Usrlst))
	{
		msg = _cmdUser->getNickName();
		msg.insert(msg.end(), _parsedCmd[2].begin(), _parsedCmd[2].end());
		insert_all(msg, " ERR_CANNOTKICKADMIN\r\n");
		my_server.sendto(_cmdUser->getfd(), msg);
		return ;
	}

	for (std::vector<std::vector<unsigned char> >::iterator it = _parsedCmd.begin() + 3; it != _parsedCmd.end(); it++)
		msg.insert(msg.end(), it->begin(), it->end());
	tmp->getUsrList().erase(Usrlst);//Retire le User du chann
									//Aucune idee de quoi envoyer a ce user comme notification qu'il a ete virer
	
}

void	Command::_fun_KILL(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_PONG(Server &my_server)
{
	(void)my_server;
}


void	Command::_answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING", "NOTICE"};
	int i = 0;
	setCmdUser(my_server);
	// _cmd_user = my_compare.findUser(_cmd_fd_user);

	std::cout << "find user for fd: " << _cmd_fd_user << " result:" << *_cmd_user << std::endl;

	
	while (i < 16 && my_compare(_parsedCmd[0], options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
		{
			_fun_CAP(my_server);
			break;
		}
		case 1:
		{
			_fun_USER(my_server);
			break;
		}
		case 2:
		{
			_fun_PASS(my_server);
			break;
		}
		case 3:
		{
			_fun_JOIN(my_server);
			break;
		}
		case 4:
		{
			_fun_PRIVMSG(my_server);
			break;
		}
		case 5:
		{
			_fun_OPER(my_server);
			break;
		}
		case 6:
		{
			_fun_QUIT(my_server);
			break;
		}
		case 7:
		{
			_fun_ERROR(my_server);
			break;
		}
		case 8:
		{
			_fun_MODE(my_server);
			break;
		}
		case 9:
		{
			_fun_TOPIC(my_server);
			break;
		}
		case 10:
		{
			_fun_KICK(my_server);
			break;
		}
		case 11:
		{
			_fun_INVITE(my_server);
			break;
		}
		case 12:
		{
			_fun_KILL(my_server);
			break;
		}
		case 13:
		{
			_fun_RESTART(my_server);
			break;
		}
		case 14:
		{
			_fun_PONG(my_server);
			break;
		}
		case 15:
		{
			_fun_NOTICE(my_server);
			break;
		}
		default:
			break;
	}
}

unsigned char	Command::getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type i, std::vector<std::vector<unsigned char> >::size_type j)
{
	return (_parsedCmd[i][j]);
}

std::vector<std::vector<unsigned char> >	Command::getParsedCmd()
{
	return (_parsedCmd);
}

void	Command::setCmdFdUser(int fd)
{
	_cmd_fd_user = fd;	
}

void	Command::setCommand(std::vector<std::vector<unsigned char> > cmd)
{
	_parsedCmd = cmd;
}

void	Command::setUser(User* usr)
{
	_cmdUser = usr;
}

void	Command::setCmdUser(Server &my_server)
{
	_cmd_user = my_server.findUser(_cmd_fd_user);
}