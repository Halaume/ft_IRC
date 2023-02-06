/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 16:43:51 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <list>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

Command::Command(void)
{
}

Command::Command(const Command &copy)
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

//std::string	Command::_fun_CAP(Server &my_server);
//std::string	Command::_fun_NICK(Server &my_server);
void	Command::_fun_USER(Server &my_server)
{
	std::stringstream ret;

	if (this->_parsedCmd.size() < 5)
	{
		ret << this->_parsedCmd[0] << " :Not enough parameters\r\n";
		my_server.send(this->_cmdUser.getfd(), ret.str());
		return ;
	}
	if (this->_cmdUser.getRegistered())
	{
		my_server.send(this->_cmdUser.getfd(), ":You may not reregister\r\n");
		return ;
	}
	this->_cmdUser.setUserName(this->_parsedCmd[1]);
	//	this->_cmdUser.setMode(this->_parsedCmd[2]);Some weird thing to do : RFC 2812/3.1.3
	this->_cmdUser.setRealName(this->_parsedCmd[4]);
	this->_cmdUser.setRegistered(true);
}

void	Command::_fun_PASS(Server &my_server)
{
	std::stringstream ret;

	if (this->_parsedCmd.size() < 2)
	{
		ret << this->_parsedCmd[0] << " :Not enough parameters\r\n";
		my_server.send(this->_cmdUser.getfd(), ret.str());
		return ;
	}
	if (this->_cmdUser.getRegistered())
	{
		my_server.send(this->_cmdUser.getfd(), ":You may not reregister\r\n");
		return ;
	}
	this->_cmdUser.setPasswd(this->_parsedCmd[1]);
}

std::list<std::string>	splitOnComa(std::string str)
{
	std::list<std::string>	ret;
	std::string::size_type	n = 0;
	std::string::size_type	prev = 0;
	while (n != std::string::npos)
	{
		if (n != 0)
			n++;
		prev = n;
		n = str.find(',');
		if (n == std::string::npos)
			ret.insert(ret.end(), str.substr(prev, str.size() - prev));
		else
			ret.insert(ret.end(), str.substr(prev, n - 1));
	}
	return (ret);
}

void	Command::_fun_JOIN(Server &my_server)
{
	//RFC 2813/4.2.1
	/*Numeric Replies:

	  ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
	  ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
	  ERR_CHANNELISFULL               ERR_BADCHANMASK
	  ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
	  ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
	  RPL_TOPIC*/
	//RPL_TOPIC pour le new User et RPL_NAMREPLY Pour tout les users du chan (Nouvel utilisateur inclut)

	std::stringstream ret;

	if (this->_parsedCmd.size() < 2)
	{
		ret << this->_parsedCmd[0] << " :Not enough parameters\r\n";
		my_server.send(this->_cmdUser.getfd(), ret.str());
		return ;
	}
	std::list<std::string>	chan = splitOnComa(this->_parsedCmd[1]);


	std::list<std::string>::iterator	it = chan.begin();
	std::list<std::string>				passwd;
	std::list<std::string>::iterator	itpasswd;

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

//void	Command::_fun_PRIVMSG(Server &my_server);
//void	Command::_fun_OPER(Server &my_server);
//void	Command::_fun_QUIT(Server &my_server);
//void	Command::_fun_ERROR(Server &my_server);
//void	Command::_fun_MODE(Server &my_server);
//void	Command::_fun_TOPIC(Server &my_server);
//void	Command::_fun_KICK(Server &my_server);
//void	Command::_fun_INVITE(Server &my_server);
//void	Command::_fun_KILL(Server &my_server);

void	Command::_fun_RESTART(Server &my_server)
{
	(void)my_server;
	//fun free -> fun server.init() -> break le run -> fun server.run()
}

//std::string	Command::_fun_PING(Server &my_server);

void	Command::_answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING"};
	int i = 0;
	while (i < 15 && this->_parsedCmd[0].compare(options[i]) != 0)
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
			this->_fun_USER(my_server);
			break;
		}
		case 2:
		{
			this->_fun_PASS(my_server);
			break;
		}
		case 3:
		{
			this->_fun_JOIN(my_server);
			break;
		}
		case 4:
		{
			this->_fun_PRIVMSG(my_server);
			break;
		}
		case 5:
		{
			this->_fun_OPER(my_server);
			break;
		}
		case 6:
		{
			this->_fun_QUIT(my_server);
			break;
		}
		case 7:
		{
			this->_fun_ERROR(my_server);
			break;
		}
		case 8:
		{
			this->_fun_MODE(my_server);
			break;
		}
		case 9:
		{
			this->_fun_TOPIC(my_server);
			break;
		}
		case 10:
		{
			this->_fun_KICK(my_server);
			break;
		}
		case 11:
		{
			this->_fun_INVITE(my_server);
			break;
		}
		case 12:
		{
			this->_fun_KILL(my_server);
			break;
		}
		case 13:
		{
			this->_fun_RESTART(my_server);
			break;
		}
		case 14:
		{
			this->_fun_PING(my_server);
			break;
		}
		default:
		{
			my_server.send(this->_cmdUser.getfd(), "Command not found");
			break;
		}
	}
}
