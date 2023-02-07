/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/07 15:52:04 by ghanquer         ###   ########.fr       */
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
		std::vector<unsigned char>	getChanName(void) const;
		void		addUser(User, Server&);
		void		addUser(User, Server&, std::vector<unsigned char>);
	private:
		std::vector<unsigned char>	_chanName;
		std::vector<unsigned char>	_chanPassword;
		std::map<char, bool>		_modes;//	A voir pour le string, trouver une facon de normaliser nos modes (Same pour Users) maybe un tableau toujours dans le meme ordre
		int							_userConnected;
		std::vector<User>			_opList;
		std::vector<User>			_userLst;
		std::vector<User>			_banLst;


};

#endif
