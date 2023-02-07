/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/07 17:01:58 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "utils.cpp"

Channel::Channel(void): _chanName(), _chanPassword(), _userConnected()
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

Channel::Channel(const Channel & copy): _chanName(copy._chanName), _chanPassword(copy._chanPassword), _modes(copy._modes), _userConnected(copy._userConnected)
{
}

Channel::Channel(std::vector<unsigned char> name): _chanName(name), _chanPassword(), _userConnected()
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

std::vector<unsigned char>	Channel::getChanName(void) const
{
	return (this->_chanName);
}

void Channel::addUser(User newUser, Server &my_server)
{
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::vector<unsigned char>	sender;
	std::vector<User>::iterator	it = this->_userLst.begin();

	while (it != this->_userLst.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser.getNbChan() == 10)
	{
		insert_all(sender, "ERR_TOMANYCHANNELS\r\n");
		my_server.send(newUser.getfd(), sender);
		return ;
	}
	if (!connected)
		this->_userLst.insert(this->_userLst.end(), newUser);
	sender.clear();
	insert_all(sender, "RPL_TOPIC\r\n");
	my_server.send(newUser.getfd(), sender);
	it = this->_userLst.begin();
	sender.clear();
	insert_all(sender, "RPL_NAMREPLY\r\n");
	while (it != this->_userLst.end())
		my_server.send(newUser.getfd(), sender);
}

void Channel::addUser(User newUser, Server &my_server, std::vector<unsigned char> passwd)
{
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::vector<User>::iterator	it = this->_userLst.begin();
	std::vector<unsigned char>	sender;

	while (it != this->_userLst.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser.getNbChan() == 10)
	{
		insert_all(sender, "ERR_TOOMANYCHANNELS\r\n");
		my_server.send(newUser.getfd(), sender);
		return ;
	}
	if (!connected)
	{
		if (this->_chanPassword.size() != 0 && passwd == this->_chanPassword)
			this->_userLst.insert(this->_userLst.end(), newUser);
		else
		{
			sender.clear();
			insert_all(sender, "ERR_BADCHANNELKEY\r\n");
			my_server.send(newUser.getfd(), sender);
			return ;
		}
	}
	sender.clear();
	insert_all(sender, "RPL_TOPIC\r\n");
	my_server.send(newUser.getfd(), sender);
	it = this->_userLst.begin();
	sender.clear();
	insert_all(sender, "RPL_NAMREPLY\r\n");
	while (it != this->_userLst.end())
		my_server.send(it->getfd(), sender);
}
