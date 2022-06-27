# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kferterb <kferterb@student.21-school.ru    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/02 08:49:01 by kferterb          #+#    #+#              #
#    Updated: 2022/03/02 22:36:19 by kferterb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo

BONUS			=	philo_bonus

SRCS			=	main.c ft_simulate.c \

SRCS_BONUS		=	main_bonus.c ft_simulate_bonus.c ft_simulate_bonus2.c \

HEADER			=	philo.h

HEADER_BONUS	=	philo_bonus.h

OBJS			=	$(SRCS:%.c=%.o)

OBJS_BONUS		=	$(SRCS_BONUS:%.c=%.o)

CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror

.PHONY			:	all clean fclean re

all				:	$(NAME)

bonus			:	$(BONUS)

$(NAME)			:	$(OBJS) $(HEADER)
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(BONUS)		:	$(OBJS_BONUS) $(HEADER)
					$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(BONUS)

%.o 			:	%.c $(HEADER)
					$(CC) $(CFLAGS) -c $< -o $@

clean			:
					@rm -f $(OBJS) $(OBJS_BONUS)

fclean			:	clean
					@$(RM) $(NAME) $(BONUS) $(OBJS_BONUS)

re				:	fclean all