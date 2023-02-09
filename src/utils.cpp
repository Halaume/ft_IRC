/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/09 20:06:40 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>

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
	std::cout << "vec size:" << my_vec.size() << std::endl;
	if (my_vec.size() > 1000)
	{
		for (std::vector<unsigned char>::size_type j = 0; j < my_vec.size(); j++)
			std::cout << my_vec[j];
	}
	
	if (my_vec.size() != str.length())
	{
		// std::cout << "error 1\n";
		return (1);
	}
		
	for (std::vector<unsigned char>::size_type i = 0; str[i]; i++)
	{
		if (my_vec[i] != str[i])
		{
			// std::cout << "error 2\n";
			return (1);
		}
			
	}
	return (0);
}
