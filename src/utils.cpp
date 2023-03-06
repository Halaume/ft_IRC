/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/06 17:14:29 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/User.hpp"

void print_vector(std::string s, std::vector<unsigned char> v)
{
	std::vector<unsigned char>::size_type m;
	
	std::cout << s + " VP:\n";
	for (m = 0; m < v.size(); m++)
		std::cout << v[m];
	std::cout << "|\n";
}

void	free_fun(Server &my_server)
{
	for (std::list<User>::iterator it = my_server.getUsersbg(); it != my_server.getUsersend(); it++)
	{
		epoll_ctl(my_server.getEpollfd(), EPOLL_CTL_DEL, it->getfd(), &(my_server.getEv()));
		close(it->getfd());
	}
	close(my_server.getEpollfd());
	close(my_server.getSct());
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

void add_to_vector(std::vector<unsigned char>& v, std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
}

void add_to_vv(std::vector<std::vector <unsigned char> >& vv, std::string str)
{
	std::vector<unsigned char> v;
	
	for (size_t i = 0; i < str.size(); i++)
		v.push_back(static_cast<unsigned char>(str[i]));
	vv.push_back(v);
}

std::vector<unsigned char> add_to_v(std::vector<unsigned char> v, std::string s)
{
	std::vector<unsigned char> ret;
	
	ret = v;
	int i = 0;
	while (s[i])
	{
		ret.push_back(static_cast<unsigned char>(s[i]));
		i++;
	}
	return (ret);
}

void add_to_v(std::vector<unsigned char> &v1, std::vector<unsigned char> &v2)
{
	for (std::vector<unsigned char>::size_type i = 0; i < v2.size(); i++)
		v1.push_back(v2[i]);
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

std::vector<unsigned char> concat_resp(std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> v3)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char>::size_type i;
	
	for (i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	ret.push_back(' ');
	for (i = 0; i < v2.size(); i++)
		ret.push_back(v2[i]);
	ret.push_back(' ');
	for (i = 0; i < v3.size(); i++)
		ret.push_back(v3[i]);
	return (ret);
}

void print_vector2(std::string s, std::vector<std::vector<unsigned char> > v)
{
	std::vector<std::vector<unsigned char> >::size_type m;
	std::vector<unsigned char>::size_type n;
	
	std::cout << s + " VVP:\n";
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
		while (*n != ',' && n != str.end())
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

std::vector<unsigned char> numToVec(int num)
{
	std::vector<unsigned char> v;
	
	while (num > 0)
	{
		v.push_back('0' + num % 10);
		num /= 10;
	}
	for (std::vector<unsigned char>::size_type i = 0; i < v.size() / 2; i++)
        std::swap(v[i], v[v.size() - i - 1]);
	return (v);
}

std::vector<unsigned char> concat_real_name(std::vector<unsigned char> cmd)
{
	std::vector<unsigned char> ret;
	int args = 0;
	bool is_last_space = false;
	bool has_char_other_than_zero = false;
	
	for (std::vector<unsigned char>::size_type i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] == ' ' && !is_last_space)
		{
			args++;
			is_last_space = true;
		}
		else if (cmd[i] != ' ')
			is_last_space = false;
				
		if (args == 4 && cmd[i] == ':')
		{
			i++;
			while (i < cmd.size() -2)
			{
				if (cmd[i] != ' ')
					has_char_other_than_zero = true;
				ret.push_back(cmd[i]);
				i++;
			}
		}	
	}
	if (has_char_other_than_zero == false)
		ret.clear();
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

std::vector<unsigned char> concat_nick_rpl(std::vector<unsigned char> nick_old, std::vector<unsigned char> user_name, std::vector<unsigned char> mask, std::vector<unsigned char> nick_new)
{
	std::vector<unsigned char> ret;
	std::vector<std::vector<unsigned char> >::size_type it;
	unsigned char text[] = " NICK ";
	int i = 0;
	
	ret = nick_old;
	ret.push_back('!');
	for (it = 0; it < user_name.size(); it++)
		ret.push_back(user_name[it]);
	ret.push_back('@');
	for (it = 0; it < mask.size(); it++)
		ret.push_back(mask[it]);
	while (text[i])
		ret.push_back(text[i++]);
	for (it = 0; it < nick_new.size(); it++)
		ret.push_back(nick_new[it]);
	ret.push_back('\r');
	ret.push_back('\n');
	return (ret);
}

bool contains_ctrl_g(std::vector<unsigned char> v)
{
	for (std::vector<unsigned char>::size_type it = 0; it < v.size(); it++)
	{
		if (v[it] == 7)
			return (true);
	}
	return (false);
}

std::vector<unsigned char> rpl_topic(std::vector<unsigned char> channel, std::vector<unsigned char> topic)
{
	std::vector<unsigned char> ret;
	
	add_to_vector(ret, static_cast<std::string>(" "));
	add_to_v(ret, channel);
	add_to_vector(ret, static_cast<std::string>(" :"));
	add_to_v(ret, topic);
	add_to_vector(ret, static_cast<std::string>("\r\n"));
	return (ret);
}

std::vector<unsigned char> rpl_name(Channel *channel)
{
	std::vector<unsigned char> ret;
	std::vector<unsigned char> v;
	
	ret.push_back(' ');
	if (channel->getMode('s') == true)
		ret.push_back('@');
	else
		ret.push_back('=');
	ret.push_back(' ');
	v = channel->getChanName();
	add_to_v(ret, v);
	add_to_vector(ret, static_cast<std::string>(" :"));	
	for (std::list<User *>::iterator it = channel->getUserListbg(); it != channel->getUserListend();)
	{
		v.clear();
		v = (*it)->getNick();
		if (channel->isOp(*it) == true)
			ret.push_back('@');
		add_to_v(ret, v);
		if (++it != channel->getUserListend())
			ret.push_back(' ');
	}
	add_to_vector(ret, static_cast<std::string>("\r\n"));
	return (ret);
}
