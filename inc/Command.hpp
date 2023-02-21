/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/21 16:05:05 by ghanquer         ###   ########.fr       */
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

class Server;

class User;



class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);
		std::vector<std::vector<unsigned char> >	getCommand(void) const;
		void		setCommand(std::vector<std::vector<unsigned char> >);
		void		setUser(User*);
		void		answer(Server &);

		void										push_to_buf(int error, std::vector<unsigned char> vector);
		
		// Getters
		unsigned char								getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type, std::vector<std::vector<unsigned char> >::size_type);
		std::vector<std::vector<unsigned char> >	getGobalCmd();
		std::vector<std::vector<unsigned char> >	getParsedCmd();
		void										setParsedCmd(std::vector<std::vector<unsigned char> >);

		// Setters
		void										setCmdFdUser(int);
		void										setCmdUser(Server &);
	
		std::vector<std::vector<unsigned char> >	_globalCmd;
		std::vector<std::vector<unsigned char> >	_parsedCmd;

	private:
		User *									_cmdUser;
		
		int										_cmd_fd_user;
		std::vector<unsigned char>				_cmd_buf;
		std::vector<unsigned char>::size_type	_cmd_size;
		int										_error;
		std::list<User>::iterator				_cmd_user;
		
		void									_fun_CAP(Server &my_server);
		void									_fun_NICK(Server &my_server);
		void									_fun_USER(void);
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
		void									_fun_PONG(Server &my_server);
		void									_fun_NOTICE(Server &my_server);

		void	do_chan(std::vector<unsigned char>, Server &my_server, std::vector<unsigned char>);

};

#endif
