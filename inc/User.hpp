/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 22:07:50 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

#include "Channel.hpp"

#define PASSWORD_NOT_SET	0
#define PASSWORD_SET		1

class Channel;

class User
{
	public:
		User(void);
		User(int);
		User(const User &copy);
		~User(void);
		User &	operator=(const User & src);

		
		
		// GETTERS
		// int					getfd(void) const;
		// std::string			getCurrCmd(void) const;
		// std::string			getUserName(void) const;
		// std::string			getRealName(void) const;
		// std::string			getClient(void) const;

		// SETTERS
		// void				setfd(int fd);
		// void				setCurrCmd(std::string currCmd);
		// void				setUserName(std::string name);
		// void				setRealName(std::string name);
		// void				setClient(std::string client);
		
		bool						operator==(User) const;
		bool						operator!=(User) const;

		bool						isNickValid(std::vector<unsigned char> nick);
		
		std::list<Channel*>& 		getChannels(void);
		int							getfd(void) const;
		int							getPassStatus(void) const;
		bool						getRegistered(void) const;
		std::vector<unsigned char>	getUserName(void) const;
		std::vector<unsigned char>	getRealName(void) const;
		std::vector<unsigned char>	getPasswd(void) const;
		std::vector<unsigned char>	getClient(void) const;
		std::vector<unsigned char>	getNick(void) const;
		int							getNbChan(void);
		
		void						setPassStatus(int);
		void						setRegistered(bool);
		void						setPasswd(std::vector<unsigned char>&);
		
		void						setClient(std::vector<unsigned char>&);
		void						setClient(std::string);
		
		void 						setfd(int);
		void						setNick(std::vector<unsigned char>&);
		void						setPasswd(std::string&);
		void						setUserName(std::vector<unsigned char>&);
		void						setRealName(std::vector<unsigned char>&);
		
		void						addChannel(Channel*);

	private:
		int							_fd;
		int							_pass_status;
		bool						_registered;
		std::vector<unsigned char>	_passwd;
		std::vector<unsigned char>	_currCmd;
		std::vector<unsigned char>	_user_name;
		std::vector<unsigned char>	_real_name;
		std::vector<unsigned char>	_client;
		std::vector<unsigned char>	_nick;
		std::list<Channel*>			_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

std::ostream &		operator<<( std::ostream & o, User const & i);

#endif
