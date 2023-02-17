/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/17 17:05:19 by ghanquer         ###   ########.fr       */
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

void	check_kill(Server& server);

Server::Server(void): _argv(), _server(), _sct(), _passwd(), _epollfd(),  _ev(), _channels(), _Users()
{
	this->_events = new struct epoll_event[1000];
}

Server::Server(const Server & copy): _argv(copy._argv), _server(copy._server), _sct(copy._sct), _passwd(copy._passwd), _epollfd(copy._epollfd), _ev(copy._ev), _channels(copy._channels), _Users(copy._Users)
{
	this->_events = copy._events;
}

Server::~Server(void)
{
	close(this->_sct);
	close(this->_epollfd);
	delete [] (this->_events);
}

Server &	Server::operator=(const Server & src)
{
	if (&src == this)
		return (*this);
	_argv = src._argv;
	_server = src._server;
	_sct = src._sct;
	_passwd = src._passwd;
	_epollfd = src._epollfd;
	_ev = src._ev;
	_channels = src._channels;
	_Users = src._Users;
	return (*this);
}

std::vector<Channel>::iterator	Server::findExistingChan(std::vector<unsigned char> channel)
{
	std::vector<Channel>::iterator	it = this->_channels.begin();
	while (it != this->_channels.end() && it->getChanName() != channel)
		it++;
	return (it);
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
	_sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1); 
	bzero(&_server, sizeof(_server));

	add_to_vector(_passwd, argv[2]);

	_server.sin_addr.s_addr = INADDR_ANY;
	_server.sin_family = AF_INET;
	_server.sin_port = htons(atoi(argv[1]));
	if (_server.sin_port == 0)
		return (close(_sct), std::cerr << "Error on port" << std::endl, 1);

	if (bind(_sct, (sockaddr *)(&_server), sizeof(_server)))
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
	this->_argv = argv;

	return (0);
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

#include <errno.h>

void	Server::run(void)
{
	int accepted = 0;
	int yes = 1;//	For SO_KEEPALIVE
	long retrec;
	socklen_t server_length = sizeof(_server);
	_ev.events = EPOLLIN | EPOLLET;
	std::vector<unsigned char> v;

	while (true)
	{	
		unsigned char buf[BUFFER_SIZE] = "";
		int	wait_ret = epoll_wait(_epollfd, _events, 1000, 1);
		check_kill(*this);
		if (wait_ret == -1)
			return ;
		for (int k = 0; k < wait_ret; ++k)
		{
			if (_events[k].data.fd == _sct)
			{
				std::cout << "1 : socket accept and event data fd:" << _events[k].data.fd << std::endl;
				accepted = accept(_sct, (sockaddr *)(&_server), &server_length);
				if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
					return ;
				fcntl(accepted, F_SETFL, O_NONBLOCK);
				std::cout << "1.1 : accepted fd:" << accepted << std::endl;
				_ev.events = EPOLLIN | EPOLLET;
				_ev.data.fd = accepted;
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, accepted, &_ev) == - 1)
					return ;
			}
			else
			{
				try
				{
					std::list<User>::iterator Usr = this->getUsr(this->_events[k].data.fd);
					switch (this->_events[k].events)
					{
						case EPOLLOUT:
							if (Usr == this->_Users.end())
							{
								_ev.events = EPOLLIN | EPOLLET;
								if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &this->_ev) == - 1)
								{
									break;
								}
							}
							this->sendto(Usr->getfd(), Usr->getRet());
							break;
						case EPOLLIN:
							if (isUserInList(_events[k].data.fd) == false)
							{
								User new_user(this->_events[k].data.fd);
								if (this->_Users.size() == 0)
									new_user.setOperator(true);
								this->_Users.push_back(new_user);
							}
							Usr = this->getUsr(this->_events[k].data.fd);
							retrec = recv(Usr->getfd(), buf, BUFFER_SIZE, 0);
							if (retrec <= 0)
							{
							}
							else if (retrec == BUFFER_SIZE)
							{
								for (int i = 0; i < BUFFER_SIZE; i++)
									v.push_back(buf[i]);
								Usr->insertCurrCmd(v);
							}
							else
							{
								Command	cmd;
								std::vector<std::vector<unsigned char> >	ParsedCommand;
								for (int i = 0; i < retrec; i++)
									v.push_back(buf[i]);
								Usr->insertCurrCmd(v);
								std::vector<unsigned char>::iterator last = Usr->getCurrCmdbg();
								for (std::vector<unsigned char>::iterator it = Usr->getCurrCmdbg(); it != Usr->getCurrCmdend(); it++)
								{
									if (it != Usr->getCurrCmdbg() && *(it - 1) == '\r' && *it == '\n')
									{
										std::vector<unsigned char>	CmdNoParse(last, it);
										for (std::vector<unsigned char>::iterator j = last; j != it; j++)
										{
											if (*j == ' ' || j == it - 2)
												cmd.getCommand().insert(cmd.getCommand().end(), std::vector<unsigned char>(j, it));
										}
										cmd.answer(*this);
										last = it + 1;
										_ev.events = EPOLLOUT | EPOLLET;
										if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &this->_ev) == - 1)
											break;
										cmd.setUser(&(*Usr));
									}
								}
								break;
							}
						default:
							epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
							close(_events[k].data.fd);
							if (Usr != this->_Users.end())
								this->_Users.erase(Usr);
							break;
					}
				}
				catch (std::exception &)
				{
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
					close(_events[k].data.fd);
					if (this->getUsr(this->_events[k].data.fd) != this->_Users.end())
						this->_Users.erase(this->getUsr(this->_events[k].data.fd));
				}
			}
		}
	}
}

void	Server::sendto(int fd, std::vector<unsigned char> buf)
{
	long int ret;
	ret = send(fd, reinterpret_cast<char *>(buf.data()), buf.size(), MSG_NOSIGNAL);
	if (ret < 0)
	{
		std::list<User>::iterator Usr = this->getUsr(fd);
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
			it->delUser(fd);
		epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &this->_ev);
		close(fd);
	}
	_ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_MOD, fd, &(this->_ev)) == - 1)
	{
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
			it->delUser(fd);
		epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &this->_ev);
		close(fd);
		return ;
	}
}

std::list<User>::iterator	Server::findUser(std::vector<unsigned char> nick)
{
	std::list<User>::iterator it;
	for (it = this->_Users.begin(); it != this->_Users.end(); it++)
	{
		if (it->getUserName() == nick)
			return (it);
	}
	return (it);
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

// GETTERS

std::list<User>::iterator	Server::getUsr(int fd)
{
	for (std::list<User>::iterator it = this->_Users.begin(); it != this->_Users.end(); it++)
	{
		if (it->getfd() == fd)
			return (it);
	}
	return (this->_Users.end());
}

std::vector<Channel>	Server::getChannel(void) const
{
	return (this->_channels);
}

char ** Server::getArgv(void) const
{
	return (this->_argv);
}

int Server::getSct(void) const
{
	return (_sct);
}

int Server::getEpollfd(void) const
{
	return (_epollfd);
}

epoll_event &	Server::getEv(void)
{
	return (this->_ev);
}

std::list<User> Server::getUsers(void) const
{
	return (this->_Users);
}
