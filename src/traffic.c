/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traffic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 08:20:32 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/13 11:35:05 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void	sendloop(t_env *env)
{
	int	try;
	int	readonly = 0;
	fd_set readset;
	struct timeval timeout, now;
	int hop = 1;
	env->loop = false;
	int ret = -1;

	FD_ZERO (&readset);
	FD_SET (env->icmp.icmpfd, &readset);
	
	while (hop < env->maxhops)
	{
		try = 0;
		printf(" %d ", hop);
		while (try < env->maxtries)
		{
			timeout.tv_sec = 2;
			timeout.tv_usec = 0;
			if (!readonly)
				send_echo(env);
			ret = select (env->icmp.icmpfd + 1, &readset, NULL, NULL, &timeout);
			gettimeofday (&now, NULL);

			if (ret < 0)
				perror("Ret select: ");
			if (ret == 0)
			{
				printf(" *");
				fflush(stdout);
			}
			if (ret > 0)
			{
				printf("PACKET RECIEVED!");
			}
			try++;
		}
		printf("\n");
		ttl_advance(&env->icmp);
		hop++;
	}
	
	
	return;
}

void	ttl_advance (t_trace * icmp)
{
	const int *ttlp;

	ttlp = &icmp->ttl;
	icmp->ttl++;
	if (setsockopt (icmp->icmpfd, IPPROTO_IP, IP_TTL, ttlp, sizeof (*ttlp)) < 0)
		printf("setsockopt Error");
}

//================== ANJA TODO ======================
// indesign
// 3d Moddeling 
// animation 