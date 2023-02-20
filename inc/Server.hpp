/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/19 22:34:52 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <vector>
#include <string>
#include <list>
#include <ctime>

#include <netinet/in.h>

#include "Channel.hpp"
#include "User.hpp"
#include "Command.hpp"

#define BUFFER_SIZE 1

const std::time_t g_time= std::time(0);   // get time now

const struct tm  tstruct = *localtime(&g_time);

class Channel;

class User;

class Command;

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


		void							printUsersList(void);
		void							printChannelsList(void);
	
		// GETTERS	
		int								getSct(void);
		int								getEpollfd(void);
		std::vector<unsigned char>		getPasswd(void) const;
		// User*							getUser(int fd);
		void							getGobalCmd(Command*, std::vector<unsigned char>, int);
		void 							getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
		void 							getParsedCmd();
		std::list<Channel>::iterator	getChannelsbg(void);
		std::list<Channel>::iterator	getChannelsend(void);

		
		
		void 						printGlobalCommand(Command cmd);
		void 						printParsedCommand(Command cmd);

		bool						isUserInList(int);
		const std::list<User>&		getUsers(void) const;
		
		// Channels
		bool						channelExists(std::vector<unsigned char>&);
		void						addNewChannel(Channel&);

		Channel&					findChan(std::vector<unsigned char>);
		void						send_to_client(int, std::vector<unsigned char>);
		std::list<User>::iterator	findUser(std::string nick);
		std::list<User>::iterator	findUserNick(std::vector<unsigned char> nick);
		std::list<User>::iterator	findUser(int fd);

		std::list<User>					_Users;

	private:
		sockaddr_in						_server;
		int								_sct;
		std::vector<unsigned char>		_passwd;
		int								_epollfd;
		epoll_event						_events[10];
		epoll_event						_ev;
		std::list<Channel>				_channels;
		
};

std::ostream &		operator<<( std::ostream & o, Server & i);


#endif
