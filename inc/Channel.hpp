/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/06 16:30:06 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <list>
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
		Channel(std::string chanName);
		~Channel(void);
		Channel &	operator=(const Channel & src);
		std::string	getChanName(void) const;
		void		addUser(User, Server&);
	private:
		std::string				_chanName;
		std::string				_chanPassword;
		std::map<char, bool>	_modes;//	A voir pour le string, trouver une facon de normaliser nos modes (Same pour Users) maybe un tableau toujours dans le meme ordre
		int						_userConnected;
		std::list<User>			_opList;
		std::list<User>			_userLst;
		std::vector<User>		_banLst;


};

#endif
