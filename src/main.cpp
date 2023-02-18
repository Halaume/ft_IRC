/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 17:32:56 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <vector>

#include <ctime> 

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/utils.hpp"


 
// Server	g_serv;

int is_kill = 0;

void	signal_handling(int sig)
{
	// if (close(g_serv.getSct()) == -1) {
	// 	std::cerr << "Failed to close socket\n";
	// }
	// if (close(g_serv.getEpollfd()) == -1) {
	// 	std::cerr << "Failed to close epoll file descriptor\n";
	// 	// return 1
	// }
	is_kill = sig;
}

int	check_kill(Server server)
{
	if (is_kill == 130)// CTRL + C
	{
		(void)server;
		//Good free, exit pgm
		//free_fun(server);
		exit(0);
	}
	return (0);
}

int main(int argc, char **argv)
{
	// g_time = std::time(0);
	Server g_serv;
	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);

	std::signal(SIGINT, signal_handling);

	// std::cout << " enum 1:" << RPL_WELCOME << std::endl;
	if(g_serv.init(argv) != 0)
		return (1);
	// if (close(g_serv.getEpollfd()) == -1) {
	// 	std::cerr << "Failed to close epoll file descriptor\n";
	// }
	return (g_serv.run());
	// return 0;
}
