#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <unistd.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <netinet/in_systm.h>
#include <netinet/ip.h> 
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <string.h> 
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <errno.h>
#include <libft.h>

#define P_NAME "ft_traceroute"
#define BUFSIZE		1025
#define NI_MAXHOST	1025

#define u_int	unsigned int
#define u_short unsigned short

typedef struct		s_proto
{
	struct sockaddr	*sa_send;
	struct sockaddr	*sa_recv;
	socklen_t		sa_len;
	int				icmpproto;
}					t_proto;

typedef __sighandler_t sighandler_t;

typedef struct		s_env
{
	int 			errcode;
	int				flag;
	pid_t			pid;
	int				nsent;
	int				count;
	int				datalen;
	int				sockfd;
	int				sockset;
	char			recvbuf[BUFSIZE];
	char			sendbuf[BUFSIZE];
	int 			maxtries;
	int				ttl;
	int				maxhop;
	int				dest_reached;
	char			last[128];
	char			host_name[NI_MAXHOST];
	char			*host;
	struct timeval	tvsend;
	int				check;
	t_proto			pr;
}					t_env;


/* ========== packet ======================================================== */
void				send_packet(t_env *);
unsigned short		checksum(unsigned short *addr, int len);

/* ========== packet_handel.c =============================================== */
void 				handel_ttl(struct timeval *tvrecv, char *ipaddr,
					t_env *env);
void				handel_echo_reply(t_env *env, struct icmp *icmp, 
					int icmplen, char *ipaddr, struct timeval *tvrecv);
void				handel_other(struct timeval *tvrecv, struct icmp *icmp, 
					char *ipaddr, t_env *env);
void				process_packet(char *ptr, ssize_t len,
					struct timeval *tvrecv, t_env *env);


/* ========== Addrinfo ====================================================== */
struct addrinfo		*get_host_ai(const char *host, const char *serv, int family,
					int socktype);
char 				*getipaddr(const struct sockaddr *sa, char *hostname);

/* ========== MAIN ========================================================== */
int					main(int argc, char **argv);
void				sig_alrm();
void				setupenv(t_env *env);
char 				*parse_args(int argc, char **argv);
void				display_help(void);

/* ========== utils ========================================================= */
void				ft_error(char *msg, int errcode);
void				*ft_calloc(size_t blocks, size_t size);

/* ========== traffic ======================================================= */
void				pingloop(t_env *env);

/* ================== time ================================================== */
double				time_sub(struct timeval *out, struct timeval *in);
/* ================== ft_freeaddrinfo ======================================= */
void				ft_freeaddrinfo(struct addrinfo *head);

#endif