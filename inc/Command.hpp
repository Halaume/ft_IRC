/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/02 17:18:36 by madelaha         ###   ########.fr       */
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
# define PASS_CONNECTION_ERROR 4



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
			std::vector<std::vector<unsigned char> >	getGobalCmd();
			std::vector<std::vector<unsigned char> >	getParsedCmd();
			User*										getCmdUser(void) const;
			unsigned char								getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type, std::vector<std::vector<unsigned char> >::size_type);
		
			void										setCommand(std::vector<std::vector<unsigned char> >);
			void										setUser(User*);
			void										setParsedCmd(std::vector<std::vector<unsigned char> >);

		// Getters

		
		std::vector<std::vector<unsigned char> >&	getRet();
		

		void										sendToChan(Server &, std::vector<Channel>::iterator, std::vector<unsigned char>);
		std::vector<unsigned char>					_globalCmd;
		std::vector<std::vector<unsigned char> >	_parsedCmd;

		int											register_user(Server &);	
		
		int											answer(Server &);


		
	private:
	
			User*									_cmd_user;
			std::vector<unsigned char>				_cmd_buf;
			int										_error;
			std::vector<std::vector<unsigned char> > _ret;

			int	_fun_PASS();	
			int	_fun_NICK(Server &);
			int	_fun_USER(Server &);
			int	_fun_JOIN(Server &);
			int	_fun_PRIVMSG(Server &);
			int	_fun_OPER(Server &);
			int	_fun_QUIT(Server &);
			int	_fun_ERROR(Server &);
			int	_fun_MODE(Server &);
			int	_fun_TOPIC(Server &);
			int	_fun_KICK(Server &);
			int	_fun_INVITE(Server &);
			int	_fun_KILL(Server &);
			int	_fun_RESTART(Server &);
			int	_fun_PONG(void);
			int	_fun_NOTICE(Server &);

			void	do_chan(std::vector<unsigned char>, Server &, std::vector<unsigned char>);

};

#endif
