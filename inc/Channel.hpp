/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 22:35:00 by iguscett         ###   ########.fr       */
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
class Command;

class Channel
{
	public:
		Channel(void);
		Channel(const Channel &copy);
		Channel(std::vector<unsigned char> chanName);
		Channel(std::vector<unsigned char> chanName, std::vector<unsigned char> chan_password);
		~Channel(void);
		Channel &	operator=(const Channel & src);
		bool		operator!=(const Channel &) const;

		// GETTERS
		std::vector<unsigned char>	getChanName(void) const;
		std::vector<unsigned char>	getChanPassword(void) const;
		std::list<User *>::iterator	getOpListbg(void);
		std::list<User *>::iterator	getOpListend(void);
		std::list<User *>::iterator	getUserListbg(void);
		std::list<User *>::iterator	getUserListend(void);
		std::list<User *>			getUserList(void);
		
		std::map<char, bool>::iterator	getModesbg(void);
		std::map<char, bool>::iterator	getModesend(void);
		
		int							getNbUsersLimit(void);
		bool						getMode(char c);

		// SETTERS
		void						setChanName(std::string&);
		void						setChanName(std::vector<unsigned char>&);
		void						setChanPassword(std::string&);
		void						setChanPassword(std::vector<unsigned char>&);
		void						setNbUsersLimit(int nb_users_limit);
		void 						setMode(char, bool);

		void						addUser(Command, User *, Server&);
		// void						addUser(User *, Server&, std::vector<unsigned char>);
		bool						isOp(User *) const;
		void						delUser(int);

		bool						isUserInChannel(User*);

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
		int							_nb_users_limit;


};

#endif
