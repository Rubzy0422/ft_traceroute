/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 08:20:09 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/13 11:30:14 by rcoetzer         ###   ########.fr       */
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
	ssize_t size;
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
	size = sendto(env->icmp.icmpfd, &pckt.hdr, sizeof(pckt), 0,
		(struct sockaddr *)env->res->ai_addr, sizeof(struct sockaddr));
	if (PING_PKT_SIZE != size)
	{
		printf("%s: sendmsg: Network is unreachable %ld \n", P_NAME, size);
		perror("Message");
		return false;
	}
	return true;
}
