/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/04 14:49:31 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Server.hpp"

#define PASSWORD_NOT_SET	0
#define PASSWORD_SET		1

class Channel;

class Server;

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

		bool									isNickValid(std::vector<unsigned char> nick);
		
		std::vector<Channel *> 					getChannels(void) const;
		std::vector<unsigned char>&				getCurrCmd(void);
		std::vector<Channel *>::iterator		getChannelsbg(void);
		std::vector<Channel *>::iterator		getChannelsend(void);
		std::vector<unsigned char>::iterator	getCurrCmdbg(void);
		std::vector<unsigned char>::iterator	getCurrCmdend(void);
		int										getfd(void) const;
		int										getPassStatus(void) const;
		bool									getRegistered(void) const;
		std::vector<unsigned char>				getUserName(void) const;
		std::vector<unsigned char>				getPasswd(void) const;
		std::vector<unsigned char>				getNick(void) const;
		int										getNbChan(void);
		std::vector<unsigned char>				getUserMask(void) const;
		int										getPassBeforeNickUser(void) const;
		bool 									getMode(char c);
		std::map<char, bool>::iterator 			getModesbg(void);
		std::map<char, bool>::iterator 			getModesend(void);
		std::vector<unsigned char> &			getRet(void);
		bool									getOperator(void) const;
		
		void									setPassStatus(int);
		std::vector<unsigned char>				getRealName(void) const;
		void									setRegistered(bool);
		void									setPasswd(std::vector<unsigned char>);
		void									setUserName(std::vector<unsigned char>);
		void									setRealName(std::vector<unsigned char>);
		void									setRet(std::vector<unsigned char>);
		void									setOperator(bool);
		std::vector<unsigned char>				getClient();
		void									insertcmd(std::vector<unsigned char> &);
		void									clearCurrCmd(void);
		void									clearRet(void);
		void 									setfd(int);
		void									setNick(std::vector<unsigned char>);
		void									setUserMask(std::vector<unsigned char>&);
		void									setPassBeforeNickUser(int);					
		void									addChannel(Channel*);
		int										createNewNick(Server &my_server);
		
		std::vector<unsigned char>              getUserModes(void);
		void									setMode(char c, bool mode);
		int										modesMessage(std::vector<unsigned char>, bool);

	private:
		int							_fd;
		bool						_operator;
		int							_pass_status;
		bool						_registered;
		std::vector<unsigned char>	_passwd;
		std::vector<unsigned char>	_currCmd;
		std::vector<unsigned char>	_user_name;
		std::vector<unsigned char>	_real_name;
		std::vector<unsigned char>	_nick;
		std::vector<unsigned char>	_ret;
		std::vector<Channel *>		_channels;//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User
		std::vector<unsigned char>	_user_mask;
		int							_pass_before_nick_user;
		std::map<char, bool>		_modes;
};

std::ostream &		operator<<( std::ostream & o, User const & i);

#endif
