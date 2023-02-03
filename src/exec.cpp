/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghanquer <ghanquer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 16:19:20 by ghanquer          #+#    #+#             */
/*   Updated: 2023/02/03 16:34:21 by ghanquer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#define NB_FUNC 16

int	exec(char **parsed_request)
{
	typedef void	(*fptr)(char **);
	fptr			tab[] = {&};
	std::string		value[] = {};
	std::string		cmd(parsed_request[0]);
	int i = 0;
	while (i < NB_FUNC && cmd.compare(value[i]) != 0)
		i++;
	if (i != NB_FUNC)
		*tab[i](parsed_request);
	return (1);
}
