#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nbrucker <nbrucker@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/02 19:25:43 by nbrucker          #+#    #+#              #
#    Updated: 2017/12/30 16:03:24 by nbrucker         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = libft.a

INCSDIR = includes/

SRCSDIR = srcs/

OBJSDIR = objs/

SRCS = 	ft_putchar.c \
		ft_putstr.c \
		ft_putendl.c \
		ft_strcmp.c \
		ft_memset.c \
		ft_bzero.c \
		ft_strdup.c \
		ft_strlen.c \
		ft_memcpy.c

OBJS = $(addprefix $(OBJSDIR),$(SRCS:.c=.o))

$(OBJSDIR)%.o: $(SRCSDIR)%.c
	@mkdir -p $(OBJSDIR)
	$(CC) $(CFLAGS) -I $(INCSDIR) -o $@ -c $<

CC = gcc

CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
	rm -rf $(OBJSDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
