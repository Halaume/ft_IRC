/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 18:27:03 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../inc/User.hpp"

User::User(void): _fd(), _userName(), _realName(), _client(), _channels()
{
}

User::User(const User & copy): _fd(copy._fd), _userName(copy._userName), _realName(copy._realName), _client(copy._client), _channels(copy._channels)
{
}

User::~User(void)
{
}

User &	User::operator=(const User & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}


// GETTERS
int User::getfd(void) const
{
	return (_fd);
}

std::string User::getCurrCmd(void) const
{
	return (_currCmd);
}

std::string User::getUserName(void) const
{
	return (_userName);
}

std::string User::getRealName(void) const
{
	return (_realName);
}

std::string User::getClient(void) const
{
	return (_client);
}


// SETTERS
// void User::setfd(int fd)
// {
// }

// void User::setCurrCmd(std::string currCmd)
// {
// }

void User::setUserName(std::string name)
{
	_userName = name;
}

// void User::setRealName(std::string name)
// {
// }

// void User::setClient(std::string client)
// {
// }