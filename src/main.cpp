/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
/*   Updated: 2023/01/31 17:38:40 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int is_kill = 0;

void	signal_handling(int sig)
{
	is_kill = sig;
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);

	std::signal(SIGINT, signal_handling);
	int sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1);

	sockaddr_in server;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	if (server.sin_port == 0)
		return (std::cerr << "Error port = 0" << std::endl, 1);

	if (bind(sct, (sockaddr *)(&server), sizeof(server)))//server_info->ai_addrlen))
		return (std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(sct, 1) == -1)
		return (std::cerr << "Error listening socket" << std::endl, 1);

	epoll_event events[10];
	epoll_event ev;
	int	epollfd = epoll_create1(0);

	if (epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	ev.events = EPOLLIN;
	ev.data.fd = sct;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sct, &ev) == -1)
		return (std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);

	int accepted = 0;
	socklen_t server_length = sizeof(server);

	while (true)
	{
		if (is_kill != 0)
		{
			close(sct);
			if (accepted != 0)
				close(accepted);
			exit(0);
		}
		std::cout << "here wait";
		int	wait_ret = epoll_wait(epollfd, events, 1, -1);
		std::cout << "wait ret = " << wait_ret << std::endl;
		if (wait_ret == -1)
		{
			if (is_kill != 0)
			{
				close(sct);
				if (accepted != 0)
					close(accepted);
				return (0);
			}

			return (std::cerr << "Error on epoll wait" << std::endl, 1);
		}
		for (int i = 0; i < wait_ret; ++i)
		{
			if (is_kill != 0)
			{
				close(sct);
				if (accepted != 0)
					close(accepted);
				return (0);
			}
			if (events[i].data.fd == sct)
			{
				accepted = accept(sct, (sockaddr *)(&server), &server_length);
				if (accepted == -1)
					return (std::cerr << "Error on accept" << std::endl, 1);
				fcntl(accepted, F_SETFL, O_NONBLOCK);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = accepted;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, accepted, &ev) == - 1)
					return (std::cerr << "Error on epoll_ctl_add accepted sock" << std::endl, 1);
			}
			else
			{
				if (is_kill != 0)
					return (close(sct), 0);
				char buf[4096] = "";
				std::cout << "PASSAGE" << std::endl;
				while (read(events[i].data.fd, buf, 1) > 0)
				{
					write(events[i].data.fd, buf, strlen(buf));
					if (is_kill != 0)
						return (close(sct), 0);
					std::cout << buf;
				}
				std::cout << "PASSE";
				std::cout << std::endl;
			}
		}
	}

	close(accepted);
	close(sct);

	return (0);
}
