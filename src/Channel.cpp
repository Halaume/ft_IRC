/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 18:00:37 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/utils.hpp"

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

// void Channel::addUser(User newUser, Server &my_server)
// {
// 	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
// 	bool	connected = false;
// 	std::vector<unsigned char>	sender;
// 	std::string sender_str;
// 	std::list<User>::iterator	it = this->_userLst.begin();

// 	while (it != this->_userLst.end() && *it != newUser)
// 		it++;
// 	if (*it == newUser)
// 		connected = true;
// 	if (!connected && newUser.getNbChan() == 10)
// 	{
// 		insert_all(sender, "ERR_TOMANYCHANNELS\r\n");
// 		// sender_str = v_to_str(sender);
// 		my_server.send_to_client(newUser.getfd(), sender_str);
// 		return ;
// 	}
// 	if (!connected)
// 		this->_userLst.insert(this->_userLst.end(), newUser);
// 	sender.clear();
// 	insert_all(sender, "RPL_TOPIC\r\n");
// 	// sender_str = v_to_str(sender);
// 	my_server.send(newUser.getfd(), sender_str);
// 	it = this->_userLst.begin();
// 	sender.clear();
// 	insert_all(sender, "RPL_NAMREPLY\r\n");
// 	// sender_str = v_to_str(sender);
// 	while (it != this->_userLst.end())
// 		my_server.send(newUser.getfd(), sender_str);
// }

// void Channel::addUser(User newUser, Server &my_server, std::vector<unsigned char> passwd)
// {
// 	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
// 	bool	connected = false;
// 	std::list<User>::iterator	it = this->_userLst.begin();
// 	std::vector<unsigned char>	sender;
// 	std::string sender_str;

// 	while (it != this->_userLst.end() && *it != newUser)
// 		it++;
// 	if (*it == newUser)
// 		connected = true;
// 	if (!connected && newUser.getNbChan() == 10)
// 	{
// 		// sender_str = v_to_str(sender);
// 		insert_all(sender, "ERR_TOOMANYCHANNELS\r\n");
// 		my_server.send(newUser.getfd(), sender_str);
// 		return ;
// 	}
// 	if (!connected)
// 	{
// 		if (this->_chanPassword.size() != 0 && passwd == this->_chanPassword)
// 			this->_userLst.insert(this->_userLst.end(), newUser);
// 		else
// 		{
// 			sender.clear();
// 			insert_all(sender, "ERR_BADCHANNELKEY\r\n");
// 			// sender_str = v_to_str(sender);
// 			my_server.send(newUser.getfd(), sender_str);
// 			return ;
// 		}
// 	}
// 	sender.clear();
// 	insert_all(sender, "RPL_TOPIC\r\n");
// 	// sender_str = v_to_str(sender);
// 	my_server.send(newUser.getfd(), sender_str);
// 	it = this->_userLst.begin();
// 	sender.clear();
// 	insert_all(sender, "RPL_NAMREPLY\r\n");
// 	// sender_str = v_to_str(sender);
// 	while (it != this->_userLst.end())
// 		my_server.send(it->getfd(), sender_str);
// }


// Getters
std::list<User>& Channel::getUsers(void)
{
	return (_userLst);
}
