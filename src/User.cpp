/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/26 22:51:10 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include "../inc/utils.hpp"
#include "../inc/User.hpp"

# define WAITING_FOR_PASS 0
# define PASS_ORDER_OK 1
# define PASS_ORDER_ERROR -1
# define PASS_NICK_OK 2
# define PASS_USER_OK 3
# define PASS_CONNECTION_ERROR 4

User::User(void): _fd(0), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _client(), _nick(), _channels(), _user_mask(), _pass_before_nick_user(WAITING_FOR_PASS)
{
	_user_name.push_back('*');
	_client.push_back('*');
	_nick.push_back('*');
}

User::User(int fd): _fd(fd), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _client(), _nick(), _channels(), _user_mask(), _pass_before_nick_user(WAITING_FOR_PASS)
{
	_user_name.push_back('*');
	_client.push_back('*');
	_nick.push_back('*');
}

User::User(const User & copy): _fd(copy._fd), _pass_status(copy._pass_status), _registered(copy._registered), \
_passwd(copy._passwd), _user_name(copy._user_name), _real_name(copy._real_name), _client(copy._client), \
_nick(copy._nick), _channels(copy._channels), _user_mask(copy._user_mask), _pass_before_nick_user(copy._pass_before_nick_user), _allCmd(copy._allCmd)
{
}

User::~User(void)
{
}

User& User::operator=(const User & src)
{
	if (&src == this)
		return (*this);
	_fd = src._fd;
    _pass_status = src._pass_status;
    _registered = src._registered;
    _passwd = src._passwd;
    _user_name = src._user_name;
    _real_name = src._real_name;
    _client = src._client;
    _channels = src._channels;
	_user_mask = src._user_mask;
	_currCmd = src._currCmd;
	_allCmd = src._allCmd;
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

std::vector<unsigned char>&	User::getAllCmd(void)
{
	return (_allCmd);
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

std::vector<unsigned char>::iterator	User::getAllCmdbg(void)
{
	return (_allCmd.begin());
}

std::vector<unsigned char>::iterator	User::getAllCmdend(void)
{
	return (_allCmd.end());
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
	_ret = ret;
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

std::vector<unsigned char> User::getClient(void) const
{
	return (_client);
}

std::vector<unsigned char> User::getNick(void) const
{
	return (_nick);	
}

std::vector<unsigned char> User::getUserMask(void) const
{
	return (_user_mask);
}

std::vector<unsigned char> User::getUserNickNameMask(void) const
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

void	User::insertcmd(std::vector<unsigned char> & vec)
{
	_currCmd.insert(_currCmd.end(), vec.begin(), vec.end());
}

void	User::insertAllCmd(std::vector<unsigned char> & vec)
{
	_allCmd.insert(_allCmd.end(), vec.begin(), vec.end());
}

std::ostream &		operator<<( std::ostream & o, User const & i)
{
	std::vector<unsigned char>::size_type m;
	
	o << "Clt: ";
	for (m = 0; m < i.getClient().size(); m++)
		o << i.getClient()[m];
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
