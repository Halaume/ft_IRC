/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/01/31 18:30:26 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include "../inc/Server.hpp"

Server::Server(void): _channels()
{
}

Server::Server(const Server & copy): _channels(copy._channels)
{
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
