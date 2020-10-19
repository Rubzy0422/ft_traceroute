/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addrinfo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 12:24:12 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 15:03:54 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void	handelerror(int errcode, const char *host)
{
	if (errcode == -2)
		printf("%s: Name or service not known\n", host);
	if (errcode == -3)
		printf("%s: Temporary failure in name resolution\n", host);
	exit(0);
}

struct addrinfo* get_host_ai(const char *host, const char *serv,
							int family, int socktype)
{
	int				n;
	struct addrinfo	hints;
	struct addrinfo	*res;

	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME; 
	hints.ai_family = family; 
	hints.ai_socktype = socktype; 

	n = getaddrinfo(host, serv, &hints, &res);
	if (n != 0)
	{
		handelerror(n, host);
		return (NULL);
	}
	return (res);
}

char *getipaddr(const struct sockaddr *sa, char *hostname)
{
	static char str[128];

	if (sa->sa_family == AF_INET)
	{
		struct sockaddr_in *sin = (struct sockaddr_in *) sa;
		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return (NULL);
		return(str);
	}
	else
	{
		printf("%s: Address family for hostname not supported\n", hostname);
		exit(0);
	}
	return (NULL);
}