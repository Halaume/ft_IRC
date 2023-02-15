/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguscett <iguscett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:27 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/15 15:57:30 by iguscett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>

#include "../inc/Numerics.hpp"
#include "../inc/utils.hpp"

std::vector<unsigned char> numeric_response(int num_code, std::vector<unsigned char> param)
{
	switch (num_code)
	{
		case ERR_NEEDMOREPARAMS:
		{
			return (ERR_NEEDMOREPARAMSmsg(param));
			// break;
		}
	}
	
	// to clean
	return (ERR_NEEDMOREPARAMSmsg(param));
}