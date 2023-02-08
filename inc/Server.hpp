/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/08 15:32:27 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <vector>
#include <string>
#include "Channel.hpp"
#include <list>
#include <netinet/in.h>
#include "User.hpp"

class Channel;
class User;

class Server
{
	public:
		Server(void);
		Server(const Server &copy);
		~Server(void);
		Server &	operator=(const Server & src);
		Channel &	findChan(std::string);
		int			init(char **);
		int			run(void);
		void		send(int fd, std::vector<unsigned char>);
		std::list<User> getUsers(void);
		
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
