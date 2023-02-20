/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 22:40:16 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/User.hpp"
#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"

std::string server_name = "mig.42.fr";

std::vector<unsigned char> no_param;

Command::Command(void):  _globalCmd(), _parsedCmd(), _cmd_fd_user(), _cmd_buf(), _error(0), _cmd_user(NULL), _pass_before_nick_user(WAITING_FOR_PASS)
{
}

Command::Command(const Command &copy): _globalCmd(copy._globalCmd), _parsedCmd(copy._parsedCmd), _cmd_fd_user(copy._cmd_fd_user) \
, _cmd_buf(copy._cmd_buf), _error(copy._error), _cmd_user(copy._cmd_user), _pass_before_nick_user(copy._pass_before_nick_user)
{

}

Command::~Command(void)
{
}

Command &	Command::operator=(const Command & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

void Command::setCmdUser(Server &my_server)
{
	_cmd_user = my_server.findUser(_cmd_fd_user);
}


/*''''''''''''''''''''''''''''''''''''
				CAP > to delete?
				-> if NICK/USER before PASS command is down
				-> if pass mismatches command is down
''''''''''''''''''''''''''''''''''''''*/
void Command::register_user(Server & my_server)
{
	
	(void)my_server;
	if (my_compare(_cmd_user->getPasswd(), my_server.getPasswd()))
	{
		push_to_buf(ERR_PASSWDMISMATCH, *this, no_param); // exit disconnect client?
		_pass_before_nick_user = PASS_CONNECTION_ERROR;
		return;
	}
	(*_cmd_user).setRegistered(true);
	push_to_buf(RPL_WELCOME, *this, no_param);
	// sendto
	push_to_buf(RPL_YOURHOST, *this, no_param);
	// sendto
	push_to_buf(RPL_CREATED, *this, no_param);
	// sendto
	push_to_buf(RPL_MYINFO, *this, no_param);
	// sendto
}

/*''''''''''''''''''''''''''''''''''''
				PASS TODO + sendto +++ exit and close connection when password mismatch
				Verifier que pas nick ou USER
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_PASS(Server &my_server)
{
	(void)my_server;
	std::vector<unsigned char> v;
	
	if ((_parsedCmd.size() < 2 || _parsedCmd[1].empty() == true) && _cmd_user->getRegistered() == false) // && !_cmd_user->getPassStatus())
	{
		push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param);
		// sendto
		return;
	}
	else if (_cmd_user->getRegistered()) // || _cmd_user->getPassStatus())
	{
		push_to_buf(ERR_ALREADYREGISTERED, *this, no_param);
		// sendto
		return;
	}
	(*_cmd_user).setPasswd(_parsedCmd[1]);
	(*_cmd_user).setPassStatus(PASSWORD_SET);
	if (_pass_before_nick_user == WAITING_FOR_PASS)
		_pass_before_nick_user = PASS_ORDER_OK;
}

/*''''''''''''''''''''''''''''''''''''
				NICK TODO  modifier le message comme décrit ci-dessous?
				The NICK message may be sent from the server to clients to acknowledge their NICK command
				was successful and to inform other clients about the change of nickname.
				In these cases, the <source> of the message will be the old nickname 
				[ [ "!" user ] "@" host ] of the user who is changing their nickname.
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_NICK(Server &my_server)
{
	if (_pass_before_nick_user == WAITING_FOR_PASS)
	{
		_pass_before_nick_user = PASS_ORDER_ERROR;
		return;
	}
	(void)my_server; //
	if (_parsedCmd.size() < 2)
	{
		push_to_buf(ERR_NONICKNAMEGIVEN, *this, no_param);
		return;
	}
	std::list<User>::iterator itu = my_server.findUserNick(_parsedCmd[1]);
	if (_cmd_user->isNickValid(_parsedCmd[1]) == false)
	{
		push_to_buf(ERR_ERRONEUSNICKNAME, *this, no_param);
		return;
	}
	else if (itu != my_server.getUsers().end())
	{
		if (!my_compare(_cmd_user->getNick(), _parsedCmd[1]))
			return;
		push_to_buf(ERR_NICKNAMEINUSE, *this, no_param);
		return;
	}
	(*_cmd_user).setNick(_parsedCmd[1]);
	if (_pass_before_nick_user == PASS_USER_OK)
		register_user(my_server);
	else
		_pass_before_nick_user = PASS_NICK_OK;
}

/*''''''''''''''''''''''''''''''''''''
				USER TODO check if last chars are spaces what happens
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_USER(Server &my_server)
{
	// std::cout << "User entered\n";
	(void)my_server; //
	if (_pass_before_nick_user == WAITING_FOR_PASS)
	{
		_pass_before_nick_user = PASS_ORDER_ERROR;
		return;
	}
	if (this->_parsedCmd.size() < 5 || _parsedCmd[4].empty() == true)
	{
		push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param);
		// send
		return ;
	}
	if (_cmd_user->getRegistered())
	{
		push_to_buf(ERR_ALREADYREGISTERED, *this, no_param);
		// sendto
		return;
	}
	else if (_parsedCmd[4].empty() == false && _parsedCmd[4][0] == ':')
	{
		std::vector<unsigned char> real_name = concat_real_name(_parsedCmd, 4);
		(*_cmd_user).setUserName(_parsedCmd[1]);
		(*_cmd_user).setRealName(real_name);
		if (_pass_before_nick_user == PASS_NICK_OK)
			register_user(my_server);
		else
			_pass_before_nick_user = PASS_USER_OK;
	}
}



/*''''''''''''''''''''''''''''''''''''
				JOIN
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_JOIN(Server &my_server)
{
	(void)my_server;
	std::vector<std::vector<unsigned char> > channels;
	std::vector<std::vector<unsigned char> > keys;
	// std::vector<std::vector<unsigned char> > keys;
	
	//RFC 2813/4.2.1
	/*Numeric Replies:
	ERR_NEEDMOREPARAMS		OK
	ERR_BANNEDFROMCHAN		
	ERR_INVITEONLYCHAN
	ERR_BADCHANNELKEY
	ERR_CHANNELISFULL
	ERR_BADCHANMASK			OK
	ERR_NOSUCHCHANNEL
	ERR_TOOMANYCHANNELS
	ERR_TOOMANYTARGETS
	ERR_UNAVAILRESOURCE
	RPL_TOPIC*/
	//RPL_TOPIC pour le new User et RPL_NAMREPLY Pour tout les users du chan (Nouvel utilisateur inclut)


	if (_parsedCmd.size() < 2)
	{
		push_to_buf(ERR_NEEDMOREPARAMS, *this, no_param);
		// sendto
		return ;
	}
	// print_vector2(_parsedCmd);
	reparseChannelsKeys(_parsedCmd[1], &channels);
	// print_vector2(channels);
	if (_parsedCmd.size() > 2)
		reparseChannelsKeys(_parsedCmd[2], &keys);
	// std::vector<std::vector<unsigned char> >::size_type keys_size = keys.size();
	// print_vector2(keys);
	for (std::vector<std::vector<unsigned char> >::size_type it = 0; it < channels.size(); ++it)
	{
		
		if (channels[it].empty() == false && channels[it][0] != '#' && channels[it][0] != '&')
		{
			push_to_buf(ERR_BADCHANMASK, *this, channels[it]);
			// sendto
		}
		else if (my_server.channelExists(channels[it]) == false)
		{
			std::cout << "User nb chans:" << _cmd_user->getNbChan() << std::endl;
			if (_cmd_user->getNbChan() >= MAX_NB_CHAN)
			{
				push_to_buf(ERR_TOOMANYCHANNELS, *this, channels[it]);
				//sendto
				return;
			}
			Channel new_channel(channels[it]);
			new_channel.addUser(*this, &(*_cmd_user), my_server);
			my_server.addNewChannel(new_channel);				
		}
		else if (my_server.channelExists(channels[it]) == true)
		{
			my_server.findChan(channels[it]).setMode('k', true);
			if (my_server.findChan(channels[it]).isUserInChannel(&(*_cmd_user)))
				return; // user is already in channel
			// check if key for channel
		}
		// if user is already in the channel return and do nothing

			// if (keys_size >= it + 1)
			// {
				// Channel new_channel(channels[it], keys[it]);
				// my_server.addNewChannel(new_channel);
			// }
			// else
			// {

			// + check if limit of chan is reached if mode l



	}



	// for (std::list<Channel>::iterator it = my_server.getChannelsbg(); it != my_server.getChannelsend(); ++it)

	
	// if (this->_parsedCmd.size() == 3)
	// {
	// 	passwd = splitOnComa(this->_parsedCmd[2]);
	// 	itpasswd = passwd.begin();
	// }
	// while (it != chan.end())
	// {
	// 	Channel	tmp = my_server.findChan(*it);
	// 	it++;
	// 	if (itpasswd != passwd.end())
	// 	{
	// 		tmp.addUser(this->_cmdUser, my_server, *itpasswd);
	// 		itpasswd++;
	// 	}
	// 	else
	// 		tmp.addUser(this->_cmdUser, my_server);
	// }
}


// void	Command::_fun_JOIN(Server &my_server)
// {
// 	//RFC 2813/4.2.1
// 	//TODO IF User == serverOp --> User = chanOp
// 	std::vector<unsigned char> ret;

// 	if (this->_parsedCmd.size() < 2)
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " :Not enough parameters\r\n");
// 		my_server.sendto(this->_cmdUser->getfd(), ret);
// 		return ;
// 	}
// 	std::vector<std::vector<unsigned char> >	chan = splitOnComa(this->_parsedCmd[1]);


// 	std::vector<std::vector<unsigned char> >::iterator	it = chan.begin();
// 	std::vector<std::vector<unsigned char> >			passwd;
// 	std::vector<std::vector<unsigned char> >::iterator	itpasswd;

// 	if (this->_parsedCmd.size() == 3)
// 	{
// 		passwd = splitOnComa(this->_parsedCmd[2]);
// 		itpasswd = passwd.begin();
// 	}
// 	while (it != chan.end())
// 	{
// 		Channel	tmp = my_server.findChan(*it);
// 		it++;
// 		if (itpasswd != passwd.end())
// 		{
// 			tmp.addUser(this->_cmdUser, my_server, *itpasswd);
// 			itpasswd++;
// 		}
// 		else
// 			tmp.addUser(this->_cmdUser, my_server);
// 	}
// }


















