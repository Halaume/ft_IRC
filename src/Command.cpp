/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/07 17:47:36 by iguscett         ###   ########.fr       */
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

Command &	operator=(const Command & src)
{
    if (&src == this)
		return (*this);
	return (*this);
}

void Command::parseCommand(std::string input)
{
	std::string commandline;
	std::string striterator;
	// std::vector<std::string> v;

	_parsedCmd.clear();
	striterator = input;
	while (striterator != "")
	{
		commandline = striterator.substr(0, striterator.find("\r\n"));
		striterator = striterator.substr(striterator.find("\r\n")+2, striterator.length());
		_parsedCmd.push_back(commandline);
		// while (commandline != "")
		// {
		// 	token = commandline.substr(0, commandline.find(" "));
		// 	v.push_back(token);
		// 	if (commandline.substr(commandline.find(" ") +1, commandline.length()).length() == commandline.length())
		// 		commandline = "";
		// 	else
		// 		commandline = commandline.substr(commandline.find(" ") +1, commandline.length());

		std::cout << "Parsed cmd:\n____________\n";
		for (int i = 0; i < (int)_parsedCmd.size(); i++)
			std::cout << "line " << i << _parsedCmd[i] << "\n";

		// }
	}
	
	// std::string str;

	// while (*input)
	// {
	// 	if (*(input + 1) && )
	// 	input++;
	// }

	
}

std::string	Command::fun_CAP(Server &my_server);
std::string	Command::fun_NICK(Server &my_server);
std::string	Command::fun_USER(Server &my_server);
std::string	Command::fun_PASS(Server &my_server);
std::string	Command::fun_JOIN(Server &my_server);
std::string	Command::fun_PRIVMSG(Server &my_server);
std::string	Command::fun_OPER(Server &my_server);
std::string	Command::fun_QUIT(Server &my_server);
std::string	Command::fun_ERROR(Server &my_server);
std::string	Command::fun_MODE(Server &my_server);
std::string	Command::fun_TOPIC(Server &my_server);
std::string	Command::fun_KICK(Server &my_server);
std::string	Command::fun_INVITE(Server &my_server);
std::string	Command::fun_KILL(Server &my_server);
std::string	Command::fun_RESTART(Server &my_server);
std::string	Command::fun_PING(Server &my_server);
