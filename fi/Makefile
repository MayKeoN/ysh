NAME	= FillIt

CC		= gcc -std=c99

CFLAG	= 
#-Wall -Werror -Wextra

SRCDIR 	= src/

SRC		= \
			fi_main.c\
			fi_init.c\
			fi_parse.c\
			fi_display.c\
			fi_solve.c

INC		= \
			-I inc/\
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





