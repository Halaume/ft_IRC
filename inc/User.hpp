/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/21 14:24:52 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

#include "Channel.hpp"

class Channel;

class User
{
	public:
		User(void);
		User(int);
		User(const User &copy);
		~User(void);
		User &	operator=(const User & src);

		bool						operator==(User) const;
		bool						operator==(std::vector<unsigned char>) const;
		bool						operator!=(User) const;
		
		std::vector<Channel *> 		getChannels(void) const;
		std::vector<unsigned char>	getCurrCmd(void) const;
		std::vector<Channel *>::iterator	getChannelsbg(void);
		std::vector<Channel *>::iterator	getChannelsend(void);
		std::vector<unsigned char>::iterator	getCurrCmdbg(void);
		std::vector<unsigned char>::iterator	getCurrCmdend(void);
		int							getfd(void) const;
		int							getPassStatus(void) const;
		bool						getRegistered(void) const;
		std::vector<unsigned char>	getUserName(void) const;
		std::vector<unsigned char>	getPasswd(void) const;
		void						setPassStatus(int);
		std::vector<unsigned char>	getRealName(void) const;
		void						setRegistered(bool);
		void						setPasswd(std::vector<unsigned char>);
		void						setPasswd(std::string);
		void						setUserName(std::vector<unsigned char> user_name);
		void						setUserNamev(std::vector<unsigned char>);
		void						setRealName(std::vector<unsigned char>);
		void						setRet(std::vector<unsigned char>);
		std::vector<unsigned char> &	getRet(void);
		int							getNbChan(void);
		bool						getOperator(void) const;
		void						setOperator(bool);
		void						insertcmd(std::vector<unsigned char> &);
		void						clearCurrCmd(void);
		
	private:
		int							_fd;
		bool						_operator;
		int							_pass_status;
		bool						_registered;
		std::vector<unsigned char>	_passwd;
		std::vector<unsigned char>	_currCmd;
		std::vector<unsigned char>	_userName;
		std::vector<unsigned char>	_realName;
		std::vector<unsigned char>	_client;
		std::vector<unsigned char>	_ret;
		std::vector<Channel *>		_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

std::ostream &		operator<<( std::ostream & o, User const & i);

#endif
