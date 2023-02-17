/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/17 16:50:52 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>


#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/User.hpp"
#include "../inc/utils.hpp"

Command::Command(void): _cmdUser(), _parsedCmd()
{
}

Command::Command(const Command &copy): _cmdUser(copy._cmdUser), _parsedCmd(copy._parsedCmd)
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

// void Command::parseCommand(std::string input)
// {
// 	std::string commandline;
// 	std::string striterator;
// 	// std::vector<std::string> v;

// 	_parsedCmd.clear();
// 	striterator = input;
// 	while (striterator != "")
// 	{
// 		commandline = striterator.substr(0, striterator.find("\r\n"));
// 		striterator = striterator.substr(striterator.find("\r\n")+2, striterator.length());
// 		_parsedCmd.push_back(commandline);
// 		// while (commandline != "")
// 		// {
// 		// 	token = commandline.substr(0, commandline.find(" "));
// 		// 	v.push_back(token);
// 		// 	if (commandline.substr(commandline.find(" ") +1, commandline.length()).length() == commandline.length())
// 		// 		commandline = "";
// 		// 	else
// 		// 		commandline = commandline.substr(commandline.find(" ") +1, commandline.length());

// 		std::cout << "Parsed cmd:\n____________\n";
// 		for (int i = 0; i < (int)_parsedCmd.size(); i++)
// 			std::cout << "line " << i << _parsedCmd[i] << "\n";

// 		// }
// 	}
// }

