/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/13 16:28:07 by madelaha         ###   ########.fr       */
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
	
		std::list<User>::iterator    getOpListbg(void);
        std::list<User>::iterator    getOpListend(void);
        std::list<User>::iterator    getUsrListbg(void);
        std::list<User>::iterator    getUsrListend(void);
		std::vector<unsigned char>	 getChanName(void) const;
		std::vector<unsigned char>	 getTopic(void) const;
		std::list<User>&			 getUsers(void);
		
		void        setTopic(std::vector<unsigned char>);
		void		addUser(User, Server&);
		void		addUser(User, Server&, std::vector<unsigned char>);
		bool        isOp(User usr) const;
		
	private:
	
		std::vector<unsigned char>	_chanName;
		std::vector<unsigned char>	_chanPassword;
		std::vector<unsigned char>  _topic;
		std::map<char, bool>		_modes;//	A voir pour le string, trouver une facon de normaliser nos modes (Same pour Users) maybe un tableau toujours dans le meme ordre
		int							_userConnected;
		std::vector<User>			_opList;
		std::list<User>				_userLst;
		std::vector<User>			_banLst;


};

#endif
