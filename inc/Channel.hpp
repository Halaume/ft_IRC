/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/18 22:58:33 by iguscett         ###   ########.fr       */
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

		// GETTERS
		std::vector<unsigned char>	getChanName(void) const;
		std::vector<unsigned char>	getChanPassword(void) const;
		std::list<User *>::iterator	getOpListbg(void);
		std::list<User *>::iterator	getOpListend(void);
		std::list<User *>::iterator	getUsrListbg(void);
		std::list<User *>::iterator	getUsrListend(void);
		std::list<User *>			getUsrList(void);

		// SETTERS
		void						setChanName(std::string&);
		void						setChanName(std::vector<unsigned char>&);
		void						setChanPassword(std::string&);
		void						setChanPassword(std::vector<unsigned char>&);

		void						addUser(User *, Server&);
		void						addUser(User *, Server&, std::vector<unsigned char>);
		bool						isOp(User *) const;
		void						delUser(int);

		// Getters
		std::list<User *>			getUsers(void) const;
		
	private:
		std::vector<unsigned char>	_chan_name;
		std::vector<unsigned char>	_chan_password;
		std::map<char, bool>		_modes;
		int							_user_connected;
		std::list<User *>			_op_list;
		std::list<User *>			_user_list;
		std::vector<User *>			_ban_list;


};

#endif
