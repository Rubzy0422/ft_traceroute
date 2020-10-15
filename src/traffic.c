/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traffic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 08:20:32 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/15 14:49:04 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void	sendloop(t_env *env)
{
	fd_set readset;
	int hop = 1;

	FD_ZERO (&readset);
	FD_SET (env->icmp.icmpfd, &readset);
	
	while (hop <= env->maxhops)
	{
		if (!sameprevious(env))
		{
			printf(" %d ", hop);
			setprevious(env);
			send_probes(env, &readset);
			printf("\n");
		}
		ttl_advance(&env->icmp);
		hop++;
	}
	return;
}

void	send_probes(t_env *env, fd_set *readset)
{
	int try;
	int ret;
	struct timeval timeout;

	ret = -1;
	try = 0;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	while (try < env->maxtries)
	{
		send_echo(env);
		ret = select(env->icmp.icmpfd + 1, readset, NULL, NULL, &timeout);
		if (ret < 0)
			perror("Ret select: ");
		if (ret == 0)
		{
			printf(" *");
			env->icmp.hostaddr = NULL;
			env->icmp.hostname = NULL;
		}
		if (ret > 0)
			if (FD_ISSET(env->icmp.icmpfd, readset))
		 		read_echo(env, try);
		try++;
	}
}

void	ttl_advance (t_trace * icmp)
{
	const int *ttlp;

	ttlp = &icmp->ttl;
	icmp->ttl++;
	if (setsockopt (icmp->icmpfd, IPPROTO_IP, IP_TTL, ttlp, sizeof (*ttlp)) < 0)
		printf("setsockopt Error");
}
