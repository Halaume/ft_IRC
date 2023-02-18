/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 22:59:45 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/utils.hpp"

Channel::Channel(void): _chan_name(), _chan_password(), _user_connected()
{
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('p', false));
	_modes.insert(std::make_pair('s', false));
	_modes.insert(std::make_pair('i', false));
	_modes.insert(std::make_pair('t', false));
	_modes.insert(std::make_pair('n', false));
	_modes.insert(std::make_pair('m', false));
	_modes.insert(std::make_pair('l', false));
	_modes.insert(std::make_pair('b', false));
	_modes.insert(std::make_pair('v', false));
	_modes.insert(std::make_pair('k', false));
}

Channel::Channel(const Channel & copy): _chan_name(copy._chan_name), _chan_password(copy._chan_password), \
_modes(copy._modes), _user_connected(copy._user_connected)
{
}

Channel::Channel(std::vector<unsigned char> name): _chan_name(name), _chan_password(), _user_connected()
{
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('p', false));
	_modes.insert(std::make_pair('s', false));
	_modes.insert(std::make_pair('i', false));
	_modes.insert(std::make_pair('t', false));
	_modes.insert(std::make_pair('n', false));
	_modes.insert(std::make_pair('m', false));
	_modes.insert(std::make_pair('l', false));
	_modes.insert(std::make_pair('b', false));
	_modes.insert(std::make_pair('v', false));
	_modes.insert(std::make_pair('k', false));
}

Channel::~Channel(void)
{
	_chan_name.erase(_chan_name.begin(), _chan_name.end());
	_chan_password.erase(_chan_password.begin(), _chan_password.end());
	_modes.erase(_modes.begin(), _modes.end());
	_op_list.erase(_op_list.begin(), _op_list.end());
	_user_list.erase(_user_list.begin(), _user_list.end());
	_ban_list.erase(_ban_list.begin(), _ban_list.end());
}

Channel &	Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

void Channel::addUser(User *newUser, Server &my_server)
{
	(void)my_server;
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::vector<unsigned char>	sendtoer;
	std::list<User *>::iterator	it = _user_list.begin();

	while (it != _user_list.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser->getNbChan() == 10)
	{
		insert_all(sendtoer, "ERR_TOMANYCHANNELS\r\n");
		// my_server.sendto(newUser->getfd(), sendtoer);
		return ;
	}
	if (!connected)
		_user_list.insert(_user_list.end(), newUser);
	sendtoer.clear();
	insert_all(sendtoer, "RPL_TOPIC\r\n");
	// my_server.sendto(newUser->getfd(), sendtoer);
	it = _user_list.begin();
	sendtoer.clear();
	insert_all(sendtoer, "RPL_NAMREPLY\r\n");
	// while (it != _user_list.end())
	// 	my_server.sendto(newUser->getfd(), sendtoer);
}

void Channel::addUser(User *newUser, Server &my_server, std::vector<unsigned char> passwd)
{
	(void)my_server;
	//Check if ban (idk if it is with nick/realname/username or with the fd), i'll do it after handling NICK and propably KICK
	bool	connected = false;
	std::list<User *>::iterator	it = _user_list.begin();
	std::vector<unsigned char>	sender;

	while (it != _user_list.end() && *it != newUser)
		it++;
	if (*it == newUser)
		connected = true;
	if (!connected && newUser->getNbChan() == 10)
	{
		insert_all(sender, "ERR_TOOMANYCHANNELS\r\n");
		// my_server.sendto(newUser->getfd(), sender);
		return ;
	}
	if (!connected)
	{
		if (_chan_password.size() != 0 && passwd == _chan_password)
			_user_list.insert(_user_list.end(), newUser);
		else
		{
			sender.clear();
			insert_all(sender, "ERR_BADCHANNELKEY\r\n");
			// my_server.sendto(newUser->getfd(), sender);
			return ;
		}
	}
	sender.clear();
	insert_all(sender, "RPL_TOPIC\r\n");
	// my_server.sendto(newUser->getfd(), sender);
	it = _user_list.begin();
	sender.clear();
	insert_all(sender, "RPL_NAMREPLY\r\n");
	// while (it != _user_list.end())
	// 	my_server.sendto((*it)->getfd(), sender);
}

void	Channel::delUser(int fd)
{
	for (std::list<User *>::iterator it = _user_list.begin(); it != _user_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			_user_list.erase(it);
	}
	for (std::list<User *>::iterator it = _op_list.begin(); it != _op_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			_op_list.erase(it);
	}
}

std::vector<unsigned char>	Channel::getChanName(void) const
{
	return (_chan_name);
}

std::vector<unsigned char>	Channel::getChanPassword(void) const
{
	return (_chan_password);
}

std::list<User *>::iterator	Channel::getOpListbg(void)
{
	return (_op_list.begin());
}

std::list<User *>::iterator	Channel::getOpListend(void)
{
	return (_op_list.end());
}

std::list<User *>::iterator	Channel::getUsrListbg(void)
{
	return (_user_list.begin());
}

std::list<User *>::iterator	Channel::getUsrListend(void)
{
	return (_user_list.end());
}

std::list<User *>	Channel::getUsrList(void)
{
	return (_user_list);
}
std::list<User *> Channel::getUsers(void) const
{
	return (_user_list);
}

bool	Channel::isOp(User *usr) const
{
	for (std::list<User *>::const_iterator it = _op_list.begin(); it != _op_list.end(); it++)
		if (*it == usr)
			return (true);
	return (false);
}

// SETTERS
void Channel::setChanName(std::vector<unsigned char>& chan_name)
{
	_chan_name = chan_name;
}

void Channel::setChanName(std::string& chan_name)
{
	_chan_name.clear();
	for (std::string::size_type i = 0; i < chan_name.size(); i++)
		_chan_name.push_back(chan_name[i]);
}

void Channel::setChanPassword(std::vector<unsigned char>& chan_password)
{
	_chan_password = chan_password;
}

void Channel::setChanPassword(std::string& chan_password)
{
	_chan_password.clear();
	for (std::string::size_type i = 0; i < chan_password.size(); i++)
		_chan_password.push_back(chan_password[i]);
}