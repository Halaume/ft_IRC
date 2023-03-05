/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/05 19:07:47 by iguscett         ###   ########.fr       */
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

Channel::Channel(std::vector<unsigned char> name): _chan_name(name), _chan_password(), _op_list(), _user_list(), \
_ban_list(), _nb_users_limit(), _invite_list(), _topic()
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
	_chan_name.erase(_chan_name.begin(), _chan_name.end());
	_chan_password.erase(_chan_password.begin(), _chan_password.end());
	_modes.erase(_modes.begin(), _modes.end());
	_op_list.erase(_op_list.begin(), _op_list.end());
	_user_list.erase(_user_list.begin(), _user_list.end());
	_ban_list.erase(_ban_list.begin(), _ban_list.end());
	_invite_list.erase(_invite_list.begin(), _invite_list.end());
	_topic.erase(_topic.begin(), _topic.end());
}

Channel& Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
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

void	Channel::delUser(int fd)
{
	for (std::list<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); it++)
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

// Getters

std::vector<unsigned char>	Channel::getChanName(void) const
{
	return (_chan_name);
}

void Channel::addUserToBan(User *new_user)
{
	// verify if user not already in list..
	// check list limit etc...
	_ban_list.push_back(new_user);
}

void Channel::addUserToInvite(User *new_user)
{
	// verify if user not already in list..
	// check list limit etc...
	print_vector("nick in chan", new_user->getNick());
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

// SETTERS
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
	
	for (it = this->_user_list.begin(); it != this->_user_list.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return (it);
	}
	return (it);
}

std::vector<unsigned char> Channel::getChannelModes(void)
{
	std::vector<unsigned char> ret = to_vector(" ");
	
	add_to_v(ret, _chan_name);
	add_to_v(ret, to_vector(" +"));
	for (std::map<char, bool>::iterator mode = this->getModesbg(); mode != this->getModesend(); mode++)
	{
		if (mode->second == true)
			ret.push_back(mode->first);
	}
	ret.push_back('\r');
	ret.push_back('\n');
	return (ret);
}

void Channel::operatorModeFct(Server &my_server, User *user, std::vector<std::vector<unsigned char> > input, int &return_value)
{
	std::vector<unsigned char> op_msg_bg;
	std::vector<unsigned char> op_msg;
	User *user_target;
	int add_or_remove = 1;
	
	if (input.size() == 3 || input[3].empty()) // afficher la liste?
		return;
	if (input[2][0] == '-')
		add_or_remove = -1;
	user_target = my_server.findUserPtrNick(input[3]);
	if (user_target == NULL)
	{
		push_to_buf(ERR_NOSUCHNICK, user, input[3]);
		return_value++;
	}
	else
	{
		op_msg.clear();
		op_msg = input[1];
		add_to_v(op_msg, to_vector(" "));
		add_to_v(op_msg, input[3]);
		add_to_v(op_msg, to_vector(" :"));
		if (add_or_remove == 1)
			add_to_v(op_msg, to_vector(" +o"));
		else
			add_to_v(op_msg, to_vector(" -o"));
		add_to_v(op_msg, to_vector("\r\n"));
		push_to_buf(MODE_CHANOPERSET, user, op_msg);
		op_msg_bg = to_vector(":" + server_name + " ");
		add_to_v(op_msg_bg, user->getNick());
		if (add_or_remove == 1 && !isOp(*user_target))
		{
			_op_list.push_back(user_target);
			if (user_target->isInChan(this) == false)
			{
				_user_list.push_back(user_target);
				user_target->addChannel(this);
				add_to_v(op_msg_bg, to_vector(" has made you channel operator on "));
				add_to_v(op_msg_bg, _chan_name);
				add_to_v(op_msg_bg, to_vector("\r\n"));
				message_to_user(my_server, my_server.findUserPtrNick(user_target->getNick()), op_msg_bg);
			}
		}
		else if (add_or_remove == -1 && isOp(*user_target))
		{
			add_to_v(op_msg_bg, to_vector(" has removed you from the channel operators on "));
			add_to_v(op_msg_bg, _chan_name);
			add_to_v(op_msg_bg, to_vector("\r\n"));
			message_to_user(my_server, my_server.findUserPtrNick(user_target->getNick()), op_msg_bg);
			delOpLst(user_target);
		}
		return_value++;
	}
}

void Channel::inviteModeFct(User *user, std::vector<std::vector<unsigned char> > input, int &return_value)
{
	if (input[2][0] == '-')
		setMode('i', false);
	else
		setMode('i', true);
	push_to_buf(RPL_CHANNELMODEIS, user, getChannelModes());
	return_value++;
}

int Channel::modesMessage(Server &my_server, User *user, std::vector<std::vector<unsigned char> > input, bool isUserCommand)
{
	std::vector<unsigned char> ret = to_vector(":");
	std::vector<unsigned char> modes;
	std::vector<unsigned char>::size_type j = 0;
	int return_value = 0;
	int add_or_remove = 1; (void) add_or_remove;
	
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
		add_or_remove = -1;
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
		if (input[2][j] == 'o')
			operatorModeFct(my_server, user, input, return_value);
		else if (input[2][j] == 'i')
			inviteModeFct(user, input, return_value);
			// HHHHHHHHHHHHHHHHHHEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRREEEEEEEEEEEEEE

	
	}
	std::cout << "return valueeeeeeeeeeeeeeeeee:" << return_value << std::endl;
	return (1);
}