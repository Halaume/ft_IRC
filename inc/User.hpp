/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/09 14:56:51 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "Channel.hpp"
#include <vector>

class Channel;

class User
{
	public:
		User(void);
		User(const User &copy);
		~User(void);
		User &	operator=(const User & src);

		
		
		// GETTERS
		// std::string			getCurrCmd(void) const;
		// std::string			getClient(void) const;

		// SETTERS
		// void				setfd(int fd);
		// void				setCurrCmd(std::string currCmd);
		// void				setUserName(std::string name);
		// void				setRealName(std::string name);
		// void				setClient(std::string client);
		
		bool						operator==(User) const;
		bool						operator!=(User) const;
		
		std::vector<Channel>& 		getChannels(void);
		int							getfd(void) const;
		bool						getRegistered(void) const;
		std::vector<unsigned char>	getUserName(void) const;
		std::vector<unsigned char>	getRealName(void) const;
		void						setRegistered(bool);
		void						setPasswd(std::vector<unsigned char>);
		void						setUserName(std::vector<unsigned char>);
		void						setRealName(std::vector<unsigned char>);
		int							getNbChan(void);
		
	private:
		int							_fd;
		bool						_registered;
		std::vector<unsigned char>	_passwd;
		std::string					_currCmd;
		std::vector<unsigned char>	_userName;
		std::vector<unsigned char>	_realName;
		std::string					_client;
		std::vector<Channel>		_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

#endif
