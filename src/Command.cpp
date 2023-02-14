/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 14:37:35 by iguscett         ###   ########.fr       */
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
#include "../inc/utils.hpp"

std::string server_name = "mig.42.fr";

Command::Command(void):  _globalCmd(), _parsedCmd(), _fdUser(), _cmd_buf(), _cmd_size(0), _error(0)
{
}

Command::Command(const Command &copy): _globalCmd(copy._globalCmd), _parsedCmd(copy._parsedCmd), _fdUser(copy._fdUser) \
, _cmd_buf(copy._cmd_buf), _cmd_size(copy._cmd_size), _error(copy._error)
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

void Command::push_to_buf(int error, std::vector<unsigned char> vector, std::string msg)
{
	(void)vector; (void)error;
	std::string str;

	_error = error;
	// _cmd_size = vector.size();
	_cmd_buf += ":";
	_cmd_buf += server_name;
	_cmd_buf += " ";
	_cmd_buf += itos(error);
	_cmd_buf += " ";
	// _cmd_buf += client;
	_cmd_buf += " ";
	for (std::vector<unsigned char>::size_type i = 0; i < vector.size(); i++)
		_cmd_buf.push_back((char)vector[i]);
	_cmd_buf += " :";
	_cmd_buf += msg;
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
	// 1 verify if host is already registered
	// 2 verify enough arguments
	// 3 ver
	// std::vector<unsigned char> ret;
	// std::string ret_str;
	// unsigned char ret[];

	std::cout << "PASS COMMAND REALIZED\n";
	(void)my_server;
	// std::cout << "vtos:" << v_to_str(this->_parsedCmd[0]) << std::endl;
	if (this->_parsedCmd.size() < 2)
	{
		_error = 461;
		// push_to_buf(461, _parsedCmd[0], "Not enough parameters\r\n");
		// std::cout << "Buf:" << _cmd_buf << std::endl;
		// my_server.send(_fdUser, ret_str);
		return ;		
	}
	else if (this->_cmdUser.getRegistered())
	{
		_error = 462;
		// ret = server_response("iguscett", "462", v_to_str(_parsedCmd[0]), "You may not reregister\r\n");
		// ret_str = v_to_str(ret);
		// std::cout << "Ret:" << ret_str << std::endl;
		// my_server.send(_fdUser, ret_str);
		return ;
	}
	else if (this->_cmdUser.setPasswd(this->_parsedCmd[1]))
	{
		_error = 464;
		return;
	}
}

void	Command::_fun_NICK(Server &my_server)
{
	(void)my_server;
}


// void	Command::_fun_USER(Server &my_server)
// {
// 	std::vector<unsigned char> ret;

// 	if (this->_parsedCmd.size() < 5)
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " :Not enough parameters\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	if (this->_cmdUser.getRegistered())
// 	{
// 		insert_all(ret, ":You may not reregister\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	this->_cmdUser.setUserName(this->_parsedCmd[1]);
// 	//	this->_cmdUser.setMode(this->_parsedCmd[2]);Some weird thing to do : RFC 2812/3.1.3
// 	this->_cmdUser.setRealName(this->_parsedCmd[4]);
// 	this->_cmdUser.setRegistered(true);
// }




// void	Command::_fun_JOIN(Server &my_server)
// {
// 	//RFC 2813/4.2.1
// 	/*Numeric Replies:

// 	  ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
// 	  ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
// 	  ERR_CHANNELISFULL               ERR_BADCHANMASK
// 	  ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
// 	  ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
// 	  RPL_TOPIC*/
// 	//RPL_TOPIC pour le new User et RPL_NAMREPLY Pour tout les users du chan (Nouvel utilisateur inclut)

// 	std::vector<unsigned char> ret;

// 	if (this->_parsedCmd.size() < 2)
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " :Not enough parameters\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	std::vector<std::vector<unsigned char> >	chan = splitOnComa(this->_parsedCmd[1]);


// 	std::vector<std::vector<unsigned char> >::iterator	it = chan.begin();
// 	std::vector<std::vector<unsigned char> >				passwd;
// 	std::vector<std::vector<unsigned char> >::iterator	itpasswd;

// 	if (this->_parsedCmd.size() == 3)
// 	{
// 		passwd = splitOnComa(this->_parsedCmd[2]);
// 		itpasswd = passwd.begin();
// 	}
// 	while (it != chan.end())
// 	{
// 		Channel	tmp = my_server.findChan(*it);
// 		it++;
// 		if (itpasswd != passwd.end())
// 		{
// 			tmp.addUser(this->_cmdUser, my_server, *itpasswd);
// 			itpasswd++;
// 		}
// 		else
// 			tmp.addUser(this->_cmdUser, my_server);
// 	}
// }



