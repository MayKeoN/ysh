NAME	= ysh

CC		= gcc -std=c99

CFLAG	= -Wall -Werror -Wextra

SRCDIR 	= src/

SRC		= \
			ysh_main.c\
			ysh_init.c\
			ysh_print.c\
			ysh_prompt.c\
			ysh_cmd.c\
			ysh_utils.c

INC		= \
			-I include/\
			-I libft/include/

LIB		= \
			-L libft/ -lft

OBJ		= $(SRC:.c=.o)

OBJS	= $(addprefix $(SRCDIR),$(OBJ))

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAG) -o $(NAME) $(OBJS) $(LIB) $(INC)

%.o: %.c
	$(CC) $(CFLAG) $(INC) $(LIB) -o $@ -c $<

clean:	
	rm -rf $(OBJ)

fclean: 
	rm -rf $(OBJ)
	rm -f $(NAME)

re:	fclean all

.PHONY: clean, fclean, re




