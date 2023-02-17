/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/17 13:21:32 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"
#include "../inc/utils.hpp"

#include <string>
 #include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include "../inc/utils.hpp"

Server::Server(void): _Users(),_server(), _sct(), _passwd(),_epollfd(), _ev(), _channels()
{
}

Server::Server(const Server & copy):_Users(copy._Users), _server(copy._server), _sct(copy._sct), _passwd(copy._passwd), \
_epollfd(copy._epollfd), _ev(copy._ev),_channels(copy._channels)
{
	// _passwd = copy._passwd;
}

Server::~Server(void)
{
}

Server &	Server::operator=(const Server & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

Channel &	Server::findChan(std::vector<unsigned char> channel)
{
	std::vector<Channel>::iterator	it = this->_channels.begin();
	while (it != this->_channels.end() && it->getChanName() != channel)
		it++;
	if (it == this->_channels.end())
		it = this->_channels.insert(this->_channels.end(), Channel(channel));
	return (*it);
}

int	Server::init(char **argv)
{
	// TO DO: always protect close functions
	_sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM|SOCK_NONBLOCK
	if (_sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1); 
	bzero(&_server, sizeof(_server));
	
	add_to_vector(_passwd, argv[2]);

	// print_vector(_passwd);

	_server.sin_addr.s_addr = INADDR_ANY;
	_server.sin_family = AF_INET;
	_server.sin_port = htons((short unsigned int)atoi(argv[1])); // check if negative? or any other char
	if (_server.sin_port == 0) // be able to connect directly on port 6667 if not specified 
		return (close(_sct), std::cerr << "Error on port" << std::endl, 1);

	if (bind(_sct, (sockaddr *)(&_server), sizeof(_server)))//server_info->ai_addrlen))
		return (close(_sct), std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(_sct, 1) == -1)
		return (close(_sct), std::cerr << "Error listening socket" << std::endl, 1);

	_epollfd = epoll_create1(0);

	if (_epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	_ev.events = EPOLLIN;
	_ev.data.fd = _sct;

	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sct, &_ev) == -1)
		return (close(_sct), close(_epollfd), std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	
	std::cout << "0 : epoll fd: " << _epollfd << std::endl;
	
	return (0);
}

int Server::accept_socket(int k)
{
	int yes = 1;//	For SO_KEEPALIVE
	int accepted = 0;
	socklen_t server_length = sizeof(_server);
	
	std::cout << "1 : socket accept and event data fd:" << _events[k].data.fd << std::endl;
	accepted = accept(_sct, (sockaddr *)(&_server), &server_length);
	if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)//Keepalive permet de garder la connexion apres utilisation
		return (std::cerr << "Error on accept" << std::endl, 1);
	fcntl(accepted, F_SETFL, O_NONBLOCK);
	std::cout << "1.1 : accepted fd:" << accepted << std::endl;
	_ev.events = EPOLLIN | EPOLLET;
	_ev.data.fd = accepted;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, accepted, &_ev) == - 1)
		return (std::cerr << "Error on epoll_ctl_add accepted sock" << std::endl, 1);
	return (0);
}

void Server::printGlobalCommand(Command cmd)
{
	std::vector<std::vector<unsigned char> >::size_type i, j;
	
	std::cout << "--Print global command--\n";
	for (i = 0; i < cmd._globalCmd.size(); i++) 
	{
		std::cout << "> size:" << cmd._globalCmd[i].size() << "  ";
		for (j = 0; j < cmd._globalCmd[i].size(); j++)
			std::cout << cmd._globalCmd[i][j];
	}
	std::cout << "\n";
}

void Server::printParsedCommand(Command cmd)
{
	std::vector<std::vector<unsigned char> >::size_type i, j;
	
	std::cout << "--Print parsed command--\n";
	for (i = 0; i < cmd._parsedCmd.size(); i++) 
	{
		std::cout << ">";
		for (j = 0; j < cmd._parsedCmd[i].size(); j++)
			std::cout << cmd._parsedCmd[i][j];
	}
	std::cout << "\n";
}

void Server::getGobalCmd(Command* cmd, std::vector<unsigned char> v, int k)
{
	int	firstRun = 1;
	int isLastCr = 0;
	
	cmd->setCmdFdUser(_events[k].data.fd);
	cmd->_globalCmd.clear();
	v.clear();
	if (_events[k].events & EPOLLHUP)
	{
		close(_events[k].data.fd);
		epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_events[k]);
	}
	unsigned char buf[BUFFER_SIZE] = "";
	while (recv(_events[k].data.fd, buf, BUFFER_SIZE, 0) > 0) // add flags? MSG_DONTWAIT
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			if (!firstRun && i == 0 && isLastCr && buf[i] == '\n')
			{
				v.push_back(buf[i]);
				cmd->_globalCmd.push_back(v);
				v.clear();
			}
			else if (i > 0 && isLastCr && buf[i] == '\n')
			{
				v.push_back(buf[i]);
				cmd->_globalCmd.push_back(v);
				v.clear();
			}
			else
				v.push_back(buf[i]);
			if (buf[i] == '\r')
				isLastCr = 1;
			else
				isLastCr = 0;
			
			
		}
		bzero(buf, BUFFER_SIZE);
		firstRun = 0;
	}	
}



