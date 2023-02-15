/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 14:25:41 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <unistd.h>
#include "../inc/User.hpp"

User::User(void): _registered(false), _passwd(), _userName(), _realName(), _client(), _channels()
{
	this->_fd = 0;
}

User::User(const User & copy): _fd(copy._fd), _registered(copy._registered), _passwd(copy._passwd), _userName(copy._userName), _realName(copy._realName), _client(copy._client), _channels(copy._channels)
{
}

User::User(int fd)
{
	this->_fd = fd;
}

User::~User(void)
{
	if (this->_fd != 0)
		close(this->_fd);
	this->_passwd.erase(this->_passwd.begin(), this->_passwd.end());
	this->_userName.erase(this->_userName.begin(), this->_userName.end());
	this->_realName.erase(this->_realName.begin(), this->_realName.end());
	this->_channels.erase(this->_channels.begin(), this->_channels.end());

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

bool	User::operator==(std::vector<unsigned char> test) const
{
	return (this->_userName == test);
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

std::vector<unsigned char>	User::getCurrCmd(void) const
{
	return (this->_currCmd);
}

std::vector<Channel *>::iterator	User::getChannelsbg(void)
{
	return (this->_channels.begin());
}

std::vector<Channel *>::iterator	User::getChannelsend(void)
{
	return (this->_channels.end());
}

std::vector<unsigned char>::iterator	User::getCurrCmdbg(void)
{
	return (this->_currCmd.begin());
}

std::vector<unsigned char>::iterator	User::getCurrCmdend(void)
{
	return (this->_currCmd.end());
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

std::vector<Channel *> User::getChannels(void) const
{
	return (this->_channels);
}

bool	User::getOperator(void) const
{
	return (this->_operator);
}

void	User::setOperator(bool val)
{
	this->_operator = val;
}

void	User::insertCurrCmd(std::vector<unsigned char> vec)
{
	this->_currCmd.insert(this->_currCmd.end(), vec.begin(), vec.end());
}
