/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/03 17:09:57 by iguscett         ###   ########.fr       */
=======
/*   Updated: 2023/02/04 16:07:56 by iguscett         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <map>
#include <string>
#include "Channel.hpp"
<<<<<<< HEAD
#include <vector>
=======
#include <list>
#include <netinet/in.h>
#include "User.hpp"
>>>>>>> main

class Server
{
	public:
		Server(void);
		// Server(const Server &copy);
		~Server(void);
		Server &	operator=(const Server & src);
<<<<<<< HEAD
		
		// GETTERS
		std::vector<std::string> getCommands(void);
		
	private:
		std::vector<std::string>		_commands;
=======
		int			init(char **);
		int			run(void);
	private:
		sockaddr_in						_server;
		int								_sct;
		char *							_passwd;
		int								_epollfd;
		epoll_event						_events[10];
		epoll_event						_ev;
		std::map<Channel, std::string>	_channels;
		std::list<User>					_Users;
>>>>>>> main
};

#endif
