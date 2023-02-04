/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 17:28:46 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "Channel.hpp"
#include "User.hpp"
#include <vector>
#include "Server.hpp"

class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);
		
	private:
		User						_cmdUser;
		std::vector<std::string>	_parsedCmd;
		std::string					_answer(Server &);
		
		std::string	_fun_CAP(Server &my_server);
		std::string	_fun_NICK(Server &my_server);
		std::string	_fun_USER(Server &my_server);
		std::string	_fun_PASS(Server &my_server);
		std::string	_fun_JOIN(Server &my_server);
		std::string	_fun_PRIVMSG(Server &my_server);
		std::string	_fun_OPER(Server &my_server);
		std::string	_fun_QUIT(Server &my_server);
		std::string	_fun_ERROR(Server &my_server);
		std::string	_fun_MODE(Server &my_server);
		std::string	_fun_TOPIC(Server &my_server);
		std::string	_fun_KICK(Server &my_server);
		std::string	_fun_INVITE(Server &my_server);
		std::string	_fun_KILL(Server &my_server);
		std::string	_fun_RESTART(Server &my_server);
		std::string	_fun_PING(Server &my_server);

};

#endif
