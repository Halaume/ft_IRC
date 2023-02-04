/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 19:28:39 by iguscett         ###   ########.fr       */
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


Server::Server(void): _server(), _sct(), _epollfd(), _ev(), _channels(), _users_list
()
{
	this->_passwd = NULL;
}

Server::Server(const Server & copy): _server(copy._server), _sct(copy._sct),_epollfd(copy._epollfd), _ev(copy._ev),_channels(copy._channels), _users_list
(copy._users_list
)
{
	this->_passwd = copy._passwd;
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
	this->_sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1);

	this->_passwd = argv[2];

	this->_server.sin_addr.s_addr = INADDR_ANY;
	this->_server.sin_family = AF_INET;
	this->_server.sin_port = htons(atoi(argv[1]));
	if (this->_server.sin_port == 0)
		return (std::cerr << "Error on port" << std::endl, 1);

	if (bind(this->_sct, (sockaddr *)(&this->_server), sizeof(this->_server)))//server_info->ai_addrlen))
		return (close(this->_sct), std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(this->_sct, 1) == -1)
		return (close(this->_sct), std::cerr << "Error listening socket" << std::endl, 1);

	this->_epollfd = epoll_create1(0);

	if (this->_epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_sct;

	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_sct, &this->_ev) == -1)
		return (std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);

	// Create user and set fd
	return (0);
}

int	Server::run(void)
{
	Command	currCmd(void);
	int accepted = 0;
	int yes = 1;//	For SO_KEEPALIVE
	socklen_t server_length = sizeof(this->_server);

	// _users_list.insert(_users_list.begin(), User());
	// _users_list.begin()->setfd(5);
	// this->getUser(5)->setfd(8);
	// _users_list.begin()->setfd(6);
	// std::cout << "fd user from list:" << this->getUser(5).getfd() << std::endl;
	// printUsersList();

	while (true)
	{
		//CHECK CTRL + C
		int	wait_ret = epoll_wait(this->_epollfd, this->_events, 1, -1);
		if (wait_ret == -1)
			return (std::cerr << "Error on epoll wait" << std::endl, 1);
		for (int i = 0; i < wait_ret; ++i)
		{
			//CHECK CTRL + C
			if (this->_events[i].data.fd == this->_sct)
			{
				//CHECK CTRL + C
				accepted = accept(this->_sct, (sockaddr *)(&this->_server), &server_length);
				if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)//Keepalive permet de garder la connexion apres utilisation
					return (std::cerr << "Error on accept" << std::endl, 1);
				fcntl(accepted, F_SETFL, O_NONBLOCK);
				this->_ev.events = EPOLLIN | EPOLLET;
				this->_ev.data.fd = accepted;
				if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, accepted, &this->_ev) == - 1)
					return (std::cerr << "Error on epoll_ctl_add accepted sock" << std::endl, 1);
				std::cout << "fd socket: " << _ev.data.fd << std::endl;
				
				//ADD accepted to User list
				//*****************************************
			}
			else
			{
				if (this->_events[i].events & EPOLLHUP)
				{
					close(this->_events[i].data.fd);
					epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, this->_events[i].data.fd, &this->_events[i]);
				}

				/* TODO
				 * CHECK CTRL + C
				 * PARSING
				 * RESPOND
				 */
				 
				char buf[1] = "";
				
				std::cout << "fd: " << _events[i].data.fd << std::endl;
				
				while (read(this->_events[i].data.fd, buf, 1) > 0)
					//while (recv(events[i].data.fd, buf, strlen(buf), MSG_DONTWAIT) > 0)
				{
					//Here Parsing (Pour l'instant je recupere char par char donc faudras voir)
					//find right user with fd
					//rightUser._currCmd.push_back(buf);
					write(this->_events[i].data.fd, buf, strlen(buf));
					// write(this->_events[i].data.fd, "OK connected to server", strlen("OK connected to server"));
					// write(this->_events[i].data.fd, "PING :test\r\n", strlen("PING :test\r\n"));
					//if (fin _currcmd == \r\n)
					//{
					//std::string	answer = answer(parsed); Where "Parsed = DATA from Iac"
					//write(this->_events[i].data.fd, answer, strlen(answer));
					//send(events[i].data.fd, buf, strlen(buf), MSG_DONTWAIT);
					//}
					//CHECK CTRL + C
					std::cout << buf;
				}
				// write(this->_events[i].data.fd, "OK connected to server", strlen("OK connected to server"));
			}
		}
	}
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
User* Server::getUser(int fd) {
	std::list<User>::iterator it;
	for (it = _users_list.begin(); it != _users_list.end(); ++it) {
		if (it->getfd() == fd)
			return (&(*it));
	}
	// Exception si User pas trouve?
	return (&(*it));
}