/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/09 08:41:25 by iguscett         ###   ########.fr       */
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
#include <errno.h>
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

int	check_kill(Server& server);

Server::Server(void): _argv(), _server(), _sct(), _passwd(), _epollfd(),  _ev(), _channels(), _users()
{
	_events = new struct epoll_event[1000];
}

Server::Server(const Server & copy): _argv(copy._argv), _server(copy._server), _sct(copy._sct), _passwd(copy._passwd), _epollfd(copy._epollfd), _ev(copy._ev), _channels(copy._channels), _users(copy._users)
{
	_events = copy._events;
}

Server::~Server(void)
{
	close(_sct);
	close(_epollfd);
	delete [] (_events);
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
	_users = src._users;
	return (*this);
}

std::vector<unsigned char>	Server::getPassword(void) const
{
	return (_passwd);
}

std::vector<Channel>::iterator    Server::findExistingChan(std::vector<unsigned char> channel)
{
	std::vector<Channel>::iterator    it = _channels.begin();
	while (it != _channels.end() && it->getChanName() != channel)
		it++;
	return (it);
}

Channel*	Server::findChan(std::vector<unsigned char> channel)
{
	std::vector<Channel>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->getChanName() == channel)
			return (&(*it));
	}
	return (NULL);
}

void	Server::setBot(void)
{
	this->_bot = User();
	Channel	botchan = Channel();

	std::vector<unsigned char>	botvec = to_vector("#bot");

	this->_bot.setNick(botvec);
	this->_bot.setRegistered(true);
	this->_bot.setOperator(true);
	this->_bot.setRealName(botvec);
	this->_bot.setUserName(botvec);
	botchan.setChanName(botvec);
	this->_channels.push_back(botchan);
	this->_bot.addChannel(this->findChan(botvec));
}

int	Server::init(char **argv)
{
	_sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sct == -1)
		return (std::cerr << "Invalid socket" << std::endl, 1); 
	bzero(&_server, sizeof(_server));

	this->_users = std::list<User>();
	this->_channels = std::vector<Channel>();
	this->_passwd = to_vector(argv[2]);

	_server.sin_addr.s_addr = INADDR_ANY;
	_server.sin_family = AF_INET;
	_server.sin_port = htons(atoi(argv[1]));
	if (_server.sin_port == 0)
		return (close(_sct), std::cerr << "Error on port" << std::endl, 1);
	int	oof = 1;
	setsockopt(this->_sct, SOL_SOCKET, SO_REUSEADDR, &oof, sizeof(int));
	if (bind(_sct, (sockaddr *)(&_server), sizeof(_server)))
		return (close(_sct), std::cerr << "Error connecting socket" << std::endl, 1);
	if (listen(_sct, 1000) == -1)
		return (close(_sct), std::cerr << "Error listening socket" << std::endl, 1);
	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	_ev.events = EPOLLIN | EPOLLET;
	_ev.data.fd = _sct;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sct, &_ev) == -1)
		return (close(_sct), close(_epollfd), std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	_argv = argv;
	return (0);
}

bool Server::isUserInList(int fd)
{
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (fd == it->getfd())
			return (true);
	}
	return (false);
}

void Server::addNewChannel(std::vector<unsigned char> new_channel)
{
	Channel	chan(new_channel);
	_channels.push_back(chan);
}

