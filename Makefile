NAME = norme

CC = gcc

CFLAGS = -Wall -Werror -Wextra

NAME_NM = ft_nm

NAME_OTOOL = ft_otool

LIBFTDIR = libft/

INCSDIR = includes/

SRCSDIR = srcs/

OBJSDIR = objs/

DIR_NM = nm/

DIR_OTOOL = otool/

SRCS_NM =	main.c \
			error.c

SRCS_OTOOL = main.c

HDRS_NM = nm.h

HDRS_OTOOL = otool.h

OBJS_NM = $(addprefix $(DIR_NM)$(OBJSDIR),$(SRCS_NM:.c=.o))

OBJS_OTOOL = $(addprefix $(DIR_OTOOL)$(OBJSDIR),$(SRCS_OTOOL:.c=.o))

$(DIR_NM)$(OBJSDIR)%.o: $(DIR_NM)$(SRCSDIR)%.c $(addprefix $(DIR_NM)$(INCSDIR),$(HDRS_NM))
	@mkdir -p $(DIR_NM)$(OBJSDIR)
	$(CC) $(CFLAGS) -I $(DIR_NM)$(INCSDIR) -I $(LIBFTDIR)$(INCSDIR) -o $@ -c $<

$(DIR_OTOOL)$(OBJSDIR)%.o: $(DIR_OTOOL)$(SRCSDIR)%.c $(addprefix $(DIR_OTOOL)$(INCSDIR),$(HDRS_OTOOL))
	@mkdir -p $(DIR_OTOOL)$(OBJSDIR)
	$(CC) $(CFLAGS) -I $(DIR_OTOOL)$(INCSDIR) -I $(LIBFTDIR)$(INCSDIR) -o $@ -c $<

all: $(NAME)

$(NAME): $(NAME_NM) $(NAME_OTOOL)

$(NAME_NM): $(OBJS_NM)
	make -C $(LIBFTDIR)
	$(CC) $(CFLAGS) -o $(NAME_NM) $(OBJS_NM) -L./$(LIBFTDIR) -lft

$(NAME_OTOOL): $(OBJS_OTOOL)
	make -C $(LIBFTDIR)
	$(CC) $(CFLAGS) -o $(NAME_OTOOL) $(OBJS_OTOOL) -L./$(LIBFTDIR) -lft

clean:
	rm -rf $(DIR_NM)$(OBJSDIR)
	rm -rf $(DIR_OTOOL)$(OBJSDIR)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -rf $(NAME_NM)
	rm -rf $(NAME_OTOOL)
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re