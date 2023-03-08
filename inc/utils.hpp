/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/08 18:22:43 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/User.hpp"

const std::string server_name = "mig.42.fr";
const std::vector<unsigned char> no_param;

std::string                                 itos(int n);
std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str);
int	                                        my_compare(std::vector<unsigned char> my_vec, std::string str);
int                                         my_compare(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
std::string                                 v_to_str(std::vector<unsigned char> vector);
void                                        add_to_vv(std::vector<std::vector <unsigned char> >& vv, std::string str);
std::vector<unsigned char>                  add_to_v(std::vector<unsigned char> v, std::string str);
void                                        add_to_v(std::vector<unsigned char> &v1, const std::vector<unsigned char> &v2);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> client, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> client, std::vector<unsigned char> cmd, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> v3, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> v3);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_nick_rpl(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_real_name(std::vector<unsigned char>);
std::vector<unsigned char>                  concat_vectors(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
std::vector<unsigned char>                  server_response(std::string, std::string, std::string, std::string);
std::vector<unsigned char>                  rpl_topic(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  to_vector(std::string str);
std::vector<unsigned char>                  rpl_name(Channel *);
std::vector<unsigned char>                  numToVec(int);
void	                                    free_fun(Server &my_server);
void                                        insert_all(std::vector<unsigned char> &my_vec, std::string to_insert);
void                                        print_vector(std::string s, std::vector<unsigned char> v);
void                                        print_vector2(std::string s, std::vector<std::vector<unsigned char> > v);
void                                        add_to_vector(std::vector<unsigned char> &v, char *str);
void                                        add_to_vector(std::vector<unsigned char>& v, std::string str);
void                                        add_to_vector(std::vector<unsigned char>&, std::vector<unsigned char>);
int	                                        my_compare(std::vector<unsigned char> my_vec, std::string str);
int                                         my_compare(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
int                                         reparseChannelsKeys(std::vector<unsigned char>, std::vector<std::vector<unsigned char> >*);
bool                                        isValidCharacter(unsigned char);
std::string                                 to_string(std::vector<unsigned char>);
std::string                                 v_to_str(std::vector<unsigned char> vector);
bool                                        contains_ctrl_g(std::vector<unsigned char> v);
bool                                        isValidUserMode(const char c);
bool                                        isCharInVector(std::vector<unsigned char> v, char c);
void										message_to_user(Server&, User*, std::vector<unsigned char>);
int                                         vtoi(std::vector<unsigned char> v);
std::vector<unsigned char>                  itov(int n);
std::vector<unsigned char>                  concatMode(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<std::vector<unsigned char> >, int);
std::vector<unsigned char>                  concatMode(Channel *, User *, std::vector<unsigned char>);
std::vector<unsigned char>                  userMadeOpertorMsg(std::vector<unsigned char>, User *, int);

#endif