int Server::run(void)
{
	int accepted = 0;
	int yes = 1;
	long retrec;
	socklen_t server_length = sizeof(_server);
	_ev.events = EPOLLIN | EPOLLET;
	std::vector<unsigned char> v;
	std::vector<unsigned char>	CmdIt;
	std::vector<unsigned char> iv;
	std::vector<unsigned char>::iterator iter;
	Command	cmd;

	this->setBot();

	while (true)
	{
		try
		{
			unsigned char buf[BUFFER_SIZE] = "";
			int	wait_ret = epoll_wait(_epollfd, _events, 1000, 1);
			std::list<User>::iterator Usr;
			int epoll_int;
			
			if (check_kill(*this) == 1)
				return (0);
			if (wait_ret == -1)
				return (0);
			for (int k = 0; k < wait_ret; ++k)
			{
				if (_events[k].data.fd == _sct)
				{
					accepted = accept(_sct, (sockaddr *)(&_server), &server_length);
					if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
						return (0);
					fcntl(accepted, F_SETFL, O_NONBLOCK);
					_ev.events = EPOLLIN | EPOLLET;
					_ev.data.fd = accepted;
					if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, accepted, &_ev) == - 1)
						return (0);
					if (isUserInList(accepted) == false)
					{
						User new_user(accepted);
						if (_users.size() == 0)
						{
							new_user.setOperator(true);
							new_user.setMode('o', true);
						}
						_users.push_back(new_user);
					}
				}
				else
				{
					try
					{
						std::cerr << "Accepted fd:" << _events[k].data.fd << std::endl;
						Usr = findUser(_events[k].data.fd);
						std::vector<std::vector<unsigned char> > ParsedCommand;
						switch (_events[k].events)
						{
							case EPOLLOUT:
								std::cerr << "Sending data" << std::endl;
								printUsersList();
								std::cout << *this;
								epoll_int = 0;
								// Usr->clearCurrCmd();
								sendto(Usr->getfd(), Usr->getRet());
								Usr = findUser(_events[k].data.fd);
								if (Usr == _users.end() || Usr->getFatalError() == 1)
								{
									_ev.events = EPOLLIN | EPOLLET;
									_ev.data.fd = Usr->getfd();
									if ((epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1) || Usr->getFatalError() == 1)
									{
										close(_events[k].data.fd);
										if (Usr != _users.end())
											_users.erase(Usr);
										if (Usr->getFatalError() == 1)
										{
											delUser(&(*Usr));
											_users.erase(Usr);
										}
										break;
									}
								}
								ParsedCommand.clear();
								Usr->clearRet();
								cmd._globalCmd.clear();
								CmdIt.clear();
								if (Usr->getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
								{
									epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
									close(_events[k].data.fd);
									if (Usr != _users.end())
										_users.erase(Usr);
									break;
								}
								print_vector("222>>>>>>>>>>>>>>>>>>>currCmd", Usr->getCurrCmd());
								if (Usr->getCurrCmd().size() > 0)
								{
									for (std::vector<unsigned char>::size_type it = 0; it < Usr->getCurrCmd().size(); it++)
									{
										cmd._globalCmd.push_back(Usr->getCurrCmd()[it]);
										if (it > 0 && Usr->getCurrCmd()[it -1] == '\r' && Usr->getCurrCmd()[it] == '\n')
										{
											iv.clear();
											for (std::vector<unsigned char>::size_type j = 0; j < cmd._globalCmd.size(); j++)
											{
												if (j == 0 && cmd._globalCmd[j] == ' ')
												{
													while (j < cmd._globalCmd.size() - 2 && cmd._globalCmd[j] == ' ')
														iv.push_back(cmd._globalCmd[j++]);
													while (j < cmd._globalCmd.size() - 2 && cmd._globalCmd[j] != ' ')
														iv.push_back(cmd._globalCmd[j++]);
												}
												else
												{
													while (j < cmd._globalCmd.size() - 2 && cmd._globalCmd[j] == ' ')
														j++;
													while (j < cmd._globalCmd.size() - 2 && cmd._globalCmd[j] != ' ')
														iv.push_back(cmd._globalCmd[j++]);
												}
												if ((j == cmd._globalCmd.size() - 2) || cmd._globalCmd[j] == ' ')
												{
													ParsedCommand.push_back(iv);
													iv.clear();
												}
											}
											cmd.setParsedCmd(ParsedCommand);
											cmd.setUser(&(*Usr));
											int ans = cmd.answer(*this);
											if (ans == 3)
												Usr->setFatalError(1);
											if (ans == 2)
											{
												// Usr->initialize();
												// close(_events[k].data.fd);
												delUser(&(*Usr));
												_users.erase(Usr);
												break;
											}
											if (ans == 4)
												return (1);
											Usr->getCurrCmd().erase(Usr->getCurrCmdbg(), (Usr->getCurrCmdbg() + it + 1));
											if (ans != 0)
											{
												epoll_int = 1;
												if (epoll_int == 1)
													break;
											}
										}
									}
									if (epoll_int == 1)
									{
										_ev.events = EPOLLOUT | EPOLLET;
										_ev.data.fd = Usr->getfd();
										if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1)
										{
											_ev.data.fd = Usr->getfd();
											epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
											close(_events[k].data.fd);
											if (Usr != _users.end())
												_users.erase(Usr);
										}
										break;
									}
									_ev.events = EPOLLIN | EPOLLET;
									_ev.data.fd = Usr->getfd();
									if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1)
									{
										epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
										close(_events[k].data.fd);
										if (Usr != _users.end())
											_users.erase(Usr);
									}
									break;
								}
								_ev.events = EPOLLIN | EPOLLET;
								_ev.data.fd = Usr->getfd();
								Usr = findUser(_events[k].data.fd);
								if (Usr == _users.end())
								{
									if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1)
									{
										close(_events[k].data.fd);
										if (Usr != _users.end())
											_users.erase(Usr);
										break;
									}
								}
								if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &(_ev)) == - 1)
								{
									_ev.data.fd = Usr->getfd();
									epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
									close(_events[k].data.fd);
									_users.erase(Usr);
								}
								Usr->clearCurrCmd();
								break;							
							case EPOLLIN:
								std::cerr << "Receiving data" << std::endl;
								printUsersList();
								std::cout << *this;
								if (isUserInList(_events[k].data.fd) == false)
								{
									User new_user(_events[k].data.fd);
									if (_users.size() == 0)
										new_user.setOperator(true);
									_users.push_back(new_user);
								}
								Usr = findUser(_events[k].data.fd);
								retrec = recv(Usr->getfd(), buf, BUFFER_SIZE, MSG_DONTWAIT);
								std::cout << "BUFFFFFFFFFFFFFFFFFFFFF:" << buf << std::endl;
								if (retrec <= 0)
								{
									epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
									close(_events[k].data.fd);
									if (Usr != _users.end())
										_users.erase(Usr);
									break ;
								}
								else if (retrec == BUFFER_SIZE)
								{
									for (int i = 0; i < BUFFER_SIZE; i++)
										v.push_back(buf[i]);
									Usr->insertcmd(v);
									v.clear();
								}
								else if (retrec < BUFFER_SIZE)
								{
									for (int i = 0; i < retrec; i++)
										v.push_back(buf[i]);
									Usr->insertcmd(v);
									v.clear();
									ParsedCommand.clear();
									Usr->clearRet();
									cmd._globalCmd.clear();
									CmdIt.clear();
									iter = Usr->getCurrCmdbg();
									print_vector(">>>>>>>>>>>>>>>>>>>currCmd", Usr->getCurrCmd());
									for (std::vector<unsigned char>::size_type it = 0; it < Usr->getCurrCmd().size(); it++)
									{
										cmd._globalCmd.push_back(Usr->getCurrCmd()[it]);
										iter++;
										if (it > 0 && Usr->getCurrCmd()[it -1] == '\r' && Usr->getCurrCmd()[it] == '\n')
										{
											iv.clear();
											for (std::vector<unsigned char>::size_type j = 0; j < cmd._globalCmd.size(); j++)
											{
												if (j == 0 && cmd._globalCmd[j] == ' ')
												{
													while (j < cmd._globalCmd.size() -2 && cmd._globalCmd[j] == ' ')
														iv.push_back(cmd._globalCmd[j++]);
													while (j < cmd._globalCmd.size() -2 && cmd._globalCmd[j] != ' ')
														iv.push_back(cmd._globalCmd[j++]);
												}
												else
												{
													while (j < cmd._globalCmd.size() -2 && cmd._globalCmd[j] == ' ')
														j++;
													while (j < cmd._globalCmd.size() -2 && cmd._globalCmd[j] != ' ')
														iv.push_back(cmd._globalCmd[j++]);
												}
												if ((j == cmd._globalCmd.size() -2) || cmd._globalCmd[j] == ' ')
												{
													ParsedCommand.push_back(iv);
													iv.clear();
												}
											}
											cmd.setParsedCmd(ParsedCommand);
											cmd.setUser(&(*Usr));
											int ans = cmd.answer(*this);
											if (ans == 3)
												Usr->setFatalError(1);
											if (ans == 2)
											{
												// Usr->initialize();
												// close(_events[k].data.fd);
												delUser(&(*Usr));
												_users.erase(Usr);
												break;
											}
											if (ans == 4)
												return (1);
											if (ans == 1 || ans == 3)
											{
												Usr->getCurrCmd().erase(Usr->getCurrCmdbg(), iter);
												_ev.events = EPOLLOUT | EPOLLET;
												_ev.data.fd = Usr->getfd();
												if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1)
												{
													_ev.data.fd = Usr->getfd();
													epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
													close(_events[k].data.fd);
													if (Usr != _users.end())
														_users.erase(Usr);
												}
												break;
											}
											cmd._globalCmd.clear();
											cmd.getParsedCmd().clear();
											ParsedCommand.clear();
										}
									}
									
								}
								else if (retrec == 0 || Usr->getPassBeforeNickUser() == PASS_CONNECTION_ERROR)
								{
									_ev.data.fd = _events[k].data.fd;
									epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
									close(_events[k].data.fd);
									if (Usr != _users.end())
										_users.erase(Usr);	
								}
								break;
						}
					}
					catch (std::exception & e)
					{
						_ev.data.fd = _events[k].data.fd;
						epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
						if (findUser(_events[k].data.fd) != _users.end())
							_users.erase(findUser(_events[k].data.fd));
						close(_events[k].data.fd);
					}
				}
			}
		}
		catch (std::exception & e)
		{
		}
	}
	return (0);
}

