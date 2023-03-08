/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 20:32:59 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/User.hpp"
#include <string>

const std::string server_name = "mig.42.fr";
const std::vector<unsigned char> no_param;

std::string                                 itos(int n);
void                                        insert_all(std::vector<unsigned char> &, std::string);
std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char>);
int	                                        my_compare(std::vector<unsigned char>, std::string);
int                                         my_compare(std::vector<unsigned char>, std::vector<unsigned char>);
void	                                    free_fun(Server &);

std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);

std::vector<unsigned char>                  concat_nick_rpl(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_real_name(std::vector<unsigned char>);
std::vector<unsigned char>                  add_to_v(std::vector<unsigned char>, std::string);
std::vector<unsigned char>                  concat_vectors(std::vector<unsigned char>, std::vector<unsigned char>);
std::string                                 v_to_str(std::vector<unsigned char>);
std::vector<unsigned char>                  to_vector(std::string str);

void                                        add_to_vector(std::vector<unsigned char> &, char *);
void                                        add_to_vector(std::vector<unsigned char>&, std::string);
void                                        add_to_vector(std::vector<unsigned char>&, std::vector<unsigned char>);
void                                        add_to_vv(std::vector<std::vector <unsigned char> >&, std::string);
void                                        add_to_v(std::vector<unsigned char> &, const std::vector<unsigned char> &);
std::vector<unsigned char>                  add_to_v(std::vector<unsigned char>, std::string);

bool                                        isValidCharacter(unsigned char);
void                                        print_vector(std::string, std::vector<unsigned char>);
void                                        print_vector2(std::string, std::vector<std::vector<unsigned char> >);
std::vector<unsigned char>                  server_response(std::string, std::string, std::string, std::string);

int                                         reparseChannelsKeys(std::vector<unsigned char>, std::vector<std::vector<unsigned char> >*);
std::vector<unsigned char>                  numToVec(int);
std::vector<unsigned char>                  rpl_topic(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  rpl_name(Channel *);
  
bool                                        contains_ctrl_g(std::vector<unsigned char> v);
bool                                        isValidUserMode(const char c);
bool                                        isCharInVector(std::vector<unsigned char> v, char c);
void										message_to_user(Server&, User*, std::vector<unsigned char>);

int                                         vtoi(std::vector<unsigned char> v);
std::vector<unsigned char>                  itov(int n);
std::vector<unsigned char>                  concatMode(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >, int);
std::vector<unsigned char>                  concatMode(Channel *, User *, std::vector<unsigned char>);
std::vector<unsigned char>                  userMadeOpertorMsg(std::vector<unsigned char>, User *, int);

std::string                                 to_string(std::vector<unsigned char>);

#endif
