/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/07 22:33:37 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include "../inc/utils.hpp"
#include "../inc/User.hpp"
#include "../inc/Server.hpp"
#include "../inc/Numerics.hpp"


# define WAITING_FOR_PASS 0
# define PASS_ORDER_OK 1
# define PASS_ORDER_ERROR -1
# define PASS_NICK_OK 2
# define PASS_USER_OK 3
# define PASS_CONNECTION_ERROR 4

User::User(void): _fd(0), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), \
_pass_before_nick_user(WAITING_FOR_PASS), _modes()
{
	_user_name.push_back('*');
	_nick.push_back('*');
	_modes.insert(std::make_pair('a', false));
	_modes.insert(std::make_pair('i', true));
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('O', false));
	_modes.insert(std::make_pair('r', false));
	_modes.insert(std::make_pair('w', false));
	_modes.insert(std::make_pair('s', false));
}

User::User(int fd): _fd(fd), _pass_status(PASSWORD_NOT_SET), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), \
_pass_before_nick_user(WAITING_FOR_PASS), _modes()
{
	_user_name.push_back('*');
	_nick.push_back('*');
	_modes.insert(std::make_pair('a', false));
	_modes.insert(std::make_pair('i', true));
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('O', false));
	_modes.insert(std::make_pair('r', false));
	_modes.insert(std::make_pair('w', false));
	_modes.insert(std::make_pair('s', false));
}

User::User(const User & copy): _fd(copy._fd), _pass_status(copy._pass_status), _registered(copy._registered), \
_passwd(copy._passwd), _user_name(copy._user_name), _real_name(copy._real_name), \
_nick(copy._nick), _channels(copy._channels), _user_mask(copy._user_mask), \
_pass_before_nick_user(copy._pass_before_nick_user), _modes(copy._modes)
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
    _channels = src._channels;
	_user_mask = src._user_mask;
	_currCmd = src._currCmd;
	_modes = src._modes;
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

bool User::getMode(char c)
{
	return (_modes[c]);
}

std::map<char, bool>::iterator User::getModesbg(void)
{
	return (_modes.begin());
}

std::map<char, bool>::iterator User::getModesend(void)
{
	return (_modes.end());
}

void User::setMode(char c, bool mode) // easier way to do it?
{
	std::map<char, bool>::iterator it;

	for (it = _modes.begin(); it != _modes.end(); ++it)
	{
		if (it->first == c)
			it->second = mode;
	}
}

std::vector<unsigned char> User::getPasswd(void) const
{
	return (_passwd);
}

std::vector<unsigned char>::iterator User::getNickbg(void)
{
	return (_nick.begin());
}

std::vector<unsigned char>::iterator User::getNickend(void)
{
	return (_nick.end());
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

std::vector<unsigned char> User::getNick(void) const
{
	return (_nick);	
}

std::vector<unsigned char> User::getUserMask(void) const
{
	return (_user_mask);
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

void User::delChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = _channels.begin();
	
	while (it != _channels.end())
	{
		if (*it == channel)
		{
			_channels.erase(it);
			return ;
		}
		it++;
	}
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
	if (val == true)
		setMode('o', true);
	else
		setMode('o', false);
}

void	User::clearCurrCmd(void)
{
	_currCmd.clear();
}

void	User::clearRet(void)
{
	_ret.clear();
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

bool User::isInChan(Channel *channel)
{
	for (std::vector<Channel *>::size_type it = 0; it < _channels.size(); it++)
	{
		if (_channels[it] == channel)
			return (true);
	}
	return (false);
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

std::vector<unsigned char> User::getUserModes(void)
{
	std::vector<unsigned char> ret = to_vector(" +");
	for (std::map<char, bool>::iterator mode = this->getModesbg(); mode != this->getModesend(); mode++)
	{
		if (mode->second == true)
			ret.push_back(mode->first);
	}
	ret.push_back('\r');
	ret.push_back('\n');
	return (ret);
}

int User::modesMessage(std::vector<unsigned char> input, bool isUserCommand)
{
	std::vector<unsigned char> ret = to_vector(":");
	std::vector<unsigned char> modes;
	std::vector<unsigned char> error_msg;
	std::vector<unsigned char>::size_type it = 0;
	int add_or_remove = 1;
	int error = 0;
	
	if (isUserCommand == true)
	{
		add_to_v(ret, getNick());
		add_to_v(ret, to_vector(" "));
	}
	add_to_v(ret, to_vector("MODE "));
	add_to_v(ret, getNick());
	add_to_v(ret, to_vector(" :"));
	if (input[0] == '-')
	{
		ret.push_back('-');
		add_or_remove = -1;
		it++;
	}
	else
		ret.push_back('+');
	if (input[0] == '+')
		it++;
	for (;it < input.size(); it++)
	{
		if (isValidUserMode(input[it])
			&& isCharInVector(modes, input[it]) == false)
		{
			if (add_or_remove == 1
				&& getMode(input[it]) == false
				&& input[it] != 'o')
			{
				setMode(input[it], true);
				modes.push_back(input[it]);
			}
			else if (add_or_remove == -1
				&& getMode(input[it]) == true)
			{
				setMode(input[it], false);
				modes.push_back(input[it]);
			}
		}
		else if (!isValidUserMode(input[it]))
			error = 1;
	}
	if (modes.size() == 0 && !error)
		return (0);
	if (error == 1)
	{
		add_to_v(error_msg, to_vector(":"));
		add_to_v(error_msg, getClient());
		add_to_v(error_msg, ERR_UMODEUNKNOWNFLAGmsg(ERR_UMODEUNKNOWNFLAG, _nick));
		_ret.insert(_ret.end(), error_msg.begin(), error_msg.end());
	}
	if (modes.size() > 0)
	{
		add_to_v(ret, modes);
		add_to_v(ret, to_vector("\r\n"));
		_ret.insert(_ret.end(), ret.begin(), ret.end());
	}
	return (1);
}