void Server::eraseUsr(std::list<User>::iterator Usr)
{
	_users.erase(Usr);
}

void Server::eraseUsr(std::vector<unsigned char> nick)
{
	std::list<User>::iterator Usr = _users.begin();
	
	while (Usr != _users.end() && Usr->getNick() != nick)
		Usr++;
	if (Usr != _users.end())
		_users.erase(Usr);
}

void Server::delUser(User *Usr)
{
	std::vector<Channel>::iterator chan = getChannelsbg();
	for (; chan != getChannelsend(); chan++)
	{
		chan->delUserUsrLst(Usr);
		chan->delUserBanLst(Usr);
		chan->delOpLst(Usr);
	}
	std::vector<Channel>::iterator ite = _channels.begin();
	while (ite != _channels.end())
	{
		if (ite->getUserList().size() == 0)
		{
			_channels.erase(ite);
			ite = _channels.begin();
		}
		else
			ite++;
	}
	_ev.data.fd = Usr->getfd();
	epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
	close(Usr->getfd());
}


void Server::sendto(int fd, std::vector<unsigned char> buf)
{
	long int ret;

	ret = send(fd, reinterpret_cast<char *>(buf.data()), buf.size(), MSG_NOSIGNAL);
	buf.push_back('\0');
	std::cerr << "Sending : " << buf.data() << "To fd : " << fd << std::endl;
	if (ret < 0)
	{
		std::list<User>::iterator Usr = findUser(fd);
		_ev.data.fd = fd;
		epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
		_users.erase(Usr);
		close(fd);
	}
}

