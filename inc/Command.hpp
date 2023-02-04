/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 17:36:27 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Server;

class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);
		
	private:
		int							_fdCmd;
		User						_cmdUser;
		std::vector<std::string>	_parsedCmd;
		
		std::string	fun_CAP(Server &my_server);
		std::string	fun_NICK(Server &my_server);
		std::string	fun_USER(Server &my_server);
		std::string	fun_PASS(Server &my_server);
		std::string	fun_JOIN(Server &my_server);
		std::string	fun_PRIVMSG(Server &my_server);
		std::string	fun_OPER(Server &my_server);
		std::string	fun_QUIT(Server &my_server);
		std::string	fun_ERROR(Server &my_server);
		std::string	fun_MODE(Server &my_server);
		std::string	fun_TOPIC(Server &my_server);
		std::string	fun_KICK(Server &my_server);
		std::string	fun_INVITE(Server &my_server);
		std::string	fun_KILL(Server &my_server);
		std::string	fun_RESTART(Server &my_server);
		std::string	fun_PING(Server &my_server);

};

#endif
