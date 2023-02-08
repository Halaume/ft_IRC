/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/08 16:52:23 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"

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
#include <vector>
#include "../inc/utils.hpp"

Server::Server(void): _server(), _sct(), _passwd(),_epollfd(), _ev(), _channels(), _Users()
{
	_passwd = NULL;
}

Server::Server(const Server & copy): _server(copy._server), _sct(copy._sct), _passwd(copy._passwd), _epollfd(copy._epollfd), _ev(copy._ev),_channels(copy._channels), _Users(copy._Users)
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

std::list<User>	Server::getUser(void) const
{
	return (this->_Users);
}

Channel &	Server::findChan(std::vector<unsigned char> channel)
{
	std::vector<Channel>::iterator	it = this->_channels.begin();
	while (it != this->_channels.end() && it->getChanName() != channel)
		it++;
	if (it == this->_channels.end())
		it = this->_channels.insert(this->_channels.end(), Channel(channel));
	return (*it);
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
	int k;
	std::vector<std::vector<unsigned char> >::size_type i, j;
	socklen_t server_length = sizeof(_server);
	std::vector<std::vector<unsigned char> > command;
	std::vector<std::vector<unsigned char> > scommand;
	std::vector<unsigned char> v;

	while (true)
	{	
		//CHECK CTRL + C
		int	wait_ret = epoll_wait(_epollfd, _events, 1, -1);
		if (wait_ret == -1)
			return (std::cerr << "Error on epoll wait" << std::endl, 1);
		for (k = 0; k < wait_ret; ++k)
		{
			if (_events[k].data.fd == _sct)
			{
				std::cout << "1 : socket accept and event data fd:" << _events[k].data.fd << std::endl;
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
				command.clear();
				v.clear();
				// Hang up
				if (_events[k].events & EPOLLHUP)
				{
					close(_events[k].data.fd);
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_events[k]);
				}
				// Read fdand get command
				unsigned char buf[BUFFER_SIZE] = "";
				while (recv(_events[k].data.fd, buf, BUFFER_SIZE, 0) > 0)// add flags? MSG_DONTWAIT
				{
					for (i = 0; i < BUFFER_SIZE; i++)
					{
						v.push_back(buf[i]);
						if (i > 0 && buf[i - 1] == '\r' && buf[i] == '\n')
						{
							command.push_back(v);
							v.clear();
						}
					}
				}
				// Print command
				std::cout << "Print command____\n";
				for (i = 0; i < command.size(); i++) 
				{
					std::cout << ">";
					for (j = 0; j < command[i].size(); j++)
						std::cout << command[i][j];
				}
				// Get scommand FUCKED UP
				for (i = 0; i < command.size(); i++)
				{
					v.clear();
					scommand.clear();
					for (j = 0; j < command[i].size(); j++)
					{			
						if (command[i][j] == ' ' || j == command[i].size() - 2)
						{
							scommand.push_back(v);
							v.clear();
						}
						else if (command[i][j] != ' ')
							v.push_back(command[i][j]);
					}
					
					// Print scommand
					std::cout << "Print scommand____\n";
					for (i = 0; i < scommand.size(); i++) 
					{
						std::cout << ">";
						for (j = 0; j < scommand[i].size(); j++)
							std::cout << scommand[i][j];
						std::cout << "\n";
					}	
				}

				// Parsing
				
				// std::cout << std::endl << "command size: " << command.size() << " and command:\n" << command << std::endl;
				// str = command.c_str();
				// std::cout << std::endl << "str:\n" << str << std::endl;
				
				// currCmd.parseCommand(command);
				
			}
		}
	}

	return (1);
}

void	Server::send(int fd, std::vector<unsigned char> buf)
{
	long int ret;
	//send(fd, buf, buf.size(), MSG_DONTWAIT);
	for (std::vector<unsigned char>::size_type i = 0; i < buf.size(); i++)
		ret = write(fd, &buf[i], 1);
	(void)ret;
}

std::list<User>::iterator	Server::findUser(std::vector<unsigned char> nick)
{
	std::list<User>::iterator it;
	for (it = this->_Users.begin(); it != this->_Users.end(); it++)
	{
		if (it->getUserName() == nick)
			return (it);
	}
	return (it);
}


// void Server::printUsersList(void)
// {
// 	std::list<User>::iterator it;
// 	for (it = _users_list.begin(); it != _users_list.end(); ++it)
// 	{
//     	std::cout << "User name:" << it->getUserName() << " fd:" << it->getfd() << std::endl;
// 	}

// }


// GETTERS

int Server::getSct(void)
{
	return (_sct);
}

int Server::getEpollfd(void)
{
	return (_epollfd);
}

std::list<User> Server::getUsers(void)
{
	return (this->_Users);
}

// User* Server::getUser(int fd)
// {
// 	std::list<User>::iterator it;
// 	for (it = _users_list.begin(); it != _users_list.end(); ++it) {
// 		if (it->getfd() == fd)
// 			return (&(*it));
// 	}
// 	// Exception si User pas trouve?
// 	return (&(*it));
// }


