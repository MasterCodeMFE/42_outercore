
NAME = ft_ls

OBJDIR = ./obj

SRC = ./src/main.c

OBJ = $(SRC:./src/%.c=$(OBJDIR)/%.o)

CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJDIR)/%.o: ./src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
