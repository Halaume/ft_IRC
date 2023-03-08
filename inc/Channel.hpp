/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/07 21:23:30 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include "User.hpp"
#include "Server.hpp"

# define ADD 1
# define REMOVE -1

# define NO_RET 0
# define RET 1
# define RET_AND_UMODEIS 2

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
		std::vector<unsigned char>		getChanName(void) const;
		std::vector<unsigned char>		getChanPassword(void) const;
		std::list<User *>::iterator		getOpListbg(void);
		std::list<User *>::iterator		getOpListend(void);
		std::list<User *>::iterator		getUserListbg(void);
		std::list<User *>::iterator		getUserListend(void);
		std::list<User *>				getUserList(void);
		std::list<User *>::iterator		getUserListOpbg(void);
		std::list<User *>::iterator		getUserListOpend(void);
		std::list<User *>::iterator		getUserListBanbg(void);
		std::list<User *>::iterator		getUserListBanend(void);
		std::list<User *>::iterator		getUserListInvitebg(void);
		std::list<User *>::iterator		getUserListInviteend(void);
		
		std::map<char, bool>::iterator	getModesbg(void);
		std::map<char, bool>::iterator	getModesend(void);
		
		int								getNbUsers(void);
		int								getNbUsersLimit(void);
		bool							getMode(char c);
		void							makeOp(User *);

		// SETTERS
		void							setChanName(std::vector<unsigned char>);
		void							setChanPassword(std::vector<unsigned char>&);
		void							setNbUsersLimit(int nb_users_limit);
		void 							setMode(char, bool);
	
		void							addUser(User *);
		void 							addUserToBan(User*);
		void 							addUserToInvite(User*);
		
		// void							addUser(User *, Server&, std::vector<unsigned char>);
		bool							isOp(User *) const;
		void							delUserLst(User*);
		void							delOpLst(User*);

		void							delUser(int);
		void							delBanLst(User *);

		bool							isUserInChannel(User*);
		bool							isUserBanned(User*);
		bool							isUserInvited(User*);
	
		// Getters	
		std::list<User *>				getUsers(void) const;

		std::vector<unsigned char>&	 	getTopic(void);
		std::map<char, bool>		 	getModes(void) const;

		void        					setTopic(std::vector<unsigned char>);
		bool        					isOp(User) const;

		std::list<User *>::iterator		findUser(std::vector<unsigned char> nick);
		int 							modesMessage(Server&, User*, std::vector<std::vector<unsigned char> >, bool);
		std::vector<unsigned char>		getChannelModes(void);

		void							operatorModeFct(Server&, User*, std::vector<std::vector<unsigned char> >, int &, int);
		void							inviteModeFct(int &, int);
		void							limitModeFct(std::vector<std::vector<unsigned char> >, int &, int);
		void 							keyModeFct(User *, std::vector<std::vector<unsigned char> >, int &, int);
		void							topicModeFct(int &, int);
		void							banModeFct(Server &, User *, std::vector<std::vector<unsigned char> >, int &, int);


	private:	
		std::vector<unsigned char>		_chan_name;
		std::vector<unsigned char>		_chan_password;
		std::map<char, bool>			_modes;
		std::list<User *>				_op_list;
		std::list<User *>				_user_list;
		std::list<User *>				_ban_list;
		int								_nb_users_limit;
		std::list<User *>				_invite_list;
		std::vector<unsigned char>		_topic;
		// std::list<User *>				_ban_list;


};

#endif
