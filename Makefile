# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/15 18:06:44 by rcoetzer          #+#    #+#              #
#    Updated: 2020/10/15 18:23:00 by rcoetzer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ft_traceroute
CC			= 	gcc
FLAGS		=	-Wall -Wextra -Werror -g -MMD

SRCDIR		=	src/
OBJDIR		=	obj/

SRCFILES	=	ft_traceroute.c\
			addressinfo.c\
			packet.c\
			traffic.c\
			interrupt.c\
			time.c\
			ft_freeaddrinfo.c

SRCS		=	$(addprefix $(SRCDIR), $(SRCFILES))
OBJS		=	$(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

LIBFT_DIR	=	./libft/
LIBFT_LIB	=	$(LIBFT_DIR)libft.a

INCS		=	-I inc -I $(LIBFT_DIR)/includes

############################################## COLORS 

Black =  \u001b[30;1m
Red =  \u001b[31;1m
Green =  \u001b[32;1m
Yellow =  \u001b[33;1m
Blue =  \u001b[34;1m
Magenta =  \u001b[35;1m
Cyan =  \u001b[36;1m
White =  \u001b[37;1m
Reset = \u001b[0m


all: $(NAME)

$(LIBFT_LIB): $(LIBFT_DIR)
	@make -C $(LIBFT_DIR)

$(LIBFT_DIR):
	@git clone http://gihub.com/rubzy0422/libft.git 

$(NAME): $(LIBFT_LIB) $(OBJDIR) $(OBJS)
	@$(CC) -lm $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB)
	@echo COMPILED $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(dir $(OBJS))

$(OBJDIR)%.o : $(SRCDIR)%.c | $(OBJDIR)
	@$(CC) $(FLAGS) -c $< -o $@ $(INCS)
	@echo COMPILED $@

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJDIR)

fclean:		clean
	@make -C $(LIBFT_DIR) fclean
	@rm -rf $(NAME)

re:	fclean all

.PHONY: all clean fclean re