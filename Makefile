# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/15 12:47:14 by rcoetzer          #+#    #+#              #
#    Updated: 2020/10/15 13:49:42 by rcoetzer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_traceroute

SRC_PATH = ./src/
FLAGS = -g


SRC_NAME =	ft_traceroute.c\
			addressinfo.c\
			packet.c\
			traffic.c\
			interrupt.c\
			time.c\
			ft_freeaddrinfo.c

LIBFT_PATH = ./libft/

INCLUDES =	-I ./inc/ \
			-I $(LIBFT_PATH)/includes/
				   
LIBS = $(LIBFT_PATH)libft.a
CC = gcc

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
SRCO = $(patsubst %.c, %.o, $(SRC))

all:$(LIBFT_PATH)libft.a $(NAME) 

$(NAME): $(SRCO)
	@$(CC) $(FLAGS) $(SRCO) $(LIBS) -o $(NAME)
	@printf "\e[32mBinary \e[1;32m$(NAME)\e[1;0m\e[32m Created.\e[0m\n"


$(LIBFT_PATH):
	@git clone http://github.com/rubzy0422/libft 

$(LIBFT_PATH)libft.a: $(LIBFT_PATH) 
	@make -C $(LIBFT_PATH)
	
$(SRC_PATH)%.o: $(SRC_PATH)%.c 
	@gcc $(FLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@/bin/rm -rf $(SRCO)
	@printf "\e[31mObjects Files \e[1;31m$(OBJS_LIST)\e[1;0m\e[31mRemoved.\e[0m\n"

fclean: clean
	@make -C $(LIBFT_PATH)/ fclean
	@/bin/rm -rf $(NAME)
	@printf "\e[31mBin \e[1;31m$(NAME)\e[1;0m\e[31m Removed.\e[0m\n"

re: fclean all

.PHONY: all fclean clean re LIBFT VECLIB init destroy 