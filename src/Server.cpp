/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 17:23:37 by ghanquer         ###   ########.fr       */
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


Server::Server(void): _server(), _sct(), _epollfd(), _ev(), _channels(), _Users()
{
	this->_passwd = NULL;
}

Server::Server(const Server & copy): _server(copy._server), _sct(copy._sct),_epollfd(copy._epollfd), _ev(copy._ev),_channels(copy._channels), _Users(copy._Users)
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

	int	epollfd = epoll_create1(0);

	if (epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_sct;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, this->_sct, &this->_ev) == -1)
		return (std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	return (0);
}

int	Server::run(void)
{
	int accepted = 0;
	int yes = 1;//	For SO_KEEPALIVE
	socklen_t server_length = sizeof(this->_server);

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
				char buf[4096] = "";
				while (read(this->_events[i].data.fd, buf, 1) > 0)
					//while (recv(events[i].data.fd, buf, strlen(buf), MSG_DONTWAIT) > 0)
				{
					//Here Parsing (Pour l'instant je recupere char par char donc faudras voir)
					write(this->_events[i].data.fd, buf, strlen(buf));
					write(this->_events[i].data.fd, "PING :test\r\n", strlen("PING :test\r\n"));
					//send(events[i].data.fd, buf, strlen(buf), MSG_DONTWAIT);
					//CHECK CTRL + C
					std::cout << buf;
				}
			}
		}
	}
	return (1);

}
