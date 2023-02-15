/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:48:32 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 15:56:54 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
#define NUMERICS_HPP

#include <vector>
#include <string>

#include "../inc/utils.hpp"

# define ERR_NEEDMOREPARAMSmsg(param) concat_vectors(param, to_vector(" :Not enough parameters")) 

enum numerics {
	ERR_NEEDMOREPARAMS = 461
};

std::vector<unsigned char>      numeric_response(int num_code, std::vector<unsigned char> param);

#endif
