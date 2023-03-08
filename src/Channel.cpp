/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 18:07:09 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"


Channel::Channel(void): _chan_name(), _chan_password(), _op_list(), _user_list(), _ban_list(), _nb_users_limit(), \
							_invite_list(), _topic()
{
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('p', false));
	_modes.insert(std::make_pair('s', false));
	_modes.insert(std::make_pair('i', false));
	_modes.insert(std::make_pair('t', false));
	_modes.insert(std::make_pair('n', false));
	_modes.insert(std::make_pair('m', false));
	_modes.insert(std::make_pair('l', false));
	_modes.insert(std::make_pair('b', false));
	_modes.insert(std::make_pair('v', false));
	_modes.insert(std::make_pair('k', false));
	_topic = to_vector("default topic");
}

Channel::Channel(const Channel & copy): _chan_name(copy._chan_name), _chan_password(copy._chan_password), \
_modes(copy._modes), _op_list(copy._op_list), _user_list(copy._user_list), _ban_list(copy._ban_list), \
_nb_users_limit(copy._nb_users_limit), _invite_list(copy._invite_list), _topic(copy._topic)
{
}

Channel::Channel(std::vector<unsigned char> name): _chan_password(), _op_list(), _user_list(), \
_ban_list(), _nb_users_limit(), _invite_list()
{
	this->_chan_name = name;
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('p', false));
	_modes.insert(std::make_pair('s', false));
	_modes.insert(std::make_pair('i', false));
	_modes.insert(std::make_pair('t', false));
	_modes.insert(std::make_pair('n', false));
	_modes.insert(std::make_pair('m', false));
	_modes.insert(std::make_pair('l', false));
	_modes.insert(std::make_pair('b', false));
	_modes.insert(std::make_pair('v', false));
	_modes.insert(std::make_pair('k', false));
	_topic = to_vector("default topic");
}

Channel::Channel(std::vector<unsigned char> name, std::vector<unsigned char> password): _chan_name(name), \
_chan_password(password), _op_list(), _user_list(), _nb_users_limit(), _invite_list(), _topic()
{
	_modes.insert(std::make_pair('o', false));
	_modes.insert(std::make_pair('p', false));
	_modes.insert(std::make_pair('s', false));
	_modes.insert(std::make_pair('i', false));
	_modes.insert(std::make_pair('t', false));
	_modes.insert(std::make_pair('n', false));
	_modes.insert(std::make_pair('m', false));
	_modes.insert(std::make_pair('l', false));
	_modes.insert(std::make_pair('b', false));
	_modes.insert(std::make_pair('v', false));
	_modes.insert(std::make_pair('k', false));
	_topic = to_vector("default topic");
}

Channel::~Channel(void)
{
}

Channel& Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
	_topic = src._topic;
	_modes = src._modes;
	_chan_name = src._chan_name;
	_chan_password = src._chan_password;
	_op_list = src._op_list;
	_user_list = src._user_list;
	_nb_users_limit = src._nb_users_limit;
	_invite_list = src._invite_list;
	_topic = src._topic;
	return (*this);
}

bool Channel::operator!=(const Channel & lhs) const
{
	return (this->_chan_name != lhs._chan_name);
}

std::vector<unsigned char>& Channel::getTopic(void)
{
	return (_topic);
}

std::map<char, bool> Channel::getModes(void) const
{
	return (_modes);
}

void Channel::setTopic(std::vector<unsigned char> topic)
{
	this->_topic = topic;
}

bool    Channel::isOp(User usr) const
{
	for (std::list<User *>::const_iterator it = _op_list.begin(); it != _op_list.end(); it++)
	{
		if (**it == usr)
			return (true);
	}
	return (false);
}

void Channel::addUser(User *user)
{
	if (_user_list.size() == 0)
		_op_list.push_back(user);
	_user_list.push_back(user);
	user->addChannel(this);
}

void Channel::delUserLst(User *user)
{
	std::list<User *>::iterator it = getUserListbg();

	while (it != getUserListend())
	{
		if (*it == user)
		{
			_user_list.erase(it);
			break ;
		}
		it++;
	}
	user->delChannel(this);
}

void Channel::delOpLst(User *user)
{
	std::list<User *>::iterator it = getOpListbg();

	while (it != getOpListend())
	{
		if (*it == user)
		{
			_op_list.erase(it);
			break ;
		}
		it++;
	}
}

void Channel::delBanLst(User *user)
{
	std::list<User *>::iterator it = getUserListBanbg();

	while (it != getUserListBanend())
	{
		if (*it == user)
		{
			_ban_list.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::delUser(int fd)
{
	for (std::list<User *>::iterator it = this->_user_list.begin();it != this->_user_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			this->_user_list.erase(it);
	}
	for (std::list<User *>::iterator it = this->_op_list.begin(); it != this->_op_list.end(); it++)
	{
		if ((*it)->getfd() == fd)
			this->_op_list.erase(it);
	}
}

std::vector<unsigned char>	Channel::getChanName(void) const
{
	return (_chan_name);
}

void Channel::addUserToBan(User *new_user)
{
	_ban_list.push_back(new_user);
}

void Channel::addUserToInvite(User *new_user)
{
	_invite_list.push_back(new_user);
}

std::list<User *>::iterator	Channel::getOpListbg(void)
{
	return (this->_op_list.begin());
}

std::list<User *>::iterator	Channel::getOpListend(void)
{
	return (this->_op_list.end());
}

std::list<User *>::iterator	Channel::getUserListbg(void)
{
	return (this->_user_list.begin());
}

std::list<User *>::iterator	Channel::getUserListend(void)
{
	return (this->_user_list.end());
}

std::list<User *>	Channel::getUserList(void)
{
	return (this->_user_list);
}

std::vector<unsigned char>	Channel::getChanPassword(void) const
{
	return (_chan_password);
}

std::list<User *>::iterator	Channel::getUserListOpbg(void)
{
	return (_op_list.begin());
}

std::list<User *>::iterator	Channel::getUserListOpend(void)
{
	return (_op_list.end());
}

std::list<User *>::iterator	Channel::getUserListBanbg(void)
{
	return (_ban_list.begin());
}

std::list<User *>::iterator	Channel::getUserListBanend(void)
{
	return (_ban_list.end());
}

std::list<User *>::iterator	Channel::getUserListInvitebg(void)
{
	return (_invite_list.begin());
}

std::list<User *>::iterator	Channel::getUserListInviteend(void)
{
	return (_invite_list.end());
}

std::list<User *> Channel::getUsers(void) const
{
	return (_user_list);
}

int Channel::getNbUsers(void)
{
	return (static_cast<int>(_user_list.size()));
}

int Channel::getNbUsersLimit(void)
{
	return (_nb_users_limit);
}

void Channel::makeOp(User *user)
{
	if (user == NULL)
		return;
	_op_list.push_back(user);
}

bool	Channel::isOp(User *usr) const
{
	for (std::list<User *>::const_iterator it = this->_op_list.begin(); it != this->_op_list.end(); it++)
		if (*it == usr)
			return (true);
	return (false);
}

bool Channel::getMode(char c)
{
	return (_modes[c]);
}

std::map<char, bool>::iterator Channel::getModesbg(void)
{
	return (_modes.begin());
}

std::map<char, bool>::iterator Channel::getModesend(void)
{
	return (_modes.end());
}

void Channel::setChanName(std::vector<unsigned char> chan_name)
{
	_chan_name = chan_name;
}

void Channel::setChanPassword(std::vector<unsigned char>& chan_password)
{
	_chan_password = chan_password;
}

void Channel::setNbUsersLimit(int nb_users_limit)
{
	_nb_users_limit = nb_users_limit;
}

void Channel::setMode(char c, bool mode)
{
	std::map<char, bool>::iterator it;

	for (it = _modes.begin(); it != _modes.end(); ++it)
	{
		if (it->first == c)
			it->second = mode;
	}
}

bool Channel::isUserInChannel(User *user)
{
	std::list<User *>::iterator it;

	for (it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

bool Channel::isUserBanned(User *user)
{
	std::list<User *>::iterator it;

	for (it = _ban_list.begin(); it != _ban_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

bool Channel::isUserInvited(User *user)
{
	std::list<User *>::iterator it;

	for (it = _invite_list.begin(); it != _invite_list.end(); ++it)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

std::list<User *>::iterator	Channel::findUser(std::vector<unsigned char> nick)
{
	std::list<User *>::iterator it;
	std::vector<unsigned char> vec = this->_chan_name;
	vec.push_back('\0');
	std::cerr << "name of the Channel : " << vec.data() << std::endl;
	for (std::list<User *>::iterator j = this->_user_list.begin(); j != this->_user_list.end(); j++)
	{
		vec = (*j)->getNick();
		vec.push_back('\0');
		std::cerr << "Name of users : " << vec.data() << std::endl;
	}

	for (it = this->_user_list.begin(); it != this->_user_list.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return (it);
	}
	return (it);
}

std::vector<unsigned char>::iterator	Channel::getChanNamebg(void)
{
	return (this->_chan_name.begin());
}

std::vector<unsigned char>::iterator	Channel::getChanNameend(void)
{
	return (this->_chan_name.end());
}

std::vector<unsigned char> Channel::getChannelModes(void)
{
	std::vector<unsigned char> ret = to_vector(" ");
	std::vector<unsigned char> nb_limit;
	
	add_to_v(ret, _chan_name);
	add_to_v(ret, to_vector(" +"));
	for (std::map<char, bool>::iterator mode = this->getModesbg(); mode != this->getModesend(); mode++)
	{
		if (mode->second == true)
			ret.push_back(mode->first);
	}
	if (getMode('l') == true)
	{
		ret.push_back(' ');
		nb_limit = itov(_nb_users_limit);
		ret.insert(ret.end(), nb_limit.begin(), nb_limit.end());
	}
	if (getMode('k') == true)
	{
		ret.push_back(' ');
		ret.insert(ret.end(), _chan_password.begin(), _chan_password.end());
	}
	ret.push_back('\r');
	ret.push_back('\n');
	return (ret);
}

void Channel::operatorModeFct(Server &my_server, User *user, std::vector<std::vector<unsigned char> > input, int &return_value, int add_or_remove)
{
	std::vector<std::vector<unsigned char> > param;
	std::vector<unsigned char> op_notification;
	std::vector<unsigned char> op_msg;
	User *user_target;
	
	if (input.size() == 3 || input[3].empty())
		return;
	user_target = my_server.findUserPtrNick(input[3]);
	if (user_target == NULL)
	{
		push_to_buf(ERR_NOSUCHNICK, user, input[3]);
		return_value = (return_value == RET_AND_UMODEIS) ? RET_AND_UMODEIS : RET;
	}
	else
	{
		if ((add_or_remove == ADD && isOp(*user_target))
			|| (add_or_remove == REMOVE && !isOp(*user_target)))
			return;
		param.insert(param.end(), input.begin() + 3, input.end());
		op_msg = concatMode(input[1], to_vector("o"), param, add_or_remove);
		push_to_buf(MODE_MESSAGE, user, op_msg);
		return_value = (return_value == RET_AND_UMODEIS) ? RET_AND_UMODEIS : RET;
		op_notification = userMadeOpertorMsg(_chan_name, user, add_or_remove);
		message_to_user(my_server, user_target, op_notification);
		if (add_or_remove == ADD && !isOp(*user_target))
		{
			_op_list.push_back(user_target);
			_user_list.push_back(user_target);
			user_target->addChannel(this);
		}
		else if (add_or_remove == REMOVE && isOp(*user_target))
			delOpLst(user_target);
	}
}

void Channel::inviteModeFct(int &return_value, int add_or_remove)
{
	if (add_or_remove == ADD)
		setMode('i', true);
	else if (add_or_remove == REMOVE)
		setMode('i', false);
	return_value = RET_AND_UMODEIS;
}

void Channel::limitModeFct(std::vector<std::vector<unsigned char> > input, int &return_value, int add_or_remove)
{
	if (!(input.size() == 3 || input[3].empty()) && (add_or_remove == ADD && vtoi(input[3]) > 0))
	{
		setMode('l', true);
		_nb_users_limit = vtoi(input[3]);
	}
	else if (add_or_remove == REMOVE && getMode('l') == true)
	{
		setMode('l', false);
		_nb_users_limit = 0;
	}
	return_value = RET_AND_UMODEIS;
}

void Channel::keyModeFct(User *user, std::vector<std::vector<unsigned char> > input, int &return_value, int add_or_remove)
{
	if (input.size() == 3 || input[3].empty())
		return;
	if (add_or_remove == ADD)
	{
		setMode('k', true);
		_chan_password = input[3];
		return_value = RET_AND_UMODEIS;
	}
	else if (add_or_remove == REMOVE && getMode('k') == true && _chan_password == input[3])
	{
		setMode('k', false);
		_chan_password.clear();
		return_value = RET_AND_UMODEIS;
	}
	else if (add_or_remove == REMOVE && getMode('k') == true && _chan_password != input[3])
	{
		push_to_buf(ERR_INVALIDKEY, user, _chan_name);
		return_value = RET;
	}
}

void Channel::topicModeFct(int &return_value, int add_or_remove)
{
	if (add_or_remove == ADD && getMode('t') == false)
		setMode('t', true);
	else if (add_or_remove == REMOVE && getMode('t') == true)
		setMode('t', false);
	return_value = RET_AND_UMODEIS;
}

void Channel::banModeFct(Server &my_server, User *user, std::vector<std::vector<unsigned char> > input, int &return_value, int add_or_remove)
{
	std::vector<std::vector<unsigned char> > list_users;
	std::vector<unsigned char> ban_msg;
	bool flag = false;
	
	if (input.size() == 3 || input[3].empty())
	{
		ban_msg = concatMode(this, user, user->getNick());
		push_to_buf(RPL_ENDOFBANLIST, user, _chan_name);
		return_value = (return_value == RET_AND_UMODEIS) ? RET_AND_UMODEIS : RET;
		return;
	}
	User *user_to_ban;
	user_to_ban = my_server.findUserPtrNick(input[3]);
	if (user_to_ban != NULL && add_or_remove == ADD && !isUserBanned(user_to_ban))
	{
		_ban_list.push_back(user_to_ban);
		list_users.push_back(input[3]);
		flag = true;
	}
	else if (user_to_ban != NULL && add_or_remove == REMOVE && isUserBanned(user_to_ban))
	{
		list_users.push_back(input[3]);
		delBanLst(user_to_ban);
		flag = true;
	}
	if (flag == true)
	{
		ban_msg = concatMode(input[1], to_vector("b"), list_users, add_or_remove);
		push_to_buf(MODE_MESSAGE, user, ban_msg);
		return_value = (return_value == RET_AND_UMODEIS) ? RET_AND_UMODEIS : RET;
	}
}

int Channel::modesMessage(Server &my_server, User *user, std::vector<std::vector<unsigned char> > input, bool isUserCommand)
{
	std::vector<unsigned char> ret = to_vector(":");
	std::vector<unsigned char> modes;
	std::vector<unsigned char>::size_type j = 0;
	std::string char_to_v;
	int return_value = NO_RET;
	int add_or_remove = ADD;
	
	if (isUserCommand == true)
	{
		add_to_v(ret, user->getNick());
		add_to_v(ret, to_vector(" "));
	}
	add_to_v(ret, to_vector("MODE "));
	add_to_v(ret, input[1]);
	add_to_v(ret, to_vector(" "));
	if (input[2][0] == '-')
	{
		ret.push_back('-');
		add_or_remove = REMOVE;
		j++;
	}
	else if (input[2][0] == '+')
	{
		ret.push_back('+');
		j++;
	}
	else
		ret.push_back('+');
	for (; j < input[2].size(); j++)
	{
		if (input[2][j] == '-')
			add_or_remove = REMOVE;
		else if(input[2][j] == '+')
			add_or_remove = ADD;
		else if (input[2][j] == 'o')
			operatorModeFct(my_server, user, input, return_value, add_or_remove);
		else if (input[2][j] == 'i')
			inviteModeFct(return_value, add_or_remove);
		else if (input[2][j] == 'l')
			limitModeFct(input, return_value, add_or_remove);
		else if (input[2][j] == 'k')
			keyModeFct(user, input, return_value, add_or_remove);
		else if (input[2][j] == 't')
			topicModeFct(return_value, add_or_remove);
		else if (input[2][j] == 'b')
			banModeFct(my_server, user, input, return_value, add_or_remove);
		else
		{
			char_to_v.push_back(input[2][j]);
			push_to_buf(ERR_UNKNOWNMODE, user, to_vector(char_to_v));
			char_to_v.clear();
			return_value = (return_value == RET_AND_UMODEIS) ? RET_AND_UMODEIS : RET;
		}
	}
	if (return_value == RET_AND_UMODEIS)
		push_to_buf(RPL_CHANNELMODEIS, user, getChannelModes());
	if (return_value == NO_RET)
		return (0);
	return (1);
}
