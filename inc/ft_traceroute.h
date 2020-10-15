#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <netinet/in.h> 
#include <netinet/ip_icmp.h>  
#include <fcntl.h> 
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <libft.h>

#define P_NAME "ft_traceroute"
# define NI_MAXHOST			1025
# define NI_MAXSERV			32
# define NI_NUMERICSERV 	2
#define PING_PKT_SIZE		60 

typedef enum {false , true} bool;

typedef struct		ping_pkt 
{
	struct icmphdr hdr; 
	char msg[PING_PKT_SIZE -sizeof(struct icmphdr)]; 
}					ping_pkt; 

typedef struct			s_trace
{
	int					icmpfd;
	struct sockaddr_in	to;
	struct sockaddr_in	from;
	int					ttl;
	struct timeval		tsent;
	ssize_t				size;
	char				*hostname;
	char				*hostaddr;
}						t_trace;

typedef struct		s_env
{
	char			*hostname;
	char			*hostaddr;
	struct addrinfo	*res;
	int				pid;
	t_trace			icmp;
	t_trace			p_icmp;
	int				seqnum;
	int				maxtries;
	int				maxhops;
	char			*str;
}					t_env;

void				setup_env(t_env *env);
char 				*parse_args(int argc, char **argv);
void				display_help();
struct addrinfo		*dns_lookup(t_env *env, const char *host);
char				*reverse_dns_lookup(char *s_ipv4_addr);
void				sendloop(t_env *env);
void				InterruptHandler();

void				send_probes(t_env *env, fd_set *readset, int hop);
int					send_echo(t_env *env);
int					read_echo(t_env *env, int try, int hop);

int					sameprevious(t_env *env);
void				setprevious(t_env *env);

unsigned short		checksum(void *buffer, int size);
void				ttl_advance(t_trace * icmp);

double				ft_timediff(struct timeval s, struct timeval e);
void				ft_freeaddrinfo(struct addrinfo *head);

#endif