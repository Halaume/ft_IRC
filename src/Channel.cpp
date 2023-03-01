/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/28 16:38:04 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"


Channel::Channel(void): _chan_name(), _chan_password(), _user_connected(), _nb_users_limit()
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
_modes(copy._modes), _user_connected(copy._user_connected), _op_list(copy._op_list), \
_user_list(copy._user_list), _ban_list(copy._ban_list), _nb_users_limit(copy._nb_users_limit), _invite_list(copy._invite_list)
{
}

Channel::Channel(std::vector<unsigned char> name): _chan_name(name), _chan_password(), _user_connected(), \
_nb_users_limit()
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

Channel::Channel(std::vector<unsigned char> name, std::vector<unsigned char> password): _chan_name(name), \
_chan_password(password), _user_connected(), _nb_users_limit()
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
	_invite_list.erase(_invite_list.begin(), _invite_list.end());
}

Channel &	Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

bool	Channel::operator!=(const Channel & lhs) const
{
	return (this->_chan_name != lhs._chan_name);
}

std::vector<unsigned char>	 Channel::getTopic(void) const
{
	return (this->_topic);	
}

std::map<char, bool>		 Channel::getModes(void) const
{
	return (this->_modes);
}

void        Channel::setTopic(std::vector<unsigned char> topic)
{
	this->_topic = topic;
}

bool    Channel::isOp(User usr) const
{
    for (std::list<User *>::const_iterator it = _op_list.begin(); it != _op_list.end(); it++)
    {
		if (**it == usr)
            return (true);
	}
    return (false);
}

void Channel::addUser(User * new_user)
{
	_user_list.push_back(new_user);
	new_user->addChannel(this);
}

void	Channel::delUser(int fd)
{
	for (std::list<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			this->_user_list.erase(it);
	}
	for (std::list<User *>::iterator it = this->_op_list.begin(); it != this->_op_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			this->_op_list.erase(it);
	}
}

// Getters

std::vector<unsigned char>	Channel::getChanName(void) const
{
	return (_chan_name);
}

void Channel::addUserToBan(User *new_user)
{
	// verify if user not already in list..
	// check list limit etc...
	_ban_list.push_back(new_user);
}

void Channel::addUserToInvite(User *new_user)
{
	// verify if user not already in list..
	// check list limit etc...
	_invite_list.push_back(new_user);
}

std::list<User *>::iterator	Channel::getOpListbg(void)
{
	return (this->_op_list.begin());
}

std::list<User *>::iterator	Channel::getOpListend(void)
{
	return (this->_op_list.end());
}

std::list<User *>::iterator	Channel::getUserListbg(void)
{
	return (this->_user_list.begin());
}

std::list<User *>::iterator	Channel::getUserListend(void)
{
	return (this->_user_list.end());
}

std::list<User *>	Channel::getUserList(void)
{
	return (this->_user_list);
}

std::vector<unsigned char>	Channel::getChanPassword(void) const
{
	return (_chan_password);
}

std::list<User *>::iterator	Channel::getUserListBanbg(void)
{
	return (_ban_list.begin());
}

std::list<User *>::iterator	Channel::getUserListBanend(void)
{
	return (_ban_list.end());
}

std::list<User *>::iterator	Channel::getUserListInvitebg(void)
{
	return (_invite_list.begin());
}

std::list<User *>::iterator	Channel::getUserListInviteend(void)
{
	return (_invite_list.end());
}

std::list<User *> Channel::getUsers(void) const
{
	return (_user_list);
}

int Channel::getNbUsers(void)
{
	return (static_cast<int>(_user_list.size()));
}

int Channel::getNbUsersLimit(void)
{
	return (_nb_users_limit);
}

bool	Channel::isOp(User *usr) const
{
	for (std::list<User *>::const_iterator it = this->_op_list.begin(); it != this->_op_list.end(); it++)
		if (*it == usr)
			return (true);
	return (false);
}

bool Channel::getMode(char c)
{
	return (_modes[c]);
}

std::map<char, bool>::iterator Channel::getModesbg(void)
{
	return (_modes.begin());
}

std::map<char, bool>::iterator Channel::getModesend(void)
{
	return (_modes.end());
}

// SETTERS
void Channel::setChanName(std::vector<unsigned char> chan_name)
{
	_chan_name = chan_name;
}

void Channel::setChanPassword(std::vector<unsigned char>& chan_password)
{
	_chan_password = chan_password;
}

void Channel::setNbUsersLimit(int nb_users_limit)
{
	_nb_users_limit = nb_users_limit;
}

void Channel::setMode(char c, bool mode)
{
	std::map<char, bool>::iterator it;

	for (it = _modes.begin(); it != _modes.end(); ++it)
	{
		if (it->first == c)
			it->second = mode;
	}
}

bool Channel::isUserInChannel(User *user)
{
	std::list<User *>::iterator it;

	for (it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

bool Channel::isUserBanned(User *user)
{
	std::list<User *>::iterator it;

	for (it = _ban_list.begin(); it != _ban_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

bool Channel::isUserInvited(User *user)
{
	std::list<User *>::iterator it;

	for (it = _invite_list.begin(); it != _invite_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

std::list<User *>::iterator	Channel::findUser(std::vector<unsigned char> nick)
{
	std::list<User *>::iterator it;
	
	for (it = this->_user_list.begin(); it != this->_user_list.end(); it++)
	{
		if ((*it)->getUserName() == nick)
			return (it);
	}
	return (it);
}
