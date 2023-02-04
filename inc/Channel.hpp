/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:48:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 16:36:53 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <list>
#include "User.hpp"

class User;

class Channel
{
	public:
		Channel(void);
		Channel(const Channel &copy);
		~Channel(void);
		Channel &	operator=(const Channel & src);
		bool		operator<(Channel & lhs) const;
		bool		operator>(Channel & lhs) const;
	private:
		std::string				_chanName;
		std::string				_chanPassword;
		std::string				_modes;//	A voir pour le string, trouver une facon de normaliser nos modes (Same pour Users) maybe un tableau toujours dans le meme ordre
		int						_userConnected;
		std::list<User>			_opList;
		std::list<User>			_userLst;


};

#endif
