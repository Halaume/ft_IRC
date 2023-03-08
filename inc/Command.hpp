/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 16:51:13 by iguscett         ###   ########.fr       */
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

# define MAX_NB_CHAN 10

# define WAITING_FOR_PASS 0
# define PASS_ORDER_OK 1
# define PASS_ORDER_ERROR -1
# define PASS_NICK_OK 2
# define PASS_USER_OK 3
# define PASS_USER_NICK_OK 4
# define PASS_CONNECTION_ERROR 5
# define PASS_OK_NICKINUSE 6



class Server;

class User;



class Command
{
	public:
		Command(void);
		Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);


		void										setCommand(std::vector<std::vector<unsigned char> >);
		void										setUser(User*);
		void										setParsedCmd(std::vector<std::vector<unsigned char> >);

		// Getters
		std::vector<std::vector<unsigned char> >	getCommand(void) const;
		User*										getCmdUser(void) const;
		unsigned char								getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type, std::vector<std::vector<unsigned char> >::size_type);
		std::vector<std::vector<unsigned char> >	getGobalCmd();
		std::vector<std::vector<unsigned char> >	getParsedCmd();
		std::vector<std::vector<unsigned char> >&	getRet();
		

		void										sendToChan(Server &, Channel *, std::vector<unsigned char>);
		std::vector<unsigned char>					_globalCmd;
		std::vector<std::vector<unsigned char> >	_parsedCmd;

		int											register_user(Server &);	
		
		int											answer(Server &);
		std::vector<unsigned char>					concat_parsedCmd(std::vector<std::vector<unsigned char> >::size_type i);


		
	private:
		User*									_cmd_user;
		std::vector<unsigned char>				_cmd_buf;
		int										_error;
		// std::list<User>::iterator				_cmd_user;
		std::vector<std::vector<unsigned char> > _ret;

		
		int										_fun_PASS(void);
		int										_fun_NICK(Server &);
		int										_fun_USER(Server &);
		int										_fun_JOIN(Server &);
		void									_fun_PRIVMSG(Server &);
		int										_fun_OPER(Server &);
		void									_fun_QUIT(Server &);
		void									_fun_ERROR(Server &);
		int										_fun_MODE(Server &);
		void									_fun_TOPIC(Server &);
		void									_fun_KICK(Server &);
		int										_fun_INVITE(Server &);
		void									_fun_KILL(Server &);
		void									_fun_RESTART(Server &);
		void									_fun_PONG(void);
		void									_fun_NOTICE(Server &);

		void									do_chan(std::vector<unsigned char>, Server &, std::vector<unsigned char>);

};

#endif
