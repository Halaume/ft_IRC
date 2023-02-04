/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 17:28:45 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
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
//std::string	Command::_fun_USER(Server &my_server);
//std::string	Command::_fun_PASS(Server &my_server);
//std::string	Command::_fun_JOIN(Server &my_server);
//std::string	Command::_fun_PRIVMSG(Server &my_server);
//std::string	Command::_fun_OPER(Server &my_server);
//std::string	Command::_fun_QUIT(Server &my_server);
//std::string	Command::_fun_ERROR(Server &my_server);
//std::string	Command::_fun_MODE(Server &my_server);
//std::string	Command::_fun_TOPIC(Server &my_server);
//std::string	Command::_fun_KICK(Server &my_server);
//std::string	Command::_fun_INVITE(Server &my_server);
//std::string	Command::_fun_KILL(Server &my_server);
//std::string	Command::_fun_RESTART(Server &my_server);
//std::string	Command::_fun_PING(Server &my_server);

/*std::map<std::string, std::string (*fptr)(Server &)>	insert_all_cmd()
  {
  std::map<std::string, fptr>	my_map;

  my_map.insert(std::make_pair("CAP", &_fun_CAP));
  my_map.insert(std::make_pair("NICK", &_fun_NICK));
  my_map.insert(std::make_pair("USER", &_fun_USER));
  my_map.insert(std::make_pair("PASS", &_fun_PASS));
  my_map.insert(std::make_pair("JOIN", &_fun_JOIN));
  my_map.insert(std::make_pair("PRIVMSG", &_fun_PRIVMSG));
  my_map.insert(std::make_pair("OPER", &_fun_OPER));
  my_map.insert(std::make_pair("QUIT", &_fun_QUIT));
  my_map.insert(std::make_pair("ERROR", &_fun_ERROR));
  my_map.insert(std::make_pair("MODE", &_fun_MODE));
  my_map.insert(std::make_pair("TOPIC", &_fun_TOPIC));
  my_map.insert(std::make_pair("KICK", &_fun_KICK));
  my_map.insert(std::make_pair("INVITE", &_fun_INVITE));
  my_map.insert(std::make_pair("KILL", &_fun_KILL));
  my_map.insert(std::make_pair("RESTART", &_fun_RESTART));
  my_map.insert(std::make_pair("PING", &_fun_PING));

  }*/

std::string	Command::_answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING"};
	int i = 0;
	while (i < 15 && this->_parsedCmd[0].compare(options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
			return (this->_fun_CAP(my_server));
		case 1:
			return (this->_fun_USER(my_server));
		case 2:
			return (this->_fun_PASS(my_server));
		case 3:
			return (this->_fun_JOIN(my_server));
		case 4:
			return (this->_fun_PRIVMSG(my_server));
		case 5:
			return (this->_fun_OPER(my_server));
		case 6:
			return (this->_fun_QUIT(my_server));
		case 7:
			return (this->_fun_ERROR(my_server));
		case 8:
			return (this->_fun_MODE(my_server));
		case 9:
			return (this->_fun_TOPIC(my_server));
		case 10:
			return (this->_fun_KICK(my_server));
		case 11:
			return (this->_fun_INVITE(my_server));
		case 12:
			return (this->_fun_KILL(my_server));
		case 13:
			return (this->_fun_RESTART(my_server));
		case 14:
			return (this->_fun_PING(my_server));
		default:
			return ("Command not found");
	}
	return ("");
}
