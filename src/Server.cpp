/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/07 19:41:50 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <fcntl.h>
// #include "../inc/Command.hpp"

Server::Server(void): _server(), _sct(), _epollfd(), _ev(), _channels(), _users_list()
{
	_passwd = NULL;
}

Server::Server(const Server & copy): _server(copy._server), _sct(copy._sct),_epollfd(copy._epollfd), _ev(copy._ev),_channels(copy._channels), _users_list(copy._users_list)
{
	_passwd = copy._passwd;
}

Server::~Server(void)
{
}

Server &	Server::operator=(const Server & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

int	Server::init(char **argv)
{
	// TO DO: always protect close functions
	_sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM|SOCK_NONBLOCK
	if (_sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1); 
	bzero(&_server, sizeof(_server));
	
	_passwd = argv[2];

	_server.sin_addr.s_addr = INADDR_ANY;
	_server.sin_family = AF_INET;
	_server.sin_port = htons(atoi(argv[1])); // check if negative? or any other char
	if (_server.sin_port == 0)
		return (close(_sct), std::cerr << "Error on port" << std::endl, 1);

	if (bind(_sct, (sockaddr *)(&_server), sizeof(_server)))//server_info->ai_addrlen))
		return (close(_sct), std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(_sct, 1) == -1)
		return (close(_sct), std::cerr << "Error listening socket" << std::endl, 1);

	_epollfd = epoll_create1(0);

	if (_epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	_ev.events = EPOLLIN;
	_ev.data.fd = _sct;

	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sct, &_ev) == -1)
		return (close(_sct), close(_epollfd), std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	
	std::cout << "0 : epoll fd: " << _epollfd << std::endl;
	
	return (0);
}

int	Server::run(void)
{
	// TO DO: always protect close functions
	Command	currCmd;
	int accepted = 0;
	int yes = 1;//	For SO_KEEPALIVE
	int i;
	socklen_t server_length = sizeof(_server);
	std::vector<std::vector<unsigned char> > command;
	std::vector<unsigned char> v;

	// _users_list.insert(_users_list.begin(), User());
	// _users_list.begin()->setfd(5);
	// getUser(5)->setfd(8);
	// _users_list.begin()->setfd(6);
	// std::cout << "fd user from list:" << getUser(5).getfd() << std::endl;
	// printUsersList();

	while (true)
	{	
		//CHECK CTRL + C
		int	wait_ret = epoll_wait(_epollfd, _events, 1, -1);
		if (wait_ret == -1)
			return (std::cerr << "Error on epoll wait" << std::endl, 1);
		for (int i = 0; i < wait_ret; ++i)
		{
			//CHECK CTRL + C
			if (_events[i].data.fd == _sct)
			{
				std::cout << "1 : socket accept and event data fd:" << _events[i].data.fd << std::endl;
				//CHECK CTRL + C
				accepted = accept(_sct, (sockaddr *)(&_server), &server_length);
				if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)//Keepalive permet de garder la connexion apres utilisation
					return (std::cerr << "Error on accept" << std::endl, 1);
				fcntl(accepted, F_SETFL, O_NONBLOCK);
				std::cout << "1.1 : accepted fd:" << accepted << std::endl;
				_ev.events = EPOLLIN | EPOLLET;
				_ev.data.fd = accepted;
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, accepted, &_ev) == - 1)
					return (std::cerr << "Error on epoll_ctl_add accepted sock" << std::endl, 1);
				

			}
			else
			{
				// TODO
				// CHECK CTRL + C
				// PARSING
				// RESPOND via send(_events[i].data.fd, str, strlen(str), 0); // Add flags? MSG_DONTWAIT

				// for (int i = 0; i < (int)command.size(); i++)
				// 	command[i].clear();
				command.clear();
				v.clear();
				
				// Hang up
				if (_events[i].events & EPOLLHUP)
				{
					close(_events[i].data.fd);
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[i].data.fd, &_events[i]);
				}

				unsigned char buf[1] = "";
				i = 0;
				while (recv(_events[i].data.fd, buf, 1, 0) > 0)// add flags? MSG_DONTWAIT
				{
					v.push_back(*buf);
					std::cout << *buf;
				}
				command.push_back(v);
				std::cout << "v:\n____\n";
				for (int i = 0; i < (int)command.size(); i++) 
				{
					for (int j = 0; j < (int)command[i].size(); j++)
						std::cout << command[i][j];
				}


					
				//Here Parsing
				
				// std::cout << std::endl << "command size: " << command.size() << " and command:\n" << command << std::endl;
				// str = command.c_str();
				// std::cout << std::endl << "str:\n" << str << std::endl;
				
				// currCmd.parseCommand(command);
				
			}
		}
	}

		// if (close(g_serv.getEpollfd()) == -1) {
	// 	std::cerr << "Failed to close epoll file descriptor\n";
	// }
	
	return (1);

}


void Server::printUsersList(void)
{
	std::list<User>::iterator it;
	for (it = _users_list.begin(); it != _users_list.end(); ++it)
	{
    	std::cout << "User name:" << it->getUserName() << " fd:" << it->getfd() << std::endl;
	}

}


// GETTERS

int Server::getSct(void) {
	return (_sct);
}

int Server::getEpollfd(void) {
	return (_epollfd);
}

User* Server::getUser(int fd) {
	std::list<User>::iterator it;
	for (it = _users_list.begin(); it != _users_list.end(); ++it) {
		if (it->getfd() == fd)
			return (&(*it));
	}
	// Exception si User pas trouve?
	return (&(*it));
}