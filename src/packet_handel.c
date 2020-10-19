/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_handel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 14:08:56 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 16:23:33 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

void handel_ttl(struct timeval *tvrecv, char *ipaddr, t_env *env)
{
		int getinfosucess;
		double rtt;

		rtt = time_sub(tvrecv, &env->tvsend);
		
		//rDNS
		getinfosucess = getnameinfo(env->pr.sa_recv, env->pr.sa_len,
		env->host_name, sizeof(env->host_name), NULL, 0, 0);
		if(ft_strcmp(env->last,ipaddr)!=0 )
		 {
			if(getinfosucess == 0)
				printf(" %s (%s)", env->host_name, ipaddr);
			else
				printf(" %s (%s)", ipaddr,ipaddr);
			ft_strcpy(env->last,ipaddr);
		}
		printf("   %.3f ms", rtt);
}

void handel_echo_reply(t_env *env, struct icmp *icmp, int icmplen, char *ipaddr,
						struct timeval *tvrecv)
{
	int getinfosucess;
	double rtt;
	
	if (icmp->icmp_id != env->pid)
			return;
		if (icmplen < 16)
		{
			printf("icmplen (%d) < 16", icmplen);
			exit(0);
		}
		env->dest_reached = 1;
		rtt = time_sub(tvrecv, &env->tvsend);
		getinfosucess = getnameinfo(env->pr.sa_recv, env->pr.sa_len,
		env->host_name, sizeof(env->host_name), NULL, 0, 0);
		if(ft_strcmp(env->last, getipaddr(env->pr.sa_recv, env->host_name)) !=0)
		{
			if(getinfosucess == 0)
				printf(" %s (%s)", env->host_name, ipaddr);
			else
				printf (" %s (%s)", ipaddr,ipaddr);
			ft_strcpy(env->last, ipaddr);
		}
		printf ("   %.3f ms", rtt);
}

void handel_local(struct timeval *tvrecv, struct icmp *icmp, char *ipaddr, 
					t_env *env)
{
		int getinfosucess;
		double rtt;

		if (!env->check)
		{
			printf(" localhost (127.0.0.1)");
			env->check = 1;
		}
		rtt = time_sub(tvrecv, &env->tvsend);
		getinfosucess = getnameinfo(env->pr.sa_recv, env->pr.sa_len,
		env->host_name, sizeof(env->host_name), NULL, 0, 0);
		if(ft_strcmp(env->last,ipaddr)!=0 )
		 {
			if(getinfosucess == 0)
				printf(" %s (%s)", env->host_name, ipaddr);
			else
				printf(" %s (%s)", ipaddr,ipaddr);
			if (icmp->icmp_type != 8)
				ft_strcpy(env->last,ipaddr);
		}
		printf("   %.3f ms", rtt);
}

void handel_other(struct timeval *tvrecv, struct icmp *icmp, char *ipaddr,
				 t_env *env)
{
	if (!ft_strcmp(ipaddr, "127.0.0.1"))
			handel_local(tvrecv,icmp,ipaddr,env);
	else
	{
		printf("Invalid packet type:%d code: %d",
		icmp->icmp_type, icmp->icmp_code);
	}
}

void process_packet(char *ptr, ssize_t len, struct timeval *tvrecv, t_env *env)
{
	int hlen1;
	int icmplen;
	struct ip *ip;
	struct icmp *icmp;

	ip = (struct ip *) ptr;
	hlen1 = ip->ip_hl << 2;
	icmp = (struct icmp *)(ptr + hlen1);
	char *ipaddr = getipaddr(env->pr.sa_recv,env->host_name);
	icmplen = len - hlen1;
	if (icmplen <= 8)
	{
		printf("icmplen (%d) < 8", icmplen);
		exit(0);
	}
	if(icmp->icmp_type == ICMP_ECHOREPLY)
		handel_echo_reply(env, icmp, icmplen, ipaddr, tvrecv);
	else
		if(icmp->icmp_type==ICMP_TIME_EXCEEDED || icmp->icmp_type==ICMP_EXC_TTL)
			handel_ttl(tvrecv, ipaddr, env);
	else
		handel_other(tvrecv, icmp, ipaddr, env);
}
