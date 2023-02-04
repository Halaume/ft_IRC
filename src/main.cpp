/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:52:09 by ghanquer          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/03 19:33:54 by iguscett         ###   ########.fr       */
=======
/*   Updated: 2023/02/03 17:25:48 by ghanquer         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <csignal>

#include "../inc/Server.hpp"

#include <vector>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"


std::vector<std::vector<std::string> > getCommandBlock(const std::string input)
{
	std::string commandline;
	std::string token;
	std::string striterator;
	std::vector<std::string> v1;
	std::vector<std::vector<std::string> > v0;
	
	striterator = input;
	while (striterator != "")
	{
		commandline = striterator.substr(0, striterator.find("\r\n"));
		striterator = striterator.substr(striterator.find("\r\n")+2, striterator.length());
		v1.clear();
		while (commandline != "")
		{
			token = commandline.substr(0, commandline.find(" "));
			v1.push_back(token);
			if (commandline.substr(commandline.find(" ") +1, commandline.length()).length() == commandline.length())
				commandline = "";
			else
				commandline = commandline.substr(commandline.find(" ") +1, commandline.length());	
		}
		v0.push_back(v1);
	}
	
	// Print command blocks
	for (int i = 0; i < (int)v0.size(); i++) {
		for (int j = 0; j < (int)v0[i].size(); j++) 
			std::cout << "|" << v0[i][j] << "|";
		std::cout << std::endl;
	}
	return (v0);
}



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

int main(int argc, char **argv)
{
	(void)argv;

	if (argc != 3)
		return (std::cerr << "Wrong number of arg" << std::endl, 1);
<<<<<<< HEAD
	std::string input;
	input = "PASS pwd\r\nPASSAGE\r\nNICK iguscett\r\n";
	getCommandBlock(input);
	return (0);
=======

	std::signal(SIGINT, signal_handling);

	Server	my_serv;

	if(my_serv.init(argv) != 0)
		return (1);
	return (my_serv.run());
>>>>>>> main
}
