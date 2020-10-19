/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 11:54:50 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 15:25:17 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

u_short checksum(u_short *b, int len)
{
	unsigned short *buf = b; 
	unsigned int sum; 
	unsigned short result; 

	sum = 0;
	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++; 
	if ( len == 1 ) 
		sum += *(unsigned char*)buf; 
	sum = (sum >> 16) + (sum & 0xFFFF); 
	sum += (sum >> 16); 
	result = ~sum; 
	return result; 
}

void send_packet(t_env *env)
{
	int len;
	struct icmp *icmp;

	ft_bzero(env->sendbuf, sizeof(env->sendbuf));
	icmp = (struct icmp *) env->sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = env->pid;
	icmp->icmp_seq = env->nsent++;
	gettimeofday((struct timeval *) icmp->icmp_data, NULL);
	gettimeofday(&env->tvsend,NULL);
	len = 8 + env->datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *)icmp, len); 
	setsockopt (env->sockfd, IPPROTO_IP, IP_TTL, &env->ttl, sizeof (env->ttl));
	sendto(env->sockfd, env->sendbuf,len, 0, env->pr.sa_send, env->pr.sa_len);
}