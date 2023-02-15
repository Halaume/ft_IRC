/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 17:52:57 by ghanquer         ###   ########.fr       */
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

#define BUFFER_SIZE 1048576

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


		void						printUsersList(void);

		// GETTERS
		std::vector<Channel>	getChannel(void) const;
		int						getSct(void) const;
		int						getEpollfd(void) const;

		char **							getArgv(void) const;
		std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
		void							sendto(int, std::vector<unsigned char>);
		std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
		std::list<User>::iterator		getUsr(int);

		void						getGobalCmd(Command*, std::vector<unsigned char>, int);
		void 						getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
		
		void 						printGlobalCommand(Command cmd);
		void 						printParsedCommand(Command cmd);

		bool						isUserInList(int);

		std::list<User>				getUsers(void) const;
		Channel &					findChan(std::vector<unsigned char>);
		std::list<User>::iterator	findUser(int fd);

	private:
		char **					_argv;
		sockaddr_in				_server;
		int						_sct;
		std::vector<unsigned char>	_passwd;
		int						_epollfd;
		epoll_event				_events[10];
		epoll_event				_ev;
		std::vector<Channel>	_channels;
		std::list<User>			_Users;

		
};

#endif
