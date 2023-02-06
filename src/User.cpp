/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 12:15:31 by ghanquer         ###   ########.fr       */
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

bool	User::getRegistered(void) const
{
	return (this->_registered);
}

void	User::setRegistered(bool registered)
{
	this->_registered = registered;
}

void	User::setPasswd(std::string passwd)
{
	this->_passwd = passwd;
}

void	User::setUserName(std::string username)
{
	this->_userName = username;
}

void	User::setRealName(std::string realname)
{
	this->_realName = realname;
}
