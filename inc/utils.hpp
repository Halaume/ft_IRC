/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/03/07 15:18:48 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/User.hpp"
#include <string>

std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str);
std::string                                 v_to_str(std::vector<unsigned char> vector);

std::vector<unsigned char>                  to_vector(std::string str);
void                                        add_to_vector(std::vector<unsigned char> &v, char *str);
void                                        add_to_vector(std::vector<unsigned char>& v, std::string str);
// void                                        add_to_vector(std::vector<unsigned char>& v, std::string& str);
void                                        add_to_vector(std::vector<unsigned char>&, std::vector<unsigned char>);
std::vector<unsigned char>                  add_to_v(std::vector<unsigned char> v, std::string str);
std::vector<unsigned char>                  concat_vectors(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
void                                        add_to_vv(std::vector<std::vector <unsigned char> >& vv, std::string str);
void                                        add_to_v(std::vector<unsigned char> &v1, std::vector<unsigned char> &v2);


std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> client, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> client, std::vector<unsigned char> cmd, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int code, std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> v3, std::vector<unsigned char> msg);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(int, std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>);
std::vector<unsigned char>                  concat_resp(std::vector<unsigned char> v1, std::vector<unsigned char> v2, std::vector<unsigned char> v3);


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


void	                                   free_fun(Server &my_server);

void                                       insert_all(std::vector<unsigned char> &my_vec, std::string to_insert);

int	                                       my_compare(std::vector<unsigned char> my_vec, std::string str);
int                                        my_compare(std::vector<unsigned char> v1, std::vector<unsigned char> v2);


std::string                                to_string(std::vector<unsigned char>);

#endif
