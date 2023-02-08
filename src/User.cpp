/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/08 14:54:02 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../inc/User.hpp"

User::User(void): _fd(), _registered(false), _passwd(), _userName(), _realName(), _client(), _channels()
{
}

User::User(const User & copy): _fd(copy._fd), _registered(copy._registered), _passwd(copy._passwd), _userName(copy._userName), _realName(copy._realName), _client(copy._client), _channels(copy._channels)
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

bool	User::operator==(User test) const
{
	return (this->_fd == test._fd);
}

bool	User::operator!=(User test) const
{
	return (!(*this == test));
}

int		User::getfd(void) const
{
	return (this->_fd);
}

bool	User::getRegistered(void) const
{
	return (this->_registered);
}

std::vector<unsigned char>	User::getUserName(void) const
{
	return (this->_userName);
}

void	User::setRegistered(bool registered)
{
	this->_registered = registered;
}

void	User::setPasswd(std::vector<unsigned char> passwd)
{
	this->_passwd = passwd;
}

void	User::setUserName(std::vector<unsigned char> username)
{
	this->_userName = username;
}

void	User::setRealName(std::vector<unsigned char> realname)
{
	this->_realName = realname;
}

int	User::getNbChan(void)
{
	return (static_cast<int>(this->_channels.size()));
}