// void	Command::_fun_QUIT(Server &my_server)
// {
// 	std::vector<unsigned char> ret;
// 	unsigned long i = 1;
	
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
	
// 	for (std::vector<Channel>::iterator itc = _cmdUser.getChannels().begin(); itc != _cmdUser.getChannels().end(); itc++)
// 	{
// 		for (std::list<User>::iterator itu = itc->getUsers().begin(); itu != itc->getUsers().end(); itu++)
// 			my_server.send(itu->getfd(), ret);
// 		_cmdUser.getChannels().erase(itc);
// 	}
	
// 	my_server.getUser().remove(this->_cmdUser);
// 	close(_cmdUser.getfd());
	
// }


// void	Command::_fun_RESTART(Server &my_server)
// {
// 	(void)my_server;
// 	//fun free -> fun server.init() -> break le run -> fun server.run()
// }


// void	Command::_fun_PRIVMSG(Server &my_server)
// {
// 	//RFC2812
// 	/*Numeric Replies:

//            ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//            ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//            ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//            ERR_NOSUCHNICK
//            RPL_AWAY*/
// 	std::vector<unsigned char> ret;
// 	if (this->_parsedCmd.size() < 3)
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " :Not enough parameters\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// //PAS SUR DE CELLE CI
// 	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] == ':')
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " ERR_TOMANYTARGETS\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	std::vector<unsigned char>	receiver = this->_parsedCmd[1];
// 	std::vector<unsigned char>	msg;
// 	if (this->_parsedCmd[2][0] == ':')
// 		msg = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
// 	else
// 		msg = this->_parsedCmd[2];
// 	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
// 	while (it != this->_parsedCmd.end())
// 	{
// 		msg.push_back(' ');
// 		msg.insert(msg.end(), it->begin(), it->end());
// 	}

// 	std::list<User>::iterator	it_receiver = my_server.findUser(receiver);
// 	if (it_receiver == my_server.getUser().end())
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " ERR_NOSUCHNICK\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	(void)my_server;
// }

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
	(void)my_server;
}

void	Command::_fun_INVITE(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_KILL(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_PING(Server &my_server)
{
//	std::vector<User>::iterator	it = my_server.getUsers().begin();
	(void)my_server;
}

void	Command::_answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING"};
	int i = 0;
	
	// std::vector<std::vector<unsigned char> >::size_type m;
	// std::cout << "Command answer function parsed command[0]:";
	// for (m = 0; m < _parsedCmd[0].size(); m++)
	// 	std::cout << _parsedCmd[0][m] ;
	// std::cout << "|EOF" << std::endl;

	
	while (i < 15 && my_compare(this->_parsedCmd[0], options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
		{
			this->_fun_CAP(my_server);
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			this->_fun_PASS(my_server);
			break;
		}
		// case 3:
		// {
		// 	this->_fun_JOIN(my_server);
		// 	break;
		// }
		// case 4:
		// {
		// 	this->_fun_PRIVMSG(my_server);
		// 	break;
		// }
		// case 5:
		// {
		// 	this->_fun_OPER(my_server);
		// 	break;
		// }
		// case 6:
		// {
		// 	this->_fun_QUIT(my_server);
		// 	break;
		// }
		// case 7:
		// {
		// 	this->_fun_ERROR(my_server);
		// 	break;
		// }
		// case 8:
		// {
		// 	this->_fun_MODE(my_server);
		// 	break;
		// }
		// case 9:
		// {
		// 	this->_fun_TOPIC(my_server);
		// 	break;
		// }
		// case 10:
		// {
		// 	this->_fun_KICK(my_server);
		// 	break;
		// }
		// case 11:
		// {
		// 	this->_fun_INVITE(my_server);
		// 	break;
		// }
		// case 12:
		// {
		// 	this->_fun_KILL(my_server);
		// 	break;
		// }
		// case 13:
		// {
		// 	this->_fun_RESTART(my_server);
		// 	break;
		// }
		// case 14:
		// {
		// 	this->_fun_PING(my_server);
		// 	break;
		// }
		default:
			break;
	}
}


// Getters
unsigned char Command::getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type i, std::vector<std::vector<unsigned char> >::size_type j)
{
	return (_parsedCmd[i][j]);
}
// std::vector<std::vector<unsigned char> > Command::getGobalCmd()
// {
// 	return (_globalCmd);
// }

std::vector<std::vector<unsigned char> > Command::getParsedCmd()
{
	return (_parsedCmd);
}

// Setters
void Command::setFdUser(int fd)
{
	_fdUser = fd;	
}
