/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/05 18:11:11 by madelaha         ###   ########.fr       */
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
		
		
	
			int								getSct(void) const;
			int								getEpollfd(void) const;
			std::vector<unsigned char>		getPasswd(void) const;
			
			void							getGobalCmd(Command*, std::vector<unsigned char>, int);
			void 							getParsedCmd(Command*, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >::size_type);
			void 							getParsedCmd();
			std::vector<Channel>::iterator	getChannelsbg(void);
			std::vector<Channel>::iterator	getChannelsend(void);
			std::list<User>::iterator		getUsersbg(void);
			std::list<User>::iterator		getUsersend(void);
			std::vector<Channel>			getChannel(void) const;
			epoll_event &					getEv(void);
			std::list<User>					getUsers(void) const;
			// User*						getUser(int fd);
			
			
			void 							printGlobalCommand(Command cmd);
			void 							printParsedCommand(Command cmd);
			void							printUsersList(void);
			void							printChannelsList(void);
			int								init(char **);
			void							run(void);
			int								accept_socket(int);
	
			char **							getArgv(void) const;
			std::vector<Channel>::iterator	findExistingChan(std::vector<unsigned char> channel);
			void							sendto(int, std::vector<unsigned char>);
			std::list<User>::iterator		findUser(std::vector<unsigned char> nick);
			bool							isUserInList(int);
			bool							channelExists(std::vector<unsigned char>&);
			void							addNewChannel(Channel&);
			void							send_to_client(int, std::vector<unsigned char>);	
			std::list<User>::iterator		findUser(std::string nick);
			std::list<User>::iterator		findUserNick(std::vector<unsigned char> nick);
			User*							findUserPtrNick(std::vector<unsigned char> nick);
			Channel*						findChan(std::vector<unsigned char>);
			std::list<User>::iterator		findUser(int fd);
			void							delUser(User & Usr);

			void							setBot(void);

			int								nbConnections(User &);
			int								nbConnectionsWithSameNick(User &);


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
			User						_bot;
};

std::ostream &		operator<<( std::ostream & o, Server & i);


#endif
