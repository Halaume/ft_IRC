/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 18:27:03 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"
// #include "User.hpp"

class Server;

// class User;



class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);

		void										push_to_buf(int error, std::vector<unsigned char> vector);
		
		// Getters
		unsigned char								getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type, std::vector<std::vector<unsigned char> >::size_type);
		std::vector<std::vector<unsigned char> >	getGobalCmd();
		std::vector<std::vector<unsigned char> >	getParsedCmd();

		// Setters
		void										setFdUser(int);

		
		std::vector<std::vector<unsigned char> >	_globalCmd;
		std::vector<std::vector<unsigned char> >	_parsedCmd;

		void										_answer(Server &);
		
	private:
		// User										_cmdUser;
		
		int										_fdUser;
		std::vector<unsigned char>				_cmd_buf;
		std::vector<unsigned char>::size_type	_cmd_size;
		int										_error;
		
		void									_fun_CAP(Server &my_server);
		void									_fun_NICK(Server &my_server);
		void									_fun_USER(Server &my_server);
		void									_fun_PASS(Server &my_server);
		void									_fun_JOIN(Server &my_server);
		void									_fun_PRIVMSG(Server &my_server);
		void									_fun_OPER(Server &my_server);
		void									_fun_QUIT(Server &my_server);
		void									_fun_ERROR(Server &my_server);
		void									_fun_MODE(Server &my_server);
		void									_fun_TOPIC(Server &my_server);
		void									_fun_KICK(Server &my_server);
		void									_fun_INVITE(Server &my_server);
		void									_fun_KILL(Server &my_server);
		void									_fun_RESTART(Server &my_server);
		void									_fun_PING(Server &my_server);

};

#endif
