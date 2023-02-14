/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 15:58:44 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"

class Server;

class User;

class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);

		void	parseCommand(std::string);
		
	private:
		User *										_cmdUser;
		std::vector<std::vector<unsigned char> >	_parsedCmd;
		void										_answer(Server &);
		
		void	_fun_CAP(Server &my_server);
		void	_fun_NICK(Server &my_server);
		void	_fun_USER(Server &my_server);
		void	_fun_PASS(Server &my_server);
		void	_fun_JOIN(Server &my_server);
		void	_fun_PRIVMSG(Server &my_server);
		void	_fun_OPER(Server &my_server);
		void	_fun_QUIT(Server &my_server);
		void	_fun_ERROR(Server &my_server);
		void	_fun_MODE(Server &my_server);
		void	_fun_TOPIC(Server &my_server);
		void	_fun_KICK(Server &my_server);
		void	_fun_INVITE(Server &my_server);
		void	_fun_KILL(Server &my_server);
		void	_fun_RESTART(Server &my_server);
		void	_fun_PING(Server &my_server);

		void	do_chan(std::vector<unsigned char>, Server &my_server, std::vector<unsigned char>);

};

#endif
