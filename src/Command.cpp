/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:11:10 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 18:38:49 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include "../inc/Command.hpp"

Command::Command(void)
{

}

Command::Command(const std::string input)
{
	std::string commandline;
	std::string token;
	std::string striterator;
	std::vector<std::string> v;
	
	striterator = input;
	while (striterator != "")
	{
		commandline = striterator.substr(0, striterator.find("\r\n"));
		striterator = striterator.substr(striterator.find("\r\n")+2, striterator.length());
		v.clear();
		while (commandline != "")
		{
			token = commandline.substr(0, commandline.find(" "));
			v.push_back(token);
			if (commandline.substr(commandline.find(" ") +1, commandline.length()).length() == commandline.length())
				commandline = "";
			else
				commandline = commandline.substr(commandline.find(" ") +1, commandline.length());
			
		



				
			// std::cout << "Token:|" << token << "|" << std::endl;
			// std::cout << "Cmdline:|" << commandline << "|" << std::endl;
			
		}
		// for (int i = 0; i < (int)v.size(); i++)
		// 	std::cout << v[i] << std::endl;
		_commandblock.push_back(v);

		
		// std::cout << "Commandline:|" << commandline << "|" << std::endl;
		// std::cout << "Strit:|" << striterator << "|" << std::endl;
	}




	for (int i = 0; i < (int)_commandblock.size(); i++) {
		for (int j = 0; j < (int)_commandblock[i].size(); j++) 
			std::cout << "|" << _commandblock[i][j] << "|";
		std::cout << std::endl;
	}





	// token = input.substr(input.find("\r\n")+1, input.length());
	// std::cout << token << std::endl;
}

// Command::Command(const Command & copy): _channels(copy._channels)
// {
// }

Command::~Command(void)
{
}

Command &	Command::operator=(const Command & src)
{
	if (&src == this)
		return (*this);
	return (*this);
}

// _____GETTERS_____
