/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:39:58 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 18:22:10 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

//--------From my tests---------
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
//------------------------------

#include <map>
#include <string>
#include "Channel.hpp"
#include <vector>

class Command
{
	public:
		Command(const std::string input);
		// Command(const Command &copy);
		~Command(void);
		Command &	operator=(const Command & src);
		
	private:
		Command(void);
		std::vector<std::vector<std::string> >	_commandblock;
		// std::vector<std::string>				_ctypes;
};

#endif
