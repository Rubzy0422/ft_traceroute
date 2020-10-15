/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addressinfo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 08:32:52 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/15 12:19:25 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void	*handelerrcode(int errcode, const char *hostname)
{
	if (errcode == -2)
		printf("%s: %s: Name or service not known\n", P_NAME, hostname);
	else if (errcode == -3)
			printf("%s: Temporary failure in name resolution\n", hostname);
	else
		printf("%s: %s: Unknown Error\n", P_NAME, hostname);
	exit(-1);
}

struct addrinfo	*dns_lookup(t_env *env, const char *host)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int errcode;
	char addrstr[100];
	void *ptr;

	if (!ft_strcmp(host, "::1"))
		host = "localhost";
	ft_bzero(&hints, sizeof(hints));
	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	errcode = getaddrinfo(host, NULL, &hints, &res);
	if (errcode != 0)
		return handelerrcode(errcode, host);
	inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, NI_MAXHOST);
	if (res->ai_family == AF_INET)
		ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
	inet_ntop (res->ai_family, ptr, addrstr, NI_MAXHOST);
	env->hostaddr = ft_strdup(addrstr);
	return res;
}

char	*reverse_dns_lookup(char *s_ipv4_addr) 
{ 
	struct sockaddr_in ip4addr;
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	int s;
	
	ft_bzero(&ip4addr, sizeof(struct sockaddr_in));
	ip4addr.sin_family = AF_INET;
	ip4addr.sin_port = htons(0);
	inet_pton(AF_INET, s_ipv4_addr, &ip4addr.sin_addr);
	s = getnameinfo((struct sockaddr *) &ip4addr,sizeof(struct sockaddr_in),
	host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
	if (s == 0)
		return ft_strdup(host);
	else
		return s_ipv4_addr;
}