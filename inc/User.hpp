/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 16:36:07 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "Channel.hpp"
#include <list>

class Channel;

class User
{
	public:
		User(void);
		User(const User &copy);
		~User(void);
		User &	operator=(const User & src);
	private:
		int						_fd;
		std::string				_currCmd;
		std::string				_userName;
		std::string				_realName;
		std::string				_client;
		std::list<Channel>		_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

#endif
