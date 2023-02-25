/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/26 00:38:03 by iguscett         ###   ########.fr       */
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
		if (!my_compare(it->getChanName(), channel))
			return (&(*it));
	}
	return (NULL);
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
	if (listen(_sct, 1000) == -1)
		return (close(_sct), std::cerr << "Error listening socket" << std::endl, 1);
	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		return (std::cerr << "Error on epoll create" << std::endl, 1);
	_ev.events = EPOLLIN | EPOLLET;
	_ev.data.fd = _sct;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sct, &_ev) == -1)
		return (close(_sct), close(_epollfd), std::cerr << "Error on epoll_ctl_add listen socket" << std::endl, 1);
	// std::cout << "0 : epoll fd: " << _epollfd << std::endl;
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

void Server::addNewChannel(Channel& new_channel)
{
	_channels.push_back(new_channel);
}

void Server::run(void)
{
	int accepted = 0;
	int yes = 1;//	For SO_KEEPALIVE
	long retrec;
	socklen_t server_length = sizeof(_server);
	_ev.events = EPOLLIN | EPOLLET;
	std::vector<unsigned char> v;
	std::vector<unsigned char>	CmdIt;
	std::vector<unsigned char>::size_type shift; (void)shift;
	std::vector<unsigned char> iv;
	Command	cmd;

	while (true)
	{	
		unsigned char buf[BUFFER_SIZE] = "";
		int	wait_ret = epoll_wait(_epollfd, _events, 1000, 1);
		std::list<User>::iterator Usr;
		check_kill(*this);
		if (wait_ret == -1)
		{
			std::cout << "wait ret down...\n";
			return ;
		}
		for (int k = 0; k < wait_ret; ++k)
		{
			std::cout << "\n_________MAIN_________\nk:" << k << "Fd: " << _events[k].data.fd << " and event :" << _events[k].events <<"\n";
			if (_events[k].data.fd == _sct)
			{
				accepted = accept(_sct, (sockaddr *)(&_server), &server_length);
				if (accepted == -1 || setsockopt(accepted, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
					return ;
				fcntl(accepted, F_SETFL, O_NONBLOCK);
				_ev.events = EPOLLIN | EPOLLET;
				_ev.data.fd = accepted;
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, accepted, &_ev) == - 1)
					return ;
				std::cout << "0.0 epoll:" << _ev.events << std::endl;
			}
			else
			{
				try
				{
					std::cerr << "2 : accepted fd:" << _events[k].data.fd << std::endl;
					Usr = getUsr(_events[k].data.fd);
					std::vector<std::vector<unsigned char> > ParsedCommand;
					switch (_events[k].events)
					{
						case EPOLLOUT:
							std::cerr << "EVENT EPOLLOUT" << std::endl;
							_ev.events = EPOLLIN | EPOLLET;
							std::cout << "1.00 epoll:" << _ev.events << std::endl;
							_ev.data.fd = Usr->getfd();
							Usr = getUsr(_events[k].data.fd);
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
							std::cout << "1.0 epoll:" << _events[k].events << std::endl;
							ParsedCommand.clear();
							Usr->clearRet();
							sendto(Usr->getfd(), cmd.getRet());
							cmd.getRet().clear();
							print_vector("1.1 CurrCmd", Usr->getCurrCmd());
							cmd._globalCmd.clear();
							CmdIt.clear();
							shift = 0;
							if (Usr->getCurrCmd().size() > 0)
							{
								for (std::vector<unsigned char>::size_type it = 0; it < Usr->getCurrCmd().size(); it++)
								{
									cmd._globalCmd.push_back(Usr->getCurrCmd()[it]);
									if (it > 0 && Usr->getCurrCmd()[it -1] == '\r' && Usr->getCurrCmd()[it] == '\n')
									{
										print_vector("GlobCmd", cmd._globalCmd);
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
												print_vector("PC", iv);
												iv.clear();
											}
										}
										cmd.setParsedCmd(ParsedCommand);
										cmd.setUser(&(*Usr));
										cmd.getRet().clear();
										cmd.answer(*this);
										sendto(Usr->getfd(), cmd.getRet());
										cmd._globalCmd.clear();
										ParsedCommand.clear();
										printUsersList();
										// break ;
									}
								}
								break;
								std::cout << "BUMP HARD AF!\n";
							}
							std::cout << "________EPOLL DEL ______\n\n";
							if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &(_ev)) == - 1)
							{
								epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
								close(_events[k].data.fd);
								_users.erase(Usr);
							}
							break;							
						case EPOLLIN:
							std::cerr << "EVENT EPOLLIN" << std::endl;
							if (isUserInList(_events[k].data.fd) == false)
							{
								User new_user(_events[k].data.fd);
								if (_users.size() == 0)
									new_user.setOperator(true);
								_users.push_back(new_user);
							}
							Usr = getUsr(_events[k].data.fd);
							retrec = recv(Usr->getfd(), buf, BUFFER_SIZE, 0);
							if (retrec == BUFFER_SIZE)
							{
								for (int i = 0; i < BUFFER_SIZE; i++)
									v.push_back(buf[i]);
								Usr->insertcmd(v);
								v.clear();
							}
							else if (retrec > 0)
							{
								for (int i = 0; i < retrec; i++)
									v.push_back(buf[i]);
								Usr->insertcmd(v);
								v.clear();
								ParsedCommand.clear();
								print_vector("2.1", Usr->getCurrCmd());
								cmd._globalCmd.clear();
								for (std::vector<unsigned char>::iterator it = Usr->getCurrCmdbg(); it != Usr->getCurrCmdend(); it++)
								{
									cmd._globalCmd.push_back(*it);
									if (it != Usr->getCurrCmdbg() && *(it - 1) == '\r' && *it == '\n')
									{
										std::vector<unsigned char>::iterator i = Usr->getCurrCmdbg();
										for (std::vector<unsigned char>::iterator j = Usr->getCurrCmdbg(); j != (it + 1); j++)
										{
											if (*j == ' ')
											{
												if (j == Usr->getCurrCmdbg())
												{
													while (*j == ' ')
														j++;
													while (j != (it - 1) && *j != ' ')
														j++;
												}
												ParsedCommand.insert(ParsedCommand.end(), std::vector<unsigned char>(i, j));
												if (j != it - 1)
												{
													while (*j == ' ')
														j++;
													j--;
												}
												i = j + 1;
											}
											else if (j == (it - 2))
												ParsedCommand.insert(ParsedCommand.end(), std::vector<unsigned char>(i, j + 1));
										}
										cmd.setParsedCmd(ParsedCommand);
										cmd.setUser(&(*Usr));
										std::cout << "----------------------------------\n--------    FDuser " << Usr->getfd() << "cmd:    ------\n";
										std::cout << reinterpret_cast<char*>(Usr->getCurrCmd().data());
										Usr->getCurrCmd().erase(Usr->getCurrCmdbg(), it + 1);
										cmd.getRet().clear();
										cmd.answer(*this);
										// sendto(Usr->getfd(), cmd.getRet());
										// if (cmd.answer(*this) == 1)
										// {
										_ev.events = EPOLLOUT | EPOLLET;
										_ev.data.fd = Usr->getfd();
										if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, Usr->getfd(), &_ev) == - 1)
										{
											std::cout << "Epoll ctl error\n";
											epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
											close(_events[k].data.fd);
											if (Usr != _users.end())
												_users.erase(Usr);
										}
										// }
										cmd._globalCmd.clear();
										// print_vector2("2.2 Ret", cmd.getRet());
										// std::cout << "Curr command end EPOLLIN\n";
										// print_vector(Usr->getCurrCmd());
										break;
									}
								}
								printUsersList();
							}
							break;
						default:
							std::cout << "________CASE DEFAULT ______\n\n";
							epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
							close(_events[k].data.fd);
							if (Usr != _users.end())
								_users.erase(Usr);
							break;
						std::cout << "________END OF SWITCH______\n\n";
					}
					std::cout << "________OUT OF SWITCH______\n\n";
					std::cout << "events state:" << _events[k].events << "\n";
				}
				catch (std::exception & e)
				{
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, _events[k].data.fd, &_ev);
					if (getUsr(_events[k].data.fd) != _users.end())
						_users.erase(getUsr(_events[k].data.fd));
					close(_events[k].data.fd);
				}
			}
		}
	}
}

void Server::sendto(int fd, std::vector<std::vector<unsigned char> >& buf)
{
	long int ret;
	std::vector<std::vector<unsigned char> >::size_type it;
	
	for (it = 0; it < buf.size(); it++)
	{
		ret = send(fd, reinterpret_cast<char *>(buf[it].data()), buf[it].size(), MSG_NOSIGNAL);
		if (ret < 0)
		{
			std::list<User>::iterator Usr = getUsr(fd);
			epoll_ctl(_epollfd, EPOLL_CTL_DEL, Usr->getfd(), &_ev);
			_users.erase(Usr);
			close(fd);
			return;
		}
	}
}

std::list<User>::iterator Server::findUser(std::vector<unsigned char> nick)
{
	std::list<User>::iterator it;
	
	for (it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getUserName() == nick)
			return (it);
	}
	return (it);
}

std::list<User>::iterator Server::findUserNick(std::vector<unsigned char> nick)
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
	return (it); // _users.end()?
}

User* Server::findUserPtrNick(std::vector<unsigned char> nick)
{
	int i = 0;
	std::list<User>::iterator itu;

	for (itu = _users.begin(); itu != _users.end(); ++itu)
	{
		if (!my_compare(itu->getNick(), nick))
			return (&(*itu));
		i++;
	}
	return (NULL);
}

void Server::printUsersList(void)
{
	std::cout << "____PRINT USERS____\n";
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << "__END PRINT USERS__\n";
}

// GETTERS

std::list<User>::iterator	Server::getUsr(int fd)
{
	for (std::list<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getfd() == fd)
			return (it);
	}
	return (_users.end());
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

std::list<User>::iterator Server::getUsersbg(void)
{
	return (_users.begin());
}

std::list<User>::iterator Server::getUsersend(void)
{
	return (_users.end());
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

bool Server::channelExists(std::vector<unsigned char>& channel_name)
{
	std::vector<Channel>::iterator it;
	
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (!my_compare(it->getChanName(), channel_name))
			return (true);
	}
	return (false);
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

















// void Server::getParsedCmd(Command* cmd, std::vector<unsigned char> v, std::vector<std::vector<unsigned char> >::size_type i)
// {
// 	std::vector<std::vector<unsigned char> >::size_type j;
// 	bool is_last_space = false;
	
// 	v.clear();
// 	cmd->_parsedCmd.clear();
// 	for (j = 0; j < cmd->_globalCmd[i].size(); j++)
// 	{	
// 		if (!is_last_space && (cmd->_globalCmd[i][j] == ' ' || j == cmd->_globalCmd[i].size() - 2))
// 		{
// 			is_last_space = true;
// 			cmd->_parsedCmd.push_back(v);
// 			v.clear();
// 		}
// 		else if (cmd->_globalCmd[i][j] != ' ')
// 		{
// 			is_last_space = false;
// 			v.push_back(cmd->_globalCmd[i][j]);
// 		}
// 	}
// }