/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/05 18:21:37 by iguscett         ###   ########.fr       */
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
void	                                    insert_all(std::vector<unsigned char> &my_vec, std::string to_insert);
std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str);
int	                                        my_compare(std::vector<unsigned char> my_vec, std::string str);
int                                         my_compare(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
void	                                    free_fun(Server &my_server);

std::string                                 v_to_str(std::vector<unsigned char> vector);

std::vector<unsigned char>                  to_vector(std::string str);
void                                        add_to_vector(std::vector<unsigned char> &v, char *str);
void                                        add_to_vector(std::vector<unsigned char>& v, std::string str);
// void                                        add_to_vector(std::vector<unsigned char>& v, std::string& str);
void                                        add_to_vector(std::vector<unsigned char>&, std::vector<unsigned char>);
std::vector<unsigned char>                  add_to_v(std::vector<unsigned char> v, std::string str);
std::vector<unsigned char>                  concat_vectors(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
void                                        add_to_vv(std::vector<std::vector <unsigned char> >& vv, std::string str);
void                                        add_to_v(std::vector<unsigned char> &v1, const std::vector<unsigned char> &v2);


std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);


std::vector<unsigned char>                  concat_nick_rpl(std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
bool                                        isValidCharacter(unsigned char);

std::vector<unsigned char>                  concat_real_name(std::vector<unsigned char>);

void                                        print_vector(std::string s, std::vector<unsigned char> v);
void                                        print_vector2(std::string s, std::vector<std::vector<unsigned char> > v);

// Responses
std::vector<unsigned char>                  server_response(std::string, std::string, std::string, std::string);

int                                         reparseChannelsKeys(std::vector<unsigned char>, std::vector<std::vector<unsigned char> >*);
    
std::vector<unsigned char>                  numToVec(int);

std::vector<unsigned char>                  rpl_topic(std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  rpl_name(Channel *);

    
bool                                        contains_ctrl_g(std::vector<unsigned char> v);
bool                                        isValidUserMode(const char c);
bool                                        isCharInVector(std::vector<unsigned char> v, char c);

void										message_to_user(Server&, User*, std::vector<unsigned char>);


#endif
