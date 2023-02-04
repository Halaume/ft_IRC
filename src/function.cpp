/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   function.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 12:14:15 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/04 14:53:01 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

std::string	fun_CAP(Command my_cmd, Server &my_server);
std::string	fun_NICK(Command my_cmd, Server &my_server);
std::string	fun_USER(Command my_cmd, Server &my_server);
std::string	fun_PASS(Command my_cmd, Server &my_server);
std::string	fun_JOIN(Command my_cmd, Server &my_server);
std::string	fun_PRIVMSG(Command my_cmd, Server &my_server);
std::string	fun_OPER(Command my_cmd, Server &my_server);
std::string	fun_QUIT(Command my_cmd, Server &my_server);
std::string	fun_ERROR(Command my_cmd, Server &my_server);
std::string	fun_MODE(Command my_cmd, Server &my_server);
std::string	fun_TOPIC(Command my_cmd, Server &my_server);
std::string	fun_KICK(Command my_cmd, Server &my_server);
std::string	fun_INVITE(Command my_cmd, Server &my_server);
std::string	fun_KILL(Command my_cmd, Server &my_server);
std::string	fun_RESTART(Command my_cmd, Server &my_server);
std::string	fun_PING(Command my_cmd, Server &my_server);