void Server::addNewChannel(Channel& new_channel)
{
	_channels.push_back(new_channel);
}

std::list<User>::iterator Server::findUser(std::vector<unsigned char> nick)
{
	std::list<User>::iterator it;

	for (it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getNick() == nick)
			return (it);
	}
	return (it);
}

std::list<User>::iterator Server::findUser(int fd)
{
	std::list<User>::iterator it;
	for (it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getfd() == fd)
			return (it);
	}	
	return (it);
}

User* Server::findUserPtrNick(std::vector<unsigned char> nick)
{
	int i = 0;
	std::list<User>::iterator itu;

	for (itu = _users.begin(); itu != _users.end(); ++itu)
	{
		if (itu->getNick() == nick)
			return (&(*itu));
		i++;
	}
	return (NULL);
}

void	Server::delChan(std::vector<Channel>::iterator itc)
{
	this->_channels.erase(itc);
}







void Server::printUsersList(void)
{
	std::cout << "____PRINT USERS____\n";
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << "__END PRINT USERS__\n";
}

std::vector<Channel> &	Server::getChannelref(void)
{
	return (_channels);
}

std::vector<Channel>	Server::getChannel(void) const
{
	return (_channels);
}

char ** Server::getArgv(void) const
{
	return (_argv);
}

