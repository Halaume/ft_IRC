/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 16:07:56 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <map>
#include <string>
#include "Channel.hpp"
#include <list>
#include <netinet/in.h>
#include "User.hpp"

class Server
{
	public:
		Server(void);
		Server(const Server &copy);
		~Server(void);
		Server &	operator=(const Server & src);
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
};

#endif
