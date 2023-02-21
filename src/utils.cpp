/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/21 18:32:58 by iguscett         ###   ########.fr       */
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

void add_to_vector(std::vector<unsigned char>& v, char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
}

void add_to_vector(std::vector<unsigned char>& v, std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
}

void add_to_vector(std::vector<unsigned char>& v1, std::vector<unsigned char> v2)
{
	for (std::vector<unsigned char>::size_type i = 0; i < v2.size(); i++)
		v1.push_back(v2[i]);
}

std::vector<unsigned char> to_vector(std::string str)
{
	return (std::vector<unsigned char>(str.begin(), str.end()));
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

std::vector<unsigned char> concat_resp(int code, std::vector<unsigned char> msg)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char>::size_type i;
	std::string scode = itos(code);
	
	ret.push_back(' ');
	if (code < 10)
	{
		ret.push_back('0');
		ret.push_back('0');
		ret.push_back(scode[0]);
	}
	else
	{
		for (std::string::size_type j = 0; j < scode.size(); j++)
			ret.push_back(scode[j]);
	}
	for (i = 0; i < msg.size(); i++)
		ret.push_back(msg[i]);
	return (ret);
}

std::vector<unsigned char> concat_resp(int code, std::vector<unsigned char> v1, std::vector<unsigned char> msg)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char>::size_type i;
	std::string scode = itos(code);
	
	ret.push_back(' ');
	if (code < 10)
	{
		ret.push_back('0');
		ret.push_back('0');
		ret.push_back(scode[0]);
	}
	else
	{
		for (std::string::size_type j = 0; j < scode.size(); j++)
			ret.push_back(scode[j]);
	}
	ret.push_back(' ');
	for (i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	for (i = 0; i < msg.size(); i++)
		ret.push_back(msg[i]);
	return (ret);
}

std::vector<unsigned char> concat_resp(int code, std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> msg)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char>::size_type i;
	std::string scode = itos(code);
	
	ret.push_back(' ');
	if (code < 10)
	{
		ret.push_back('0');
		ret.push_back('0');
		ret.push_back(scode[0]);
	}
	else
	{
		for (std::string::size_type j = 0; j < scode.size(); j++)
			ret.push_back(scode[j]);
	}
	ret.push_back(' ');
	for (i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	ret.push_back(' ');
	for (i = 0; i < v2.size(); i++)
		ret.push_back(v2[i]);
	for (i = 0; i < msg.size(); i++)
		ret.push_back(msg[i]);
	return (ret);
}

void print_vector(std::vector<unsigned char> v)
{
	std::vector<unsigned char>::size_type m;
	
	std::cout << "Vector print|\n";
	for (m = 0; m < v.size(); m++)
		std::cout << v[m];
	std::cout << "|\n";
}

void print_vector2(std::vector<std::vector<unsigned char> > v)
{
	std::vector<std::vector<unsigned char> >::size_type m;
	std::vector<unsigned char>::size_type n;
	
	std::cout << "Vector2 print|\n";
	for (m = 0; m < v.size(); m++)
	{
		for (n = 0; n < v[m].size(); n++)
			std::cout << v[m][n];
		std::cout << "|\n";
	}
}

int ft_strlen(const unsigned char* str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
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
	if (my_vec.size() != str.length())
		return (1);

	for (std::vector<unsigned char>::size_type i = 0; i < my_vec.size(); i++)
	{
		if (my_vec[i] != str[i])
			return (1);
			
	}
	return (0);
}

int	my_compare(std::vector<unsigned char> v1, std::vector<unsigned char> v2)
{
	// std::cout << "size1:" << v1.size() << " size2:" << v2.size() << std::endl;
	if (v1.size() != v2.size())
		return (1);
	for (std::vector<unsigned char>::size_type i = 0; i < v1.size(); i++)
	{
		// std::cout << "getcli:" << v1[i] << " and cli:" << v2[i] << std::endl;
		if (v1[i] != v2[i])
			return (1);
	}
	return (0);
}

bool isValidCharacter(unsigned char c)
{
	if ((c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z') ||
		(c == '-') || (c == '_') ||
		(c == '[') || (c == ']') ||
		(c == '{') || (c == '}') ||
		(c == '\\') || (c == '`') ||
		(c == '|'))
		return (true);
	return (false);
}

std::vector<unsigned char> concat_real_name(std::vector<std::vector<unsigned char> > parsedCmd, int start)
{
	std::vector<unsigned char> ret;
	int count = 0;
	
	for (std::vector<std::vector<unsigned char> >::size_type i = 0; i < parsedCmd.size(); i++)
	{
		if (count >= 4)
		{
			if (count > 4)
				ret.push_back(' ');
			for (std::vector<unsigned char>::size_type j = 0; j < parsedCmd[i].size(); j++)
			{
				ret.push_back(parsedCmd[i][j]);
			}
		}
		if (start > 0)
			start--;
		count++;
	}
	return (ret);
}

int reparseChannelsKeys(std::vector<unsigned char> cmd, std::vector<std::vector<unsigned char> > *chan_key)
{
	std::vector<unsigned char> v;

	for (std::vector<unsigned char>::size_type it = 0; it < cmd.size(); ++it)
	{
		if (cmd[it] == ',' && v.size() > 0)
		{
			chan_key->push_back(v);
			v.clear();
		}
		else if (it == (cmd.size() - 1))
		{
			if (cmd[it] != ',')
				v.push_back(cmd[it]);
			if (v.size() > 0)
			{
				chan_key->push_back(v);
				v.clear();
			}
		}
		else if (cmd[it] != ',')
			v.push_back(cmd[it]);
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

