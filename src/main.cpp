/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 14:41:35 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <vector>

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/utils.hpp"

// Server	g_serv;

int is_kill = 0;

void	signal_handling(int sig)
{
	is_kill = sig;
}

void	check_kill(Server& server)
{
	if (is_kill == 130)// CTRL + C
	{
		free_fun(server);
		exit(0);
	}
}

int main(int argc, char **argv)
{
	Server g_serv;
	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);

	std::signal(SIGINT, signal_handling);

	if(g_serv.init(argv) != 0)
		return (1);
	g_serv.run();
	return (1);
}
