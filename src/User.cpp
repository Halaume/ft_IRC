/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 18:08:42 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "../inc/User.hpp"

User::User(void): _fd(), _pass_status(0), _registered(false), _passwd(), _user_name(), _realName(), _client(), _channels()
{
	_user_name.push_back('*');
	_client.push_back('*');
}

User::User(int fd): _fd(fd), _pass_status(0), _registered(false), _passwd(), _user_name(), _realName(), _client(), _channels()
{
	_user_name.push_back('*');
	_client.push_back('*');
}

User::User(const User & copy): _fd(copy._fd), _pass_status(copy._pass_status), _registered(copy._registered), _passwd(copy._passwd), _user_name(copy._user_name), _realName(copy._realName), _client(copy._client), _channels(copy._channels)
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

std::vector<unsigned char> User::getPasswd(void) const
{
	return (_passwd);
}

int		User::getPassStatus(void) const
{
	return (this->_pass_status);
}

bool	User::getRegistered(void) const
{
	return (this->_registered);
}

std::vector<unsigned char> User::getUserName(void) const
{
	return (this->_user_name);
}

void	User::setPassStatus(int pass_status)
{
	this->_pass_status = pass_status;
}

void	User::setRegistered(bool registered)
{
	this->_registered = registered;
}

void User::setUserName(std::vector<unsigned char> user_name)
{
	this->_user_name = user_name;
	// std::vector<unsigned char>::size_type m;
	// std::cout << "un1:";
	// for (m = 0; m < user_name.size(); m++)
	// 	std::cout << user_name[m];
	// std::cout << "\nun2:";
	// for (m = 0; m < _user_name.size(); m++)
	// 	std::cout << _user_name[m];
}

void	User::setRealName(std::vector<unsigned char> realname)
{
	this->_realName = realname;
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

void User::setPasswd(std::vector<unsigned char> passwd)
{
	_passwd.clear();
	_passwd = passwd;
}

std::ostream &		operator<<( std::ostream & o, User const & i)
{
	std::vector<unsigned char>::size_type m;
	
	o << "Username: ";
	for (m = 0; m < i.getUserName().size(); m++)
		o << i.getUserName()[m];
	o << " fd: " << i.getfd() << " user registered:" << i.getRegistered() << " passwd:";
	for (m = 0; m < i.getPasswd().size(); m++)
		o << i.getPasswd()[m];
	o << std::endl;

	return o;
}
