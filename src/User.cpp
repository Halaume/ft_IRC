/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/16 19:42:01 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "../inc/utils.hpp"
#include "../inc/User.hpp"

User::User(void): _fd(), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), _user_name(), _realName(), _client(), _nick(), _channels()
{
	_user_name.push_back('*');
	_client.push_back('*');
	_nick.push_back('*');
}

User::User(int fd): _fd(fd), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), _user_name(), _realName(), _client(), _nick(), _channels()
{
	_user_name.push_back('*');
	_client.push_back('*');
	_nick.push_back('*');
}

User::User(const User & copy): _fd(copy._fd), _pass_status(copy._pass_status), _registered(copy._registered), _passwd(copy._passwd), _user_name(copy._user_name), _realName(copy._realName), _client(copy._client), _nick(copy._nick), _channels(copy._channels)
{
}

User::~User(void)
{
}

User& User::operator=(const User & src)
{
	if (&src == this)
		return (*this);
	this->_fd = src._fd;
    this->_pass_status = src._pass_status;
    this->_registered = src._registered;
    this->_passwd = src._passwd;
    this->_user_name = src._user_name;
    this->_realName = src._realName;
    this->_client = src._client;
    // this->_ret = src._ret;
    this->_channels = src._channels;
	return (*this);
}

bool User::operator==(User test) const
{
	return (this->_fd == test._fd);
}

bool User::operator!=(User test) const
{
	return (!(*this == test));
}

bool User::isNickValid(std::vector<unsigned char> nick)
{
	int j = 0;
	std::vector<unsigned char>::size_type it;
	
	for (it = 0; it < nick.size(); ++it)
	{
		if (isValidCharacter(nick[it]) == false)
			return (false);
		j++;
	}
	if (j > 9)
		return (false);
	return (true);
}

int User::getfd(void) const
{
	return (this->_fd);
}

std::vector<unsigned char> User::getPasswd(void) const
{
	return (_passwd);
}

int User::getPassStatus(void) const
{
	return (_pass_status);
}

bool User::getRegistered(void) const
{
	return (this->_registered);
}

std::vector<unsigned char> User::getUserName(void) const
{
	return (this->_user_name);
}

int	User::getNbChan(void)
{
	return (static_cast<int>(this->_channels.size()));
}

std::vector<Channel>& User::getChannels(void)
{
	return (this->_channels);
}

std::vector<unsigned char> User::getClient(void) const
{
	return (_client);
}

std::vector<unsigned char> User::getNick(void) const
{
	return (_nick);	
}

void User::setUserName(std::vector<unsigned char>& user_name)
{
	_user_name = user_name;
}

void	User::setRealName(std::vector<unsigned char>& realname)
{
	this->_realName = realname;
}

void User::setPasswd(std::vector<unsigned char>& passwd)
{
	_passwd = passwd;
}

void User::setClient(std::vector<unsigned char>& client)
{
	_client = client;
}

void User::setNick(std::vector<unsigned char>& nick)
{
	_nick = nick;
}

void User::setPassStatus(int pass_status)
{
	_pass_status = pass_status;
}

void	User::setRegistered(bool& registered)
{
	this->_registered = registered;
}

std::ostream &		operator<<( std::ostream & o, User const & i)
{
	std::vector<unsigned char>::size_type m;
	
	o << "Client: ";
	for (m = 0; m < i.getClient().size(); m++)
		o << i.getClient()[m];
	o << " Nick: ";
	for (m = 0; m < i.getNick().size(); m++)
		o << i.getNick()[m];
	o << " fd: " << i.getfd() << " user registered:" << i.getRegistered() << " passwd:";
	for (m = 0; m < i.getPasswd().size(); m++)
		o << i.getPasswd()[m];
	o << std::endl;

	return o;
}
