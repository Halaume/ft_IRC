/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/13 15:52:35 by madelaha         ###   ########.fr       */
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

#define BUFFER_SIZE 3000

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
		int			getSct(void);
		int			getEpollfd(void);
		// User*		getUser(int fd);

		std::list<User>				getUser(void) const;
		Channel &					findChan(std::vector<unsigned char>);
		std::vector<Channel>::iterator   findExistingChan(std::vector<unsigned char> channel);
		void						send(int, std::vector<unsigned char>);
		std::list<User>::iterator	findUser(std::vector<unsigned char> nick);
		std::vector<Channel>         getChannels(void) const;

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
