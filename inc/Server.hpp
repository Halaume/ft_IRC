/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 17:59:02 by madelaha         ###   ########.fr       */
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
	
		// GETTERS
		
		int							getSct(void);
		int							getEpollfd(void);
		std::vector<Channel>		getChannels(void) const;
		std::list<User>				getUser(void) const;
		std::vector<unsigned char>	getPassword(void) const;

		// User*		getUser(int fd);

		Channel &						findChan(std::vector<unsigned char>);
		std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
		std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
		
		int		init(char **);
		int		run(void);
		void	send(int, std::vector<unsigned char>);
		void 	printUsersList(void);


	private:
		sockaddr_in						_server;
		int								_sct;
		std::vector<unsigned char>		_passwd;
		int								_epollfd;
		epoll_event						_events[10];
		epoll_event						_ev;
		std::vector<Channel>			_channels;
		std::list<User>					_Users;
};

#endif
