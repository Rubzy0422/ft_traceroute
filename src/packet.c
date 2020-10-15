/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 08:20:09 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/15 14:45:23 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

unsigned short checksum(void *b, int len)
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

int	send_echo(t_env *env)
{
	struct ping_pkt pckt;
	int i;
	ft_bzero(&pckt, sizeof(pckt)); 
	pckt.hdr.type = ICMP_ECHO;
	pckt.hdr.code = 0;
	pckt.hdr.un.echo.id = env->pid; 
	i = 0;
	while(i++ < (int)sizeof(pckt.msg)-1) 
		pckt.msg[i] = i+'0';
	pckt.msg[i] = 0;
	pckt.hdr.un.echo.sequence = ++env->seqnum;
	pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
	gettimeofday(&env->icmp.tsent, NULL);
	env->icmp.size = sendto(env->icmp.icmpfd, &pckt.hdr, sizeof(pckt), 0,
		(struct sockaddr *)env->res->ai_addr, sizeof(struct sockaddr));
	if (PING_PKT_SIZE != env->icmp.size)
	{
		printf("\nconnect: Network is unreachable\n");
		exit(-1);
		return false;
	}
	return true;
}

int	read_echo(t_env *env, int try)
{
	char buf[PING_PKT_SIZE];
	struct sockaddr_in r_addr;
	unsigned int addr_len = sizeof(r_addr);
	struct timeval now;

	gettimeofday(&now, NULL);
	double triptime = ft_timediff(env->icmp.tsent, now);
	if (recvfrom(env->icmp.icmpfd, &buf, sizeof(buf), 0, (struct sockaddr *)&r_addr, &addr_len)) 
	{
		env->icmp.hostaddr = inet_ntoa(r_addr.sin_addr);
		env->icmp.hostname = reverse_dns_lookup(env->icmp.hostaddr);
		
		if (try == 0)
		{
			printf(" %s (%s)  ", env->icmp.hostname, env->icmp.hostaddr);
		}
		struct ping_pkt * pckt = (struct ping_pkt *)((void *)buf + sizeof(struct ip ));
		if (pckt->hdr.type == ICMP_TIME_EXCEEDED || pckt->hdr.type == ICMP_HOST_UNREACH || pckt->hdr.type == 0 || pckt->hdr.type == 8)
		{
			printf("  %.3f ms", triptime);
		}

		//VALIDATE ICMP ECHO REPLY || HOST_UNREACH
		//printf("BUFF:\n\n==START==\n\n%s\n\n==END==\n\n", buf);

	}
	else
		perror("Recvfrom");
	return 0;
}

int		sameprevious(t_env *env)
{
	if (env->icmp.hostaddr && env->p_icmp.hostaddr)
		if (!ft_strcmp(env->p_icmp.hostaddr, env->icmp.hostaddr))
		{
			printf("STOP!");
			return true;	
		}
	return false;
}

void	setprevious(t_env *env)
{
	if (env->p_icmp.hostname)
	{
		free(env->p_icmp.hostname);
		free(env->p_icmp.hostaddr);
	}
	env->p_icmp.hostname = ft_strdup(env->icmp.hostname);
	env->p_icmp.hostaddr = ft_strdup(env->icmp.hostaddr);
}

