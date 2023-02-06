/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 16:36:19 by ghanquer         ###   ########.fr       */
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
		void		send(int, std::string);
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
