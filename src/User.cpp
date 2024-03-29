/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:10:59 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/09 06:54:50 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include "../inc/utils.hpp"
#include "../inc/User.hpp"
#include "../inc/Server.hpp"
#include "../inc/Numerics.hpp"

User::User(void): _fd(0), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), \
_pass_before_nick_user(WAITING_FOR_PASS), _modes(), _fatal_error(0)
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
	this->_operator = false;
}

User::User(int fd): _fd(fd), _registered(false), _passwd(), \
_user_name(), _real_name(), _nick(), _channels(), _user_mask(), \
_pass_before_nick_user(WAITING_FOR_PASS), _modes(), _fatal_error(0)
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
	this->_operator = false;
}

User::User(const User & copy): _fd(copy._fd), _operator(copy._operator), _registered(copy._registered), \
_passwd(copy._passwd), _user_name(copy._user_name), _real_name(copy._real_name), \
_nick(copy._nick), _channels(copy._channels), _user_mask(copy._user_mask), \
_pass_before_nick_user(copy._pass_before_nick_user), _modes(copy._modes), _fatal_error(0)
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
	_registered = src._registered;
	_passwd = src._passwd;
	_user_name = src._user_name;
	_real_name = src._real_name;
	_channels = src._channels;
	_user_mask = src._user_mask;
	_currCmd = src._currCmd;
	_nick = src._nick;
	_pass_before_nick_user = src._pass_before_nick_user;
	this->_operator = src._operator;
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

void User::setFatalError(int fatal_error)
{
	_fatal_error = fatal_error;
}

int User::getFatalError(void)
{
	return (_fatal_error);
}

std::map<char, bool>::iterator User::getModesend(void)
{
	return (_modes.end());
}

void User::setMode(char c, bool mode)	
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

std::vector<unsigned char>::const_iterator User::getNickbg(void) const
{
	return (_nick.begin());	
}

std::vector<unsigned char>::const_iterator User::getNickend(void) const
{
	return (_nick.end());	
}

bool User::getRegistered(void) const
{
	return (_registered);
}

std::vector<unsigned char> &User::getRet(void)
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

std::vector<Channel *>::iterator User::getChannelsbg(void)
{
	return (_channels.begin());
}

std::vector<Channel *>::iterator User::getChannelsend(void)
{
	return (_channels.end());
}

std::vector<unsigned char>::iterator User::getCurrCmdbg(void)
{
	return (_currCmd.begin());
}

std::vector<unsigned char>::iterator User::getCurrCmdend(void)
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
	o << " Is op: " << i.getOperator() << std::endl;
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
	int add_or_remove = ADD;
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
		add_or_remove = REMOVE;
		it++;
	}
	else
		ret.push_back('+');
	if (input[0] == '+')
		it++;
	for (;it < input.size(); it++)
	{
		if (isValidUserMode(input[it]))
		{
			if (add_or_remove == ADD && getMode(input[it]) == false && input[it] != 'o')
			{
				setMode(input[it], true);
				modes.push_back(input[it]);
			}
			else if (add_or_remove == REMOVE && getMode(input[it]) == true)
			{
				setMode(input[it], false);
				modes.push_back(input[it]);
			}
		}
		else if (!isValidUserMode(input[it]))
			error = 1;
	}
	if (error == 1)
	{
		add_to_v(error_msg, to_vector(":"));
		add_to_v(error_msg, getClient());
		add_to_v(error_msg, ERR_UMODEUNKNOWNFLAGmsg(ERR_UMODEUNKNOWNFLAG, getNick()));
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

std::vector<unsigned char>::const_iterator	User::getUserNamebg(void) const
{
	return (this->_user_name.begin());
}

std::vector<unsigned char>::const_iterator	User::getUserNameend(void) const
{
	return (this->_user_name.end());
}

std::vector<Channel *>::iterator	User::getChanIt(std::vector<unsigned char> chanName)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		if ((*it)->getChanName() == chanName)
			return (it);
	return (this->_channels.end());
}

void	User::del_chan(std::vector<Channel *>::iterator chan)
{
	this->_channels.erase(chan);
}

void User::initialize(void)
{
	std::vector<unsigned char> empty;
	
	clearCurrCmd();
	setOperator(false);
	setRegistered(false);
	setPasswd(empty);
	setUserName(empty);
	setRealName(empty);
	setNick(empty);
	setRet(empty);
	setUserMask(empty);
	setPassBeforeNickUser(WAITING_FOR_PASS);
	setFatalError(0);
	setOperator(false);
	setMode('a', false);
	setMode('i', true);
	setMode('o', false);
	setMode('O', false);
	setMode('r', false);
	setMode('w', false);
	setMode('s', false);
}