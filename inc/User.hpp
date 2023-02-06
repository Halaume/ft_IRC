/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 12:13:53 by ghanquer         ###   ########.fr       */
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
		bool	getRegistered(void) const;
		void	setRegistered(bool);
		void	setPasswd(std::string);
		void	setUserName(std::string);
		void	setRealName(std::string);
	private:
		int						_fd;
		bool					_registered;
		std::string				_passwd;
		std::string				_currCmd;
		std::string				_userName;
		std::string				_realName;
		std::string				_client;
		std::list<Channel>		_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

#endif
