/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelaha <madelaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/20 16:35:25 by madelaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "../inc/Server.hpp"
#include <string>

std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str);
std::vector<unsigned char>                  to_vector(std::string str);
std::vector<unsigned char>                  concat_vectors(std::vector<unsigned char> v1, std::vector<unsigned char> v2);
std::vector<unsigned char>                  server_response(std::string client, std::string code, std::string command, std::string to_insert);
std::string                                 itos(int n);
std::string                                 v_to_str(std::vector<unsigned char> vector);
void	                                    insert_all(std::vector<unsigned char> &my_vec, std::string to_insert);
void	                                    free_fun(Server &my_server);
void                                        add_to_vector(std::vector<unsigned char> v, char *str);
void                                        add_to_vector(std::vector<unsigned char>* v, std::string str);
void                                        add_to_vector(std::vector<unsigned char>*, std::vector<unsigned char>);
void                                        print_vector(std::vector<unsigned char> v);
int		                                    my_compare(std::vector<unsigned char> my_vec, std::string str);

#endif
