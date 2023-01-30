/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
/*   Updated: 2023/01/30 17:13:32 by ghanquer         ###   ########.fr       */
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


int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);

	int sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sct == EACCES)
		return (std::cerr << "Invalid socket" << std::endl, 1);

	hostent * localHost = gethostbyname("");
	char * localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

	sockaddr_in server;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	if (bind(sct, (sockaddr *)(&server), sizeof(server)))
		return (std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(sct, 1) == -1)
		return (std::cerr << "Error listening socket" << std::endl, 1);
//	TODO POLL/EPOLL !
//
	epoll_event events[1];
	epoll_event ev;
	int	epollfd = epoll_create1(0);
	if (epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	ev.events = EPOLLIN;
	ev.data.fd = sct;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sct, &ev) == -1)
		return (std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	int	wait_ret = epoll_wait(epollfd, events, 1, -1);
	if (wait_ret == -1)
		return (std::cerr << "Error on epoll wait" << std::endl, 1);
	unsigned int server_size = sizeof(server);
	for (int i = 0; i < wait_ret; i++)
	{
		if (events[i].data.fd == sct)
		{
			int accepted = accept(sct, (sockaddr *)(&server), &server_size);
			if (accepted == -1)
				return (std::cerr << "Error on accept" << std::endl, 1);
		}
	}


	close(sct);

	return (0);
}
