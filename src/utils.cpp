/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/13 17:16:15 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <unistd.h>
#include "../inc/Server.hpp"

void	free_fun(Server &my_server)
{
	my_server.getChannel().erase(my_server.getChannel().begin(), my_server.getChannel().end());
	my_server.getUser().erase(my_server.getUser().begin(), my_server.getUser().end());
	close(my_server.getSct());
	close(my_server.getEpollfd());
}

void	insert_all(std::vector<unsigned char> &my_vec, std::string to_insert)
{
	for (std::vector<unsigned char>::size_type i = 0; to_insert[i]; i++)
		my_vec.push_back(static_cast<unsigned char>(to_insert[i]));
}

std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str)
{
	std::vector<std::vector<unsigned char> >	ret;
	std::vector<unsigned char>::iterator	n = str.begin();
	std::vector<unsigned char>::iterator	prev;
	while (n != str.end())
	{
		if (n != str.begin())
			n++;
		prev = n;
		while (*n != ',')
			n++;
		if (n == str.end())
			ret.insert(ret.end(), std::vector<unsigned char>(prev, str.end()));
		else
			ret.insert(ret.end(), std::vector<unsigned char>(prev, n));
	}
	return (ret);
}

int	my_compare(std::vector<unsigned char> my_vec, std::string str)
{
	if (my_vec.size() != str.length())
		return (1);
	for (std::vector<unsigned char>::size_type i = 0; str[i]; i++)
	{
		if (my_vec[i] != str[i])
			return (1);
	}
	return (0);
}
