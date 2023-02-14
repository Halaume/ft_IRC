/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/13 22:22:19 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>

const std::string server_name = "lateam.42.fr";

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

std::string v_to_str(std::vector<unsigned char> vector)
{
	std::string str;

	for (std::vector<unsigned char>::size_type i = 0; i < vector.size(); i++)
		str.push_back(vector[i]);
	return (str);
}

// Responses
std::vector<unsigned char> server_response(std::string client, std::string code, std::string command, std::string to_insert)
{
	int j;
	std::vector<unsigned char> ret;

	ret.push_back(':');
	// if (client.length() > 0)
	// {
	// 	for (j = 0; j < (int)client.length(); j++)
	// 		ret.push_back(client[j]);
	// 	ret.push_back('!');
	// 	for (j = 0; j < (int)client.length(); j++)
	// 		ret.push_back(client[j]);
	// 	ret.push_back('@');
	// 	for (j = 0; j < (int)server_name.length(); j++)
	// 		ret.push_back(server_name[j]);
	// }
	// else
	// {
		for (j = 0; j < (int)server_name.length(); j++)
			ret.push_back(server_name[j]);
	// }
	ret.push_back(' ');
	for (j = 0; j < (int)code.length(); j++)
		ret.push_back(code[j]);
	ret.push_back(' ');
	for (j = 0; j < (int)client.length(); j++)
		ret.push_back(client[j]);
	ret.push_back(' ');
	for (j = 0; j < (int)command.length(); j++)
		ret.push_back(command[j]);
	ret.push_back(' ');
	ret.push_back(':');
	for (std::vector<unsigned char>::size_type i = 0; to_insert[i]; i++)
		ret.push_back(static_cast<unsigned char>(to_insert[i]));
	return (ret);
}
