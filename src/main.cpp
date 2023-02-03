/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 15:31:17 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <csignal>

#include "../inc/Server.hpp"

int is_kill = 0;

void	signal_handling(int sig)
{
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

int	enable_keepalive(int sock)
{
		return (-1);
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);

	std::signal(SIGINT, signal_handling);

	Server	my_serv;

	if(my_serv.init(argv) != 0)
		return (1);
	return (my_serv.run());
}
