/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 12:37:46 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include "../inc/Server.hpp"

void	free_fun(Server &my_server)
{
	close(my_server.getSct());
	close(my_server.getEpollfd());
}

void add_to_vector(std::vector<unsigned char> v, char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
}

void add_to_vector(std::vector<unsigned char>* v, std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		(*v).push_back(static_cast<unsigned char>(str[i]));
}

std::vector<unsigned char> to_vector(std::string str)
{
	std::vector<unsigned char> v;
	
	for (size_t i = 0; i < str.size(); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
	return (v);
}


void add_to_vector(std::vector<unsigned char>* v1, std::vector<unsigned char> v2)
{
	for (std::vector<unsigned char>::size_type i = 0; i < v2.size(); i++)
		(*v1).push_back(v2[i]);
}

std::vector<unsigned char> concat_vectors(std::vector<unsigned char> v1, std::vector<unsigned char> v2)
{
	std::vector<unsigned char> ret;

	for (std::vector<unsigned char>::size_type i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	for (std::vector<unsigned char>::size_type j = 0; j < v2.size(); j++)
		ret.push_back(v2[j]);
	return (ret);
}

void print_vector(std::vector<unsigned char> v)
{
	std::vector<unsigned char>::size_type m;
	
	std::cout << "Vector print:\n";
	for (m = 0; m < v.size(); m++)
		std::cout << v[m];
	std::cout << "\n";
}

int ft_strlen(const unsigned char* str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

std::string itos(int n)
{
	std::string str;
	char		c;
	std::basic_string<char>::size_type l;
	
	while (n > 0)
	{
		c = '0' + (n % 10);
		str.push_back(c);
		n /= 10;
	}
	l = str.length();
	for (std::basic_string<char>::size_type i = 0; i < l / 2; i++)
        std::swap(str[i], str[l - i - 1]);
	return (str);
}

void insert_all(std::vector<unsigned char> &my_vec, std::string to_insert)
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
	// std::cout << "vec size:" << my_vec.size() << std::endl;
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


int	my_compare_vec(std::vector<unsigned char> vec1, std::vector<unsigned char> vec2)
{
	if (vec1.size() != vec2.size())
		return (1);

	std::vector<unsigned char>::size_type i = 0;
	while (vec2.size())
	{
		if (vec1[i] != vec2[i])
			return (1);
		i++;
	}
	return (0);
}

