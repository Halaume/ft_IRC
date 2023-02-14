/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/13 22:04:47 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <vector>
#include <string>
#include <list>

#include <netinet/in.h>

#include "Channel.hpp"
#include "User.hpp"
#include "Command.hpp"

#define BUFFER_SIZE 1

class Channel;

class User;

// class Command;

class Server
{
	public:
		Server(void);
		Server(const Server &copy);
		~Server(void);
		Server &	operator=(const Server & src);
		
		int			init(char **);
		int			run(void);
		int			accept_socket(int);


		void printUsersList(void);

		// GETTERS
		int			getSct(void);
		int			getEpollfd(void);
		// User*		getUser(int fd);

		void						getGobalCmd(Command*, std::vector<unsigned char>, int);
		void 						getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
		
		void 						printGlobalCommand(Command cmd);
		void 						printParsedCommand(Command cmd);

		std::list<User>				getUser(void) const;
		Channel &					findChan(std::vector<unsigned char>);
		void						send(int, std::string);
		std::list<User>::iterator	findUser(std::vector<unsigned char> nick);

	private:
		sockaddr_in						_server;
		int								_sct;
		char *							_passwd;
		int								_epollfd;
		epoll_event						_events[10];
		epoll_event						_ev;
		std::vector<Channel>			_channels;
		std::list<User>					_Users;
};

#endif
