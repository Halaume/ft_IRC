/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 17:24:42 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include "../inc/Server.hpp"

std::string commandsList[] = {	"KICK",
								"MODE",
								"INVITE",
								"TOPIC",
								"PASS"
								};

// Server::Server(void): _channels()
Server::Server(void)
{
	for (unsigned int i = 0; i < sizeof(commandsList) / sizeof(commandsList[0]); i++)
		_commands.push_back(commandsList[i]);
}


// Server::Server(const Server & copy): _channels(copy._channels)
// Server::Server(const Server & copy): _channels(copy._channels)
// {
// }

Server::~Server(void)
{
}

Server &	Server::operator=(const Server & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

// _____GETTERS_____
std::vector<std::string> Server::getCommands(void)
{
	return (_commands);
}