// void	Command::_fun_QUIT(Server &my_server)
// {
// 	std::vector<unsigned char> ret;
// 	unsigned long i = 1;
	
// 	if (this->_parsedCmd.size() > 1)
// 	{
// 		ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
// 		i++;
// 		while (i < _parsedCmd.size())
// 		{
// 			ret.push_back(' ');
// 			ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
// 			i++;
// 		}
// 	}
	
// 	for (std::vector<Channel>::iterator itc = _cmdUser.getChannels().begin(); itc != _cmdUser.getChannels().end(); itc++)
// 	{
// 		for (std::list<User>::iterator itu = itc->getUsers().begin(); itu != itc->getUsers().end(); itu++)
// 			my_server.send(itu->getfd(), ret);
// 		_cmdUser.getChannels().erase(itc);
// 	}
	
// 	my_server.getUser().remove(this->_cmdUser);
// 	close(_cmdUser.getfd());
	
// }


// void	Command::_fun_RESTART(Server &my_server)
// {
// 	(void)my_server;
// 	//fun free -> fun server.init() -> break le run -> fun server.run()
// }


// void	Command::_fun_PRIVMSG(Server &my_server)
// {
// 	//RFC2812
// 	/*Numeric Replies:

//            ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//            ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//            ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//            ERR_NOSUCHNICK
//            RPL_AWAY*/
// 	std::vector<unsigned char> ret;
// 	if (this->_parsedCmd.size() < 3)
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " :Not enough parameters\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// //PAS SUR DE CELLE CI
// 	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] == ':')
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " ERR_TOMANYTARGETS\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	std::vector<unsigned char>	receiver = this->_parsedCmd[1];
// 	std::vector<unsigned char>	msg;
// 	if (this->_parsedCmd[2][0] == ':')
// 		msg = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
// 	else
// 		msg = this->_parsedCmd[2];
// 	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
// 	while (it != this->_parsedCmd.end())
// 	{
// 		msg.push_back(' ');
// 		msg.insert(msg.end(), it->begin(), it->end());
// 	}

// 	std::list<User>::iterator	it_receiver = my_server.findUser(receiver);
// 	if (it_receiver == my_server.getUser().end())
// 	{
// 		ret = this->_parsedCmd[0];
// 		insert_all(ret, " ERR_NOSUCHNICK\r\n");
// 		my_server.send(this->_cmdUser.getfd(), ret);
// 		return ;
// 	}
// 	(void)my_server;
// }

void	Command::_fun_OPER(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_MODE(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_TOPIC(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_KICK(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_INVITE(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_KILL(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_PING(Server &my_server)
{
//	std::vector<User>::iterator	it = my_server.getUsers().begin();
	(void)my_server;
}

void	Command::_answer(Server &my_server)
{
	if (_pass_before_nick_user == PASS_ORDER_ERROR || _pass_before_nick_user == PASS_CONNECTION_ERROR)
		return;
	std::string	options[] = {"CAP", "NICK", "PASS", "USER", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING"};
	int i = 0;
	setCmdUser(my_server);

	
			
	while (i < 15 && my_compare(this->_parsedCmd[0], options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
			_fun_CAP(my_server);
			break;
		case 1:
			_fun_NICK(my_server);
			break;
		case 2:
			_fun_PASS(my_server);
			break;
		case 3:
			_fun_USER(my_server);
			break;
		case 4:
			_fun_JOIN(my_server);
			break;
		// case 4:
		// 	this->_fun_PRIVMSG(my_server);
		// 	break;
		// case 5:
		// 	this->_fun_OPER(my_server);
		// 	break;
		// case 6:
		// {
		// 	this->_fun_QUIT(my_server);
		// 	break;
		// }
		// case 7:
		// {
		// 	this->_fun_ERROR(my_server);
		// 	break;
		// }
		// case 8:
		// {
		// 	this->_fun_MODE(my_server);
		// 	break;
		// }
		// case 9:
		// {
		// 	this->_fun_TOPIC(my_server);
		// 	break;
		// }
		// case 10:
		// {
		// 	this->_fun_KICK(my_server);
		// 	break;
		// }
		// case 11:
		// {
		// 	this->_fun_INVITE(my_server);
		// 	break;
		// }
		// case 12:
		// {
		// 	this->_fun_KILL(my_server);
		// 	break;
		// }
		// case 13:
		// {
		// 	this->_fun_RESTART(my_server);
		// 	break;
		// }
		// case 14:
		// {
		// 	this->_fun_PING(my_server);
		// 	break;
		// }
		default:
			break;
	}
}

// Getters
unsigned char Command::getParsedCmdChar(std::vector<std::vector<unsigned char> >::size_type i, std::vector<std::vector<unsigned char> >::size_type j)
{
	return (_parsedCmd[i][j]);
}

std::vector<std::vector<unsigned char> > Command::getParsedCmd()
{
	return (_parsedCmd);
}

std::list<User>::iterator Command::getCmdUser()
{
	return (_cmd_user);
}

// Setters
void Command::setCmdFdUser(int fd)
{
	_cmd_fd_user = fd;	
}


/*''''''''''''''''''''''''''''''''''''
				CAP > to delete?
''''''''''''''''''''''''''''''''''''''*/
void	Command::_fun_CAP(Server &my_server)
{
	std::cout << "CAP COMMAND REALIZED : nothing to do here\n";
	(void)my_server;
}