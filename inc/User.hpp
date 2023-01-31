/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 17:40:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/01/31 18:10:46 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "Channel.hpp"

class User
{
	public:
		User(void);
		User(const User &copy);
		~User(void);
		User &	operator=(const User & src);
	private:
		std::string	_userName;
		std::string	_realName;
		std::string	_client;
		int			_chanConnected;
		Channel		_channels[10];//	Size 10 ref:RFC 1459/1.3 Max number of chan for a User

};

#endif
