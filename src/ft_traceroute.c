/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 16:12:22 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/15 13:51:41 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_traceroute.h>

int main(int argc, char **argv)
{
	t_env env;
	
	env.hostname = parse_args(argc,argv);
	if (env.hostname == NULL)
		display_help();
	setup_env(&env);
	if (env.icmp.icmpfd < 0)
		printf("Socket could not be created");

	if (setsockopt (env.icmp.icmpfd, IPPROTO_IP, IP_TTL, 
	&env.icmp.ttl, sizeof (env.icmp.ttl)) < 0)
		printf("Could not prepare TTL socket!\n");

	// If socket setup success
	printf("%s to %s (%s), %d hops max, %d byte packets\n", 
	argv[0], env.hostname, env.hostaddr, env.maxhops, PING_PKT_SIZE);
	signal(SIGINT, InterruptHandler);
	
	sendloop(&env);
	ft_freeaddrinfo(env.res);
}

void	setup_env(t_env *env)
{
	if (getuid())
	{
		printf("You must be root to execute this program\n");
		exit(0);
	}
	env->pid = getpid() & 0xffff;
	env->res = dns_lookup(env, env->hostname);
	env->seqnum = -1;
	env->maxtries = 3;
	env->maxhops = 30;
	env->icmp.to = *(struct sockaddr_in *)env->res->ai_addr;
	env->icmp.ttl = 1;
	env->p_icmp.hostaddr = NULL;
	env->p_icmp.hostname = NULL;
	env->icmp.icmpfd = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);
}
char 	*parse_args(int argc, char **argv)
{
	int		i;
	int		x;
	char	*str_destAddr;
	
	i = 0;
	x = 0;
	str_destAddr = NULL;
	while (++i < argc)
		if (argv[i][x] == '-')
			while (argv[i][x++])
			{
				if (argv[i][x] == 'h')
					display_help();
				if (!ft_strchr("h",argv[i][x]))
					display_help();
			}
		else
			if (str_destAddr == NULL)
				str_destAddr = argv[i]; 
	return str_destAddr;
}

void	display_help()
{
	printf("TRACEROUTE\n\tprint the route packets trace to network host\n");
	printf("Usage:\n\t%s <destination> [options]\n", P_NAME);
	printf("Options:\n");
	printf("\t-h\tDisplay this help messge and exit\n");
	exit(0);
}