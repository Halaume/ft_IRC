/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:26 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/01 17:28:12 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <list>
#include "../inc/Channel.hpp"

Channel::Channel(void): _chanName(), _chanPassword(), _modes(), _userConnected(), _opList()
{
}

Channel::Channel(const Channel & copy): _chanName(copy._chanName), _chanPassword(copy._chanPassword), _modes(copy._modes), _userConnected(copy._userConnected), _opList(copy._opList)
{
}

Channel::~Channel(void)
{
}

Channel &	Channel::operator=(const Channel & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

bool	Channel::operator<(Channel & lhs) const
{
	return (this->_chanName < lhs._chanName);
}

bool	Channel::operator>(Channel & lhs) const
{
	return (this->_chanName > lhs._chanName);
}
