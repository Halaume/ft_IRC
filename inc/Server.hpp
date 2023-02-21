/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/21 18:40:28 by iguscett         ###   ########.fr       */
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

#define BUFFER_SIZE 1048576

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
		
		int								init(char **);
		void							run(void);
		int								accept_socket(int);


		void							printUsersList(void);
		void							printChannelsList(void);
	
		// GETTERS	
		int								getSct(void) const;
		int								getEpollfd(void) const;
		std::vector<unsigned char>		getPasswd(void) const;
		// User*							getUser(int fd);
		void							getGobalCmd(Command*, std::vector<unsigned char>, int);
		void 							getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
		void 							getParsedCmd();
		std::vector<Channel>::iterator	getChannelsbg(void);
		std::vector<Channel>::iterator	getChannelsend(void);
		std::vector<Channel>			getChannel(void) const;
		epoll_event &					getEv(void);
		
		
		void 							printGlobalCommand(Command cmd);
		void 							printParsedCommand(Command cmd);
		// GETTERS


		char **							getArgv(void) const;
		std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
		void							sendto(int, std::vector<unsigned char>);
		std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
		std::list<User>::iterator		getUsr(int);
		bool							isUserInList(int);
		
		// Channels
		bool							channelExists(std::vector<unsigned char>&);
		void							addNewChannel(Channel&);
	
		// Channel*						findChanPtr(std::vector<unsigned char>);
		void							send_to_client(int, std::vector<unsigned char>);
			
		std::list<User>::iterator		findUser(std::string nick);
		std::list<User>::iterator		findUserNick(std::vector<unsigned char> nick);
		User*							findUserPtrNick(std::vector<unsigned char> nick);
		std::list<User>					getUsers(void) const;
		Channel*						findChan(std::vector<unsigned char>);
		std::list<User>::iterator		findUser(int fd);
		std::vector<unsigned char>		getPassword(void) const;


	private:
		char **						_argv;
		sockaddr_in					_server;
		int							_sct;
		std::vector<unsigned char>	_passwd;
		int							_epollfd;
		epoll_event *				_events;
		epoll_event					_ev;
		std::vector<Channel>		_channels;
		std::list<User>				_users;

		
};

std::ostream &		operator<<( std::ostream & o, Server & i);


#endif
