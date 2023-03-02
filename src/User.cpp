/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/02 16:39:10 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include "../inc/utils.hpp"
#include "../inc/User.hpp"
#include "../inc/Server.hpp"

# define WAITING_FOR_PASS 0
# define PASS_ORDER_OK 1
# define PASS_ORDER_ERROR -1
# define PASS_NICK_OK 2
# define PASS_USER_OK 3
# define PASS_CONNECTION_ERROR 4

User::User(void): _fd(0), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), _pass_before_nick_user(WAITING_FOR_PASS)
{
	_user_name.push_back('*');
	_nick.push_back('*');
}

User::User(int fd): _fd(fd), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), _pass_before_nick_user(WAITING_FOR_PASS)
{
	_user_name.push_back('*');
	_nick.push_back('*');
}

User::User(const User & copy): _fd(copy._fd), _pass_status(copy._pass_status), _registered(copy._registered), \
_passwd(copy._passwd), _user_name(copy._user_name), _real_name(copy._real_name), \
_nick(copy._nick), _channels(copy._channels), _user_mask(copy._user_mask), _pass_before_nick_user(copy._pass_before_nick_user)
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
	this->_real_name = src._real_name;
	this->_channels = src._channels;
	this->_user_mask = src._user_mask;
	this->_currCmd = src._currCmd;
	this->_nick = src._nick;
	this->_pass_before_nick_user = src._pass_before_nick_user;
	return (*this);
}

bool User::operator==(User test) const
{
	return (_fd == test._fd);
}

bool	User::operator==(std::vector<unsigned char> test) const
{
	return (_user_name == test);
}

bool	User::operator!=(User test) const
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
	return (_fd);
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
	return (_registered);
}

std::vector<unsigned char> &	User::getRet(void)
{
	return (_ret);
}

std::vector<unsigned char>&	User::getCurrCmd(void)
{
	return (_currCmd);
}

int User::getPassBeforeNickUser(void) const
{
	return (_pass_before_nick_user);
}

std::vector<Channel *>::iterator	User::getChannelsbg(void)
{
	return (_channels.begin());
}

std::vector<Channel *>::iterator	User::getChannelsend(void)
{
	return (_channels.end());
}

std::vector<unsigned char>::iterator	User::getCurrCmdbg(void)
{
	return (_currCmd.begin());
}

std::vector<unsigned char>::iterator	User::getCurrCmdend(void)
{
	return (_currCmd.end());
}

std::vector<unsigned char>::const_iterator	User::getUserNamebg(void) const
{
	return (this->_user_name.begin());
}

std::vector<unsigned char>::const_iterator	User::getUserNameend(void) const
{
	return (this->_user_name.end());
}

std::vector<unsigned char>	User::getUserName(void) const
{
	return (_user_name);
}

std::vector<unsigned char>	User::getRealName(void) const
{
	return (_real_name);
}

void	User::setRegistered(bool registered)
{
	_registered = registered;
}

void	User::setPasswd(std::vector<unsigned char> passwd)
{
	_passwd = passwd;
}

void	User::setRet(std::vector<unsigned char> ret)
{
	_ret = std::vector<unsigned char>(ret);
}

void User::setUserName(std::vector<unsigned char> userName)
{
	_user_name = userName;
}

void	User::setRealName(std::vector<unsigned char> realname)
{
	_real_name = realname;
}

void User::setPassBeforeNickUser(int value)
{
	_pass_before_nick_user = value;
}

int	User::getNbChan(void)
{
	return (static_cast<int>(_channels.size()));
}

std::vector<Channel *> User::getChannels(void) const
{
	return (_channels);
}

std::vector<unsigned char>::const_iterator User::getNickbg(void) const
{
	return (_nick.begin());	
}

std::vector<unsigned char>::const_iterator User::getNickend(void) const
{
	return (_nick.end());	
}

std::vector<unsigned char> User::getNick(void) const
{
	return (_nick);	
}

std::vector<unsigned char> User::getUserMask(void) const
{
	return (_user_mask);
}

void User::setNick(std::vector<unsigned char> nick)
{
	_nick = nick;
}

void User::setPassStatus(int pass_status)
{
	_pass_status = pass_status;
}

void User::setfd(int fd)
{
	_fd = fd;
}

void User::setUserMask(std::vector<unsigned char>& user_mask)
{
	_user_mask = user_mask;
}

void User::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

bool	User::getOperator(void) const
{
	return (_operator);
}

void	User::setOperator(bool val)
{
	_operator = val;
}

void	User::clearCurrCmd(void)
{
	_currCmd.clear();
}

void	User::clearRet(void)
{
	_ret.clear();
}

std::vector<unsigned char> User::getClient(void)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char>::size_type it;

	for (it = 0; it < _nick.size(); ++it)
		ret.push_back(_nick[it]);
	ret.push_back('!');
	for (it = 0; it < _user_name.size(); ++it)
		ret.push_back(_user_name[it]);
	ret.push_back('@');
	for (it = 0; it < _user_mask.size(); ++it)
		ret.push_back(_user_mask[it]);
	return (ret);
}

int User::createNewNick(Server &my_server)
{
	std::vector<unsigned char> nick_mem;
	std::vector<unsigned char> num;
	std::vector<unsigned char>::size_type pos;
	int it = 1;
	std::vector<unsigned char>::size_type numit;
	int lastnum = 1;

	num = _nick;
	if (_nick.size() == 9)
		_nick[8] = '_';
	else
		_nick.push_back('_');
	if (my_server.nbConnectionsWithSameNick(*this) == 1)
		return (0);
	_nick = num;
	nick_mem = _nick;
	while (my_server.nbConnectionsWithSameNick(*this) > 1 && lastnum < 5)
	{
		_nick = nick_mem;
		num.clear();
		num = numToVec(it++);
		if (_nick.size() + num.size() > 9)
			pos = _nick.size() - num.size() + 1;
		else
			pos = _nick.size();
		numit = 0;
		for (std::vector<unsigned char>::size_type p = 0; p < 9; p++)
		{
			if (p >= pos && p < _nick.size())
				_nick[p] = num[numit++];
			else if (p >= pos && p >= _nick.size() && numit < num.size())
				_nick.push_back(num[numit++]);
		}
		lastnum++;
	}
	if (lastnum == 5)
		return (1);
	return (0);
}

void	User::insertcmd(std::vector<unsigned char> & vec)
{
	_currCmd.insert(_currCmd.end(), vec.begin(), vec.end());
}

std::ostream &		operator<<( std::ostream & o, User const & i)
{
	std::vector<unsigned char>::size_type m;

	o << " Usrn: ";
	for (m = 0; m < i.getUserName().size(); m++)
		o << i.getUserName()[m];
	o << " Rln: ";
	for (m = 0; m < i.getRealName().size(); m++)
		o << i.getRealName()[m];
	o << " Nick: ";
	for (m = 0; m < i.getNick().size(); m++)
		o << i.getNick()[m];
	o << " fd: " << i.getfd() << " usr reg:" << i.getRegistered() << " passwd:";
	for (m = 0; m < i.getPasswd().size(); m++)
		o << i.getPasswd()[m];
	// o << " Ad: " << &i;
	o << std::endl;

	return o;
}
