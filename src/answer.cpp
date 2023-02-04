/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   answer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 16:19:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 12:18:29 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "function.cpp"

std::map<std::string, typedef std::string (*fptr)(Command, Server &)>	insert_all_cmd()
{
	std::map<std::string, fptr>	my_map;

	my_map.insert("CAP", &fun_CAP);
	my_map.insert("NICK", &fun_NICK);
	my_map.insert("USER", &fun_USER);
	my_map.insert("PASS", &fun_PASS);
	my_map.insert("JOIN", &fun_JOIN);
	my_map.insert("PRIVMSG", &fun_PRIVMSG);
	my_map.insert("OPER", &fun_OPER);
	my_map.insert("QUIT", &fun_QUIT);
	my_map.insert("ERROR", &fun_ERROR);
	my_map.insert("MODE", &fun_MODE);
	my_map.insert("TOPIC", &fun_TOPIC);
	my_map.insert("KICK", &fun_KICK);
	my_map.insert("INVITE", &fun_INVITE);
	my_map.insert("KILL", &fun_KILL);
	my_map.insert("RESTART", &fun_RESTART);

}

std::string	answer(Command my_cmd, Server &my_server)
{
	typedef std::string	(*fptr)(Command, Server);

	std::map<std::string, fptr>				lst_cmd = insert_all_cmd();
	std::map<std::string, fptr>::iterator	my_iterator = find(lst_cmd.cmd[0][0]);

	if (my_iterator == lst_cmd.end())
		return ("Message erreur cmd not found si il y en a un");
	//si ca viens de irssi ca devrais pas arriver mais
	//si on nous envoie une cmd qui existe pas ca sortirais la
	return (my_iterator->second(my_cmd, my_server));
}
