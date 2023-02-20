/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 16:29:16 by madelaha         ###   ########.fr       */
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

class Server
{
	public:
			Server(void);
			Server(const Server &copy);
			~Server(void);
			Server &	operator=(const Server & src);
		
			std::vector<Channel>		getChannel(void) const;
			std::list<User>				getUsers(void) const;
			std::list<User>::iterator	getUsr(int);
			std::vector<unsigned char>	getPassword(void) const;
			int							getSct(void) const;
			int							getEpollfd(void) const;
			void						getGobalCmd(Command*, std::vector<unsigned char>, int);
			void 						getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
			char **						getArgv(void) const;
	
			std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
			std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
			std::list<User>::iterator		findUser(int fd);
			Channel &						findChan(std::vector<unsigned char>);
			void							sendto(int, std::vector<unsigned char>);
			void 							printGlobalCommand(Command cmd);
			void 							printParsedCommand(Command cmd);
			void							run(void);
			void							printUsersList(void);
			bool							isUserInList(int);
			int								init(char **);

	private:
			char **						_argv;
			sockaddr_in					_server;
			int							_sct;
			std::vector<unsigned char>	_passwd;
			int							_epollfd;
			epoll_event *				_events;
			epoll_event					_ev;
			std::vector<Channel>		_channels;
			std::list<User>				_Users;	
};

#endif
