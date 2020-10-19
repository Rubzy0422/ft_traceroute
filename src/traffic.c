/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traffic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 13:02:37 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 16:24:47 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ft_traceroute.h>

void recv_packet(t_env *env, fd_set *readset, struct timeval *timeout)
{
	int ret;
	struct timeval recv_tval;
	ssize_t recvsize;
	
	ret = select(env->sockfd + 1, readset, NULL, NULL, timeout);
	if (ret < 0)
		ft_error("Ret select error", ret);
	if (ret == 0)
		printf(" *");
	if (ret > 0)
	{
		if (FD_ISSET(env->sockfd, readset))
		{
			recvsize = recvfrom(env->sockfd, env->recvbuf,
			sizeof(env->recvbuf), 0, env->pr.sa_recv, &env->pr.sa_len);
			if (recvsize < 0)
				ft_error("recvfrom error\n", recvsize);
			gettimeofday(&recv_tval, NULL);
			process_packet(env->recvbuf, recvsize, &recv_tval, env);
		}	
	}
}

void pingloop(t_env *env)
{
	int size;
	fd_set readset;

	size = 0;
	env->sockfd = socket(env->pr.sa_send->sa_family, SOCK_RAW, IPPROTO_ICMP);
	if (env->sockfd < 0)
		ft_error("Could not create socket", env->sockfd);
	env->sockset = setsockopt(env->sockfd,SOL_SOCKET,SO_RCVBUF,&size,
							sizeof(size));
	if (env->sockset < 0)
		ft_error("Could not create socket", env->sockset);
	FD_ZERO (&readset);
	FD_SET(env->sockfd, &readset);
	struct timeval timeout;
	while(!env->dest_reached && env->ttl <= env->maxhop){
		printf(" %d ", env->ttl);
		env->count = 0;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		while(env->count++ < env->maxtries){	
			send_packet(env);
			recv_packet(env, &readset, &timeout);
		}
		env->ttl++;
		printf("\n");
	}
}