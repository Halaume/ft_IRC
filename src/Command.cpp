/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 16:39:14 by iguscett         ###   ########.fr       */
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
