/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freeaddrinfo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 12:40:47 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 15:28:09 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void	ft_freeaddrinfo(struct addrinfo *head)
{
	//freeaddrinfo(head);
	
	struct addrinfo	*tmp;

	while (head != NULL)
	{
		tmp = head->ai_next;
		free(head);
		head = tmp;
	}
}