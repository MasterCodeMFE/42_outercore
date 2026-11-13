NAME = ft_ls

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./include

SRC = $(SRCDIR)/main.c \
	  $(SRCDIR)/add_entry.c \
	  $(SRCDIR)/read_directory.c \
	  $(SRCDIR)/join_path.c \
	  $(SRCDIR)/parse_flags.c \
	  $(SRCDIR)/process_paths.c \
	  $(SRCDIR)/free_entries.c \
	  $(SRCDIR)/print_errors.c \
	  $(SRCDIR)/add_error.c \
	  $(SRCDIR)/free_errors.c \
	  $(SRCDIR)/add_path.c \
	  $(SRCDIR)/read_file.c \
	  $(SRCDIR)/print_permissions.c \
	  $(SRCDIR)/print_owner.c \
	  $(SRCDIR)/print_group.c \
	  $(SRCDIR)/print_date.c \
	  $(SRCDIR)/get_widths.c \
	  $(SRCDIR)/print_names.c \
	  $(SRCDIR)/sort_list.c \
	  $(SRCDIR)/classify_paths.c \
      $(SRCDIR)/process_files.c \
      $(SRCDIR)/process_directories.c \
	  $(SRCDIR)/print_file_type.c \
	  $(SRCDIR)/print_symlink.c \
	  $(SRCDIR)/print_entry.c \
	  $(SRCDIR)/append_entry.c \
	  $(SRCDIR)/print_total.c \
	  $(SRCDIR)/print_size.c \



OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/ft_ls.h | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re