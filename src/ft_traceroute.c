/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 13:02:20 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/10/19 16:16:26 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ft_traceroute.h>

void	ft_free(t_env *env)
{
	free(env->host);
	free(env->pr.sa_recv);
}

int		main(int argc, char **argv)
{
	struct addrinfo	*ai;
	t_env			env;
	char			*ipaddr;
	
	env.host= ft_strdup(parse_args(argc,argv));
	if (!env.host)
		display_help();
	setupenv(&env);
	signal(SIGALRM, sig_alrm);
	ai = get_host_ai(env.host, NULL, 0, 0);
	ipaddr = getipaddr(ai->ai_addr, env.host_name);
	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
	ai->ai_canonname, ipaddr, env.maxhop, env.datalen);
	env.pr.sa_send = ai->ai_addr;
	ft_strcpy(env.last, ipaddr);
	env.pr.sa_recv = ft_calloc(1, ai->ai_addrlen);
	env.pr.sa_len = ai->ai_addrlen;
	pingloop(&env);
	ft_freeaddrinfo(ai);
	ft_free(&env);
	return 0;
}

void	setupenv(t_env *env)
{
	if (getuid())
		ft_error("You must be root to execute this program\n", 0);
	env->errcode = 0;
	env->maxtries = 3;
	env->flag=0;
	env->nsent =0;
	env->count=0;
	env->datalen = 60;
	env->ttl=1;
	env->maxhop=30;
	env->dest_reached=0;
	env->pid = getpid();
	env->check = 0;
}

void	sig_alrm()
{
	printf(" *");
	alarm(0);
}

void	display_help(void)
{
	printf("%s\n\tprint the route packets trace to network host\n", P_NAME);
	printf("Usage:\n\t%s <destination> [options]\n", P_NAME);
	printf("Options:\n");
	printf("\t-h\tDisplay this help messge and exit\n");
	exit(0);
}

char 	*parse_args(int argc, char **argv)
{
	int		i;
	int		x;
	char	*host;
	
	i = 0;
	x = 0;
	host = NULL;
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
			if (host == NULL)
				host = argv[i]; 
	return host;
}

