/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 17:50:50 by ghanquer         ###   ########.fr       */
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
		void		run(void);
		// GETTERS
		std::vector<Channel>	getChannel(void) const;
		int						getSct(void) const;
		int						getEpollfd(void) const;
		epoll_event &			getEv(void);

		char **							getArgv(void) const;
		std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
		void							sendto(int, std::vector<unsigned char>);
		std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
		std::list<User>::iterator		getUsr(int);
		bool						isUserInList(int);

		std::list<User>				getUsers(void) const;
		Channel &					findChan(std::vector<unsigned char>);
		std::list<User>::iterator	findUser(int fd);
		std::vector<unsigned char>	getPassword(void) const;


	private:
		char **					_argv;
		sockaddr_in				_server;
		int						_sct;
		std::vector<unsigned char>	_passwd;
		int						_epollfd;
		epoll_event *			_events;
		epoll_event				_ev;
		std::vector<Channel>	_channels;
		std::list<User>			_Users;

		
};

#endif
