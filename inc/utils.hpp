/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/14 22:15:32 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

std::string                                 itos(int n);
void	                                    insert_all(std::vector<unsigned char> &my_vec, std::string to_insert);
std::vector<std::vector<unsigned char> >	splitOnComa(std::vector<unsigned char> str);
int	                                        my_compare(std::vector<unsigned char> my_vec, std::string str);

std::string                                 v_to_str(std::vector<unsigned char> vector);

void                                        add_to_vector(std::vector<unsigned char> v, char *str);
void                                        add_to_vector(std::vector<unsigned char>* v, std::string str);
void                                        add_to_vector(std::vector<unsigned char>*, std::vector<unsigned char>);

void                                        print_vector(std::vector<unsigned char> v);

// Responses
std::vector<unsigned char>                  server_response(std::string client, std::string code, std::string command, std::string to_insert);

#endif