int Server::getSct(void) const
{
	return (_sct);
}

int Server::getEpollfd(void) const
{
	return (_epollfd);
}

std::list<User> Server::getUsers(void) const
{
	return (_users);
}

std::vector<unsigned char> Server::getPasswd(void) const
{
	return (_passwd);
}

std::vector<Channel>::iterator Server::getChannelsbg(void)
{
	return (_channels.begin());
}

std::vector<Channel>::iterator Server::getChannelsend(void)
{
	return (_channels.end());
}

std::list<User>::iterator	Server::getUsersbg(void)
{
	return (_users.begin());
}
std::list<User>::iterator	Server::getUsersend(void)
{
	return (_users.end());
}

bool Server::channelExists(std::vector<unsigned char>& channel_name)
{
	std::vector<Channel>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->getChanName() == channel_name)
			return (true);
	}
	return (false);
}

int Server::nbConnections(User &user)
{
	int nb = 0;

	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getUserMask() == user.getUserMask())
			nb++;
	}
	return (nb);
}

int Server::nbConnectionsWithSameNick(User &user)
{
	int nb = 0;

	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getUserMask() == user.getUserMask() && it->getNick() == user.getNick())
			nb++;
	}
	return (nb);
}

int Server::nbConnectionsWithSameMask(User &user)
{
	int nb = 0;

	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getUserMask() == user.getUserMask())
			nb++;
	}
	return (nb);
}

std::ostream & operator<<( std::ostream & o, Server & i)
{
	std::vector<Channel>::iterator m;
	std::vector<unsigned char>::size_type n;
	std::list<User*>::iterator itu;

	o << "____Channels begin____\n";
	for (m = i.getChannelsbg(); m != i.getChannelsend(); m++)
	{
		o << "Chan:";
		for (n = 0; n < m->getChanName().size(); n++)
			o << m->getChanName()[n];
		o << " Key:";
		for (n = 0; n < m->getChanPassword().size(); n++)
			o << m->getChanPassword()[n];

		o << " modes:";
		std::map<char, bool>::iterator itm;
		for (itm = m->getModesbg(); itm != m->getModesend(); ++itm)
		{
			if (itm->second == true)
				o << itm->first;
		}
		o << "\n--------USERS--------\n";
		for (itu = m->getUserListbg(); itu != m->getUserListend(); ++itu)
		{
			o << (*(*itu));
		}
		o << "\n--------USERS OP--------\n";
		for (itu = m->getUserListOpbg(); itu != m->getUserListOpend(); ++itu)
		{
			if (*itu != NULL)
				o << (*(*itu));
		}
		o << "\n--------USERS BANNED--------\n";
		for (itu = m->getUserListBanbg(); itu != m->getUserListBanend(); ++itu)
		{
			if (*itu != NULL)
				o << (*(*itu));
		}
		o << "\n--------USERS INVITED--------\n";
		for (itu = m->getUserListInvitebg(); itu != m->getUserListInviteend(); ++itu)
		{
			if (*itu != NULL)
				o << (*(*itu));
		}
		o << std::endl;
	}
	o << "____Channels end____\n";
	
	return o;
}
epoll_event &	Server::getEv(void)
{
	return (_ev);
}


//---------------------------BOT---------------------
//
User &	Server::getBot(void)
{
	return (this->_bot);
}
