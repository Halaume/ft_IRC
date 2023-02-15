/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 14:47:28 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include "User.hpp"
#include "Server.hpp"

class User;
class Server;

class Channel
{
	public:
		Channel(void);
		Channel(const Channel &copy);
		Channel(std::vector<unsigned char> chanName);
		~Channel(void);
		Channel &	operator=(const Channel & src);
		bool		operator!=(const Channel &) const;
		std::vector<unsigned char>	getChanName(void) const;
		std::list<User *>::iterator	getOpListbg(void);
		std::list<User *>::iterator	getOpListend(void);
		std::list<User *>::iterator	getUsrListbg(void);
		std::list<User *>::iterator	getUsrListend(void);
		std::list<User *>			getUsrList(void);
		void						addUser(User *, Server&);
		void						addUser(User *, Server&, std::vector<unsigned char>);
		bool						isOp(User *) const;
		void						delUser(int);

		// Getters
		std::list<User *>			getUsers(void) const;
		
	private:
		std::vector<unsigned char>	_chanName;
		std::vector<unsigned char>	_chanPassword;
		std::map<char, bool>		_modes;
		int							_userConnected;
		std::list<User *>			_opList;
		std::list<User *>			_userLst;
		std::vector<User *>			_banLst;


};

#endif
