/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 13:31:20 by ghanquer         ###   ########.fr       */
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

#define BUFFER_SIZE 1048576

class Channel;

class User;

class Server
{
	public:
		Server(void);
		Server(const Server &copy);
		~Server(void);
		Server &	operator=(const Server & src);
		
		int			init(char **);
		int			run(void);


		void printUsersList(void);

		// GETTERS
		std::vector<Channel>	getChannel(void) const;
		int						getSct(void) const;
		int						getEpollfd(void) const;
		// User*		getUser(int fd);

		std::list<User>					getUser(void) const;
		char **							getArgv(void) const;
		std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
		Channel &						findChan(std::vector<unsigned char>);
		void							sendto(int, std::vector<unsigned char>);
		std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
		std::list<User>::iterator		getUsr(int);

	private:
		char **					_argv;
		sockaddr_in				_server;
		int						_sct;
		char *					_passwd;
		int						_epollfd;
		epoll_event				_events[10];
		epoll_event				_ev;
		epoll_event				_evout;
		std::vector<Channel>	_channels;
		std::list<User>			_Users;
};

#endif
