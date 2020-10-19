/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 13:02:43 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 15:50:15 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ft_traceroute.h>

void *ft_calloc(size_t blocks, size_t size)
{
	void *ptr;
	
	ptr = malloc(blocks * size);
	ft_bzero(ptr, sizeof(ptr));

	return ptr;
}

void ft_error(char *msg, int errcode)
{
	if (errcode == 0)
		printf("%s\n", msg);
	else
		printf("%s : ERRCODE: %d", msg, errcode);
	exit(-1);
}