void	Command::_fun_CAP(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_NICK(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_USER(Server &my_server)
{
	std::vector<unsigned char> ret;

	if (this->_parsedCmd.size() < 5)
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	if (this->_cmdUser.getRegistered())
	{
		insert_all(ret, ":You may not reregister\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	this->_cmdUser.setUserName(this->_parsedCmd[1]);
	//	this->_cmdUser.setMode(this->_parsedCmd[2]);Some weird thing to do : RFC 2812/3.1.3
	this->_cmdUser.setRealName(this->_parsedCmd[4]);
	this->_cmdUser.setRegistered(true);
}

void	Command::_fun_PASS(Server &my_server)
{
	std::vector<unsigned char> ret;

	if (this->_parsedCmd.size() < 2)
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	if (this->_cmdUser.getRegistered())
	{
		insert_all(ret, ":You may not reregister\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	this->_cmdUser.setPasswd(this->_parsedCmd[1]);
}


void	Command::_fun_JOIN(Server &my_server)
{
	//RFC 2813/4.2.1
	/*Numeric Replies:

	  ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
	  ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
	  ERR_CHANNELISFULL               ERR_BADCHANMASK
	  ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
	  ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
	  RPL_TOPIC*/
	//RPL_TOPIC pour le new User et RPL_NAMREPLY Pour tout les users du chan (Nouvel utilisateur inclut)

	std::vector<unsigned char> ret;

	if (this->_parsedCmd.size() < 2)
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	std::vector<std::vector<unsigned char> >	chan = splitOnComa(this->_parsedCmd[1]);


	std::vector<std::vector<unsigned char> >::iterator	it = chan.begin();
	std::vector<std::vector<unsigned char> >				passwd;
	std::vector<std::vector<unsigned char> >::iterator	itpasswd;

	if (this->_parsedCmd.size() == 3)
	{
		passwd = splitOnComa(this->_parsedCmd[2]);
		itpasswd = passwd.begin();
	}
	while (it != chan.end())
	{
		Channel	tmp = my_server.findChan(*it);
		it++;
		if (itpasswd != passwd.end())
		{
			tmp.addUser(this->_cmdUser, my_server, *itpasswd);
			itpasswd++;
		}
		else
			tmp.addUser(this->_cmdUser, my_server);
	}
}



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


void	Command::_fun_RESTART(Server &my_server)
{
	(void)my_server;
	//fun free -> fun server.init() -> break le run -> fun server.run()
}


void	Command::_fun_PRIVMSG(Server &my_server)
{
	//RFC2812
	/*Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY*/
	std::vector<unsigned char> ret;
	if (this->_parsedCmd.size() < 3)
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " :Not enough parameters\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
//PAS SUR DE CELLE CI
	if (this->_parsedCmd.size() > 3 && this->_parsedCmd[2][0] == ':')
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " ERR_TOMANYTARGETS\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	std::vector<unsigned char>	receiver = this->_parsedCmd[1];
	if (this->_parsedCmd[2][0] == ':')
		ret = std::vector<unsigned char>(this->_parsedCmd[2].begin() + 1, this->_parsedCmd[2].end());
	else
		ret = this->_parsedCmd[2];
	std::vector<std::vector<unsigned char> >::iterator	it = this->_parsedCmd.begin() + 3;
	while (it != this->_parsedCmd.end())
	{
		ret.push_back(' ');
		ret.insert(ret.end(), it->begin(), it->end());
	}

	std::list<User>::iterator	it_receiver = my_server.findUser(receiver);
	if (it_receiver == my_server.getUser().end())
	{
		ret = this->_parsedCmd[0];
		insert_all(ret, " ERR_NOSUCHNICK\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	(void)my_server;
}

void	Command::_fun_OPER(Server &my_server)
{
	std::vector<unsigned char> ret;
	
	if (_parsedCmd.size() == 3)
	{
		std::list<User>::iterator itu = my_server.findUser(_parsedCmd[1]);
		if (itu == my_server.getUser().end())
		{
			insert_all(ret, " ERR_PASSWMISMTCH\r\n");
			my_server.send(this->_cmdUser.getfd(),ret);
			return ;
		}
		else 
		{
			if (my_compare_vec(_parsedCmd[2], my_server.getPassword()) == 1)
			{
				insert_all(ret, " ERR_PASSWMISMTCH\r\n");
				my_server.send(this->_cmdUser.getfd(),ret);
				return ;
			}
		}
	}
	insert_all(ret, " ERR_PASSWMISMATCH\r\n");
	my_server.send(this->_cmdUser.getfd(), ret);
	return ;
}

void	Command::_fun_ERROR(Server &my_server)
{
	(void)my_server;
}

void	Command::_fun_MODE(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_INVITE(Server &my_server)
{
	std::vector<unsigned char>	ret;

	if (this->_parsedCmd.size() > 3)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	std::vector<Channel>::iterator	itc = my_server.findExistingChan(_parsedCmd[2]);
	if (itc == my_server.getChannels().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	std::list<User>::iterator	itu = itc->findUser(this->_cmdUser.getUserName());
	if (itu == itc->getUsrListend())
	{
		insert_all(ret, " ERR_NOTONCHANNEL\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	itu = itc->findUser(_parsedCmd[1]);
	if (itu != itc->getUsrListend())
	{
		insert_all(ret, " ERR_USERONCHANNEL\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	if (itc->getModes().find('i')->second && !itc->isOp(_cmdUser))
	{
		insert_all(ret, " ERR_CHANOPRIVSNEEDED\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	insert_all(ret, " RPL_INVITING\r\n");
	my_server.send(this->_cmdUser.getfd(), ret);
	return ;
}

void	Command::_fun_QUIT(Server &my_server)
{
	std::vector<unsigned char> ret;
	unsigned long i = 1;
	
	if (this->_parsedCmd.size() > 1)
	{
		ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
		i++;
		while (i < _parsedCmd.size())
		{
			ret.push_back(' ');
			ret.insert(ret.end(), _parsedCmd[i].begin(), _parsedCmd[i].end());
			i++;
		}
	}
	
	for (std::vector<Channel>::iterator itc = _cmdUser.getChannels().begin(); itc != _cmdUser.getChannels().end(); itc++)
	{
		for (std::list<User>::iterator itu = itc->getUsers().begin(); itu != itc->getUsers().end(); itu++)
			my_server.send(itu->getfd(), ret);
		_cmdUser.getChannels().erase(itc);
	}
	my_server.getUser().remove(this->_cmdUser);
	close(_cmdUser.getfd());
	
}


void	Command::_fun_TOPIC(Server &my_server)
{
	std::vector<unsigned char> ret;
	std::vector<Channel>::iterator		itc = my_server.findExistingChan(_parsedCmd[1]);
	std::list<User>::iterator			Usrlst = itc->getUsrListbg();

	ret = this->_parsedCmd[0];
	if (_parsedCmd.size() < 2)
	{
		insert_all(ret, " ERR_NEEDMOREPARAMS\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	if (itc == my_server.getChannels().end())
	{
		insert_all(ret, " ERR_NOSUCHCHANNEL\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	while (Usrlst != itc->getUsrListend() && *Usrlst != this->_cmdUser)
        Usrlst++;
    if (Usrlst == itc->getUsrListend())
    {
        ret = this->_cmdUser.getUserName();
        ret.insert(ret.end(), this->_parsedCmd[2].begin(), this->_parsedCmd[2].end());
        insert_all(ret, " ERR_NOTONCHANNEL\r\n");
        my_server.send(this->_cmdUser.getfd(), ret);
        return ;
    }
	
	if (itc->isOp(*Usrlst) == false)
	{
		insert_all(ret, " ERR_CHOPRIVSNEEDED\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	
	std::vector<unsigned char>	Topic = itc->getTopic();
	if (_parsedCmd.size() == 2)
	{
		if (Topic.size() > 0)
			insert_all(ret, " RPL_TOPIC\r\n");
		else
			insert_all(ret, " RPL_NOTOPIC\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		return ;
	}
	else if (_parsedCmd.size() == 3 && this->_parsedCmd[2].size() == 1 && this->_parsedCmd[2][0] == ':')
	{
		Topic.clear();
		itc->setTopic(Topic);
		insert_all(ret, " RPL_TOPIC\r\n");
		my_server.send(this->_cmdUser.getfd(), ret);
		for (std::list<User>::iterator itu = itc->getUsers().begin(); itu != itc->getUsers().end(); itu++)
			my_server.send(itu->getfd(), ret);
	}
	else
	{
		Topic.clear();
		std::vector<std::vector<unsigned char> >::iterator iterator = this->_parsedCmd.begin();
		if (*(iterator->begin()) == ':')
			Topic.insert(Topic.end(), iterator->begin() + 1, iterator->end());
		iterator++;
		Topic.push_back(' ');
		while (iterator != this->_parsedCmd.end())
		{
			iterator++;
			Topic.insert(Topic.end(), iterator->begin(), iterator->end());
			Topic.push_back(' ');
		}
		itc->setTopic(Topic);
		for (std::list<User>::iterator itu = itc->getUsers().begin(); itu != itc->getUsers().end(); itu++)
			my_server.send(itu->getfd(), ret);
	}
}

void	Command::_fun_KICK(Server &my_server)
{
	(void)my_server;
}

// void	Command::_fun_INVITE(Server &my_server)
// {
	
	


// 	(void)my_server;
// }

void	Command::_fun_KILL(Server &my_server)
{
	(void)my_server;
}


void	Command::_fun_PING(Server &my_server)
{
//	std::vector<User>::iterator	it = my_server.getUsers().begin();
	(void)my_server;
}

void	Command::_fun_NOTICE(Server &my_server)
{

	(void)my_server;
}

void	Command::_answer(Server &my_server)
{
	std::string	options[] = {"CAP", "USER", "PASS", "JOIN", "PRIVMSG", "OPER", "QUIT", "ERROR", "MODE", "TOPIC", "KICK", "INVITE", "KILL", "RESTART", "PING", "NOTICE"};
	int i = 0;
	while (i < 16 && my_compare(this->_parsedCmd[0], options[i]) != 0)
		i++;
	switch (i)
	{
		case 0:
		{
			this->_fun_CAP(my_server);
			break;
		}
		case 1:
		{
			this->_fun_USER(my_server);
			break;
		}
		case 2:
		{
			this->_fun_PASS(my_server);
			break;
		}
		case 3:
		{
			this->_fun_JOIN(my_server);
			break;
		}
		case 4:
		{
			this->_fun_PRIVMSG(my_server);
			break;
		}
		case 5:
		{
			this->_fun_OPER(my_server);
			break;
		}
		case 6:
		{
			this->_fun_QUIT(my_server);
			break;
		}
		case 7:
		{
			this->_fun_ERROR(my_server);
			break;
		}
		case 8:
		{
			this->_fun_MODE(my_server);
			break;
		}
		case 9:
		{
			this->_fun_TOPIC(my_server);
			break;
		}
		case 10:
		{
			this->_fun_KICK(my_server);
			break;
		}
		case 11:
		{
			this->_fun_INVITE(my_server);
			break;
		}
		case 12:
		{
			this->_fun_KILL(my_server);
			break;
		}
		case 13:
		{
			this->_fun_RESTART(my_server);
			break;
		}
		case 14:
		{
			this->_fun_PING(my_server);
			break;
		}
		case 15:
		{
			this->_fun_NOTICE(my_server);
			break;
		}
		default:
			break;
	}
}
