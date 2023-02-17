/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/17 11:01:17 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"
// #include "User.hpp"
// #define server_name "mig.42.fr"
// std::string server_name = "mig.42.fr";

# define WAITING_FOR_PASS 0
# define PASS_ORDER_OK 1
# define PASS_ORDER_ERROR -1

class Server;

class User;



class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);

		void										push_to_buf(int error);
		
		// Getters
		unsigned char								getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type, std::vector<std::vector<unsigned char> >::size_type);
		std::vector<std::vector<unsigned char> >	getGobalCmd();
		std::vector<std::vector<unsigned char> >	getParsedCmd();
		std::list<User>::iterator					getCmdUser();

		// Setters
		void										setCmdFdUser(int);
		void										setCmdUser(Server &);
	
		std::vector<std::vector<unsigned char> >	_globalCmd;
		std::vector<std::vector<unsigned char> >	_parsedCmd;

		void										_answer(Server &);
		
	private:
		// User										_cmdUser;
		
		int										_cmd_fd_user;
		std::vector<unsigned char>				_cmd_buf;
		int										_error;
		std::list<User>::iterator				_cmd_user;
		int										_pass_before_nick_user;
		
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
