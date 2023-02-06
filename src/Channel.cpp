/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 16:46:35 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <list>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"

Channel::Channel(void): _chanName(), _chanPassword(), _userConnected(), _opList()
{
	this->_modes.insert(std::make_pair('o', false));
	this->_modes.insert(std::make_pair('p', false));
	this->_modes.insert(std::make_pair('s', false));
	this->_modes.insert(std::make_pair('i', false));
	this->_modes.insert(std::make_pair('t', false));
	this->_modes.insert(std::make_pair('n', false));
	this->_modes.insert(std::make_pair('m', false));
	this->_modes.insert(std::make_pair('l', false));
	this->_modes.insert(std::make_pair('b', false));
	this->_modes.insert(std::make_pair('v', false));
	this->_modes.insert(std::make_pair('k', false));
}

Channel::Channel(const Channel & copy): _chanName(copy._chanName), _chanPassword(copy._chanPassword), _modes(copy._modes), _userConnected(copy._userConnected), _opList(copy._opList)
{
}

Channel::Channel(std::string name): _chanName(name), _chanPassword(), _userConnected(), _opList()
{
	this->_modes.insert(std::make_pair('o', false));
	this->_modes.insert(std::make_pair('p', false));
	this->_modes.insert(std::make_pair('s', false));
	this->_modes.insert(std::make_pair('i', false));
	this->_modes.insert(std::make_pair('t', false));
	this->_modes.insert(std::make_pair('n', false));
	this->_modes.insert(std::make_pair('m', false));
	this->_modes.insert(std::make_pair('l', false));
	this->_modes.insert(std::make_pair('b', false));
	this->_modes.insert(std::make_pair('v', false));
	this->_modes.insert(std::make_pair('k', false));
}

Channel::~Channel(void)
{
}

Channel &	Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

std::string	Channel::getChanName(void) const
{
	return (this->_chanName);
}

void Channel::addUser(User newUser, Server &my_server)
{
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::list<User>::iterator	it = this->_userLst.begin();

	while (it != this->_userLst.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser.getNbChan() == 10)
	{
		my_server.send(newUser.getfd(), "ERR_TOOMANYCHANNELS");
		return ;
	}
	if (!connected)
		this->_userLst.insert(this->_userLst.end(), newUser);
	my_server.send(newUser.getfd(), "RPL_TOPIC");
	it = this->_userLst.begin();
	while (it != this->_userLst.end())
		my_server.send(it->getfd(), "RPL_NAMREPLY");
}

void Channel::addUser(User newUser, Server &my_server, std::string passwd)
{
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::list<User>::iterator	it = this->_userLst.begin();

	while (it != this->_userLst.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser.getNbChan() == 10)
	{
		my_server.send(newUser.getfd(), "ERR_TOOMANYCHANNELS");
		return ;
	}
	if (!connected)
	{
		if (this->_chanPassword != "" && passwd == this->_chanPassword)
			this->_userLst.insert(this->_userLst.end(), newUser);
		else
		{
			my_server.send(newUser.getfd(), "ERR_BADCHANNELKEY");
			return ;
		}
	}
	my_server.send(newUser.getfd(), "RPL_TOPIC");
	it = this->_userLst.begin();
	while (it != this->_userLst.end())
		my_server.send(it->getfd(), "RPL_NAMREPLY");
}