int	Server::run(void)
{
	// TO DO: always protect close functions
	int k;
	std::vector<std::vector<unsigned char> >::size_type i;
	Command cmd;
	std::vector<unsigned char> v;

	while (true)
	{	
		//CHECK CTRL + C
		int	wait_ret = epoll_wait(_epollfd, _events, 1, -1);
		if (wait_ret == -1)
			return (std::cerr << "Error on epoll wait" << std::endl, 1);
		for (k = 0; k < wait_ret; ++k)
		{
			if (_events[k].data.fd == _sct)
				this->accept_socket(k);
			else
			{
				if (isUserInList(_events[k].data.fd) == false) // ADD User to list
				{
					User new_user(_events[k].data.fd);
					new_user.setRegistered(true); // <<<<< delete
					_Users.push_back(new_user);
				}
				//////////////////////////
				printUsersList();
				/////////////////////////
				
				this->getGobalCmd(&cmd, v, k);				// Get global command
				this->printGlobalCommand(cmd);				// Print global command
				// Parse command and execute
				for (i = 0; i < cmd._globalCmd.size(); i++)
				{
					this->getParsedCmd(&cmd, v, i);
					
					// Print executed commands///////
					std::cout << "______X-e-cute command______\n";
					std::vector<std::vector<unsigned char> >::size_type m, n;
					for (m = 0; m < cmd.getParsedCmd().size(); m++)
					{
						for (n = 0; n < cmd.getParsedCmd()[m].size(); n++)
							std::cout << cmd.getParsedCmdChar(m, n);
						std::cout << " ";
					}
					std::cout << std::endl;
					//////////////////////////
					cmd._answer(*this);
					
					cmd._parsedCmd.clear();
					// 	::send(_events[k].data.fd, ":irc.la_team.com 001 iguscett: Welcome to La Team's Network, iguscett\r\n", strlen(":irc.la_team.com 001 iguscett: Welcome to La Team's Network, iguscett\r\n"), 0);
					
					// Parse scommand
					
				}
				cmd._globalCmd.clear();

			}
		}
	}

	return (1);
}

bool Server::isUserInList(int fd)
{
	for (std::list<User>::iterator it = _Users.begin(); it != _Users.end(); ++it)
	{
		if (fd == it->getfd())
			return (true);
	}
	return (false);
}

void Server::send_to_client(int fd, std::vector<unsigned char> buf)
{
	long int ret;
	
	ret = send(fd, reinterpret_cast<char *>(buf.data()), buf.size(), MSG_NOSIGNAL);
	(void)ret;
}

std::list<User>::iterator	Server::findUser(int fd)
{
	std::list<User>::iterator it;
	for (it = _Users.begin(); it != _Users.end(); ++it)
	{
		if (it->getfd() == fd)
			return (it);
	}	
	return (it); // _Users.end()?
}

std::list<User>::iterator Server::findUserNick(std::vector<unsigned char> nick)
{
	int i = 0;
	std::list<User>::iterator itu;

	for (itu = _Users.begin(); itu != _Users.end(); ++itu)
	{
		// std::cout << i << std::endl;
		if (!my_compare(itu->getNick(), nick))
			return (itu);
		i++;
	}
	// std::cout << i << std::endl;
	// if (itu == _Users.end())
	// 	std::cout << "OK == end()\n";
	return (itu);
}

void Server::printUsersList(void)
{
	std::cout << "____PRINT USERS____\n";
	for (std::list<User>::iterator it = _Users.begin(); it != _Users.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << "__END PRINT USERS__\n";
}

// GETTERS

void Server::getParsedCmd(Command* cmd, std::vector<unsigned char> v, std::vector<std::vector<unsigned char> >::size_type i)
{
	std::vector<std::vector<unsigned char> >::size_type j;
	
	v.clear();
	cmd->_parsedCmd.clear();
	for (j = 0; j < cmd->_globalCmd[i].size(); j++)
	{			
		if (cmd->_globalCmd[i][j] == ' ' || j == cmd->_globalCmd[i].size() - 2)
		{
			cmd->_parsedCmd.push_back(v);
			v.clear();
		}
		else if (cmd->_globalCmd[i][j] != ' ')
			v.push_back(cmd->_globalCmd[i][j]);
	}
}

// void Server::getParsedCmd(void)
// {
// 	std::vector<std::vector<unsigned char> >::size_type j;
	
// 	return (_parsedCmd)
// }

int Server::getSct(void)
{
	return (_sct);
}

int Server::getEpollfd(void)
{
	return (_epollfd);
}

const std::list<User>& Server::getUsers(void) const
{
	return (this->_Users);
}

std::vector<unsigned char> Server::getPasswd(void) const
{
	return (_passwd);
}