#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/sysmacros.h>
# include <dirent.h>
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>

/**
 * @brief Represents a single file system entry.
 *
 * @details Stores the entry name, full path, file metadata, and a pointer
 * to the next entry in the linked list.
 */
typedef struct s_ls_entry
{
	char				*name;
	char				*path;
	struct stat			metadata;
	struct s_ls_entry	*next;
}	t_ls_entry;

/**
 * @brief Represents a linked list of file system entries.
 *
 * @details Stores pointers to the first and last entries to allow efficient
 * insertion at the end of the list.
 */
typedef struct s_ls_list
{
	t_ls_entry	*first;
	t_ls_entry	*last;
}	t_ls_list;

/**
 * @brief Stores the command-line options supported by ft_ls.
 *
 * @details Each member represents one mandatory option:
 *
 * - l: Enables long-format output (-l).
 * - R: Enables recursive directory traversal (-R).
 * - a: Includes hidden entries (-a).
 * - r: Reverses the sorting order (-r).
 * - t: Sorts entries by modification time (-t).
 */
typedef struct s_ls_flags
{
	int	l;
	int	R;
	int	a;
	int	r;
	int	t;
}	t_ls_flags;

/**
 * @brief Represents an error associated with a file system path.
 *
 * @details Stores the path that caused the error, the corresponding errno
 * value, and a pointer to the next error in the linked list.
 */
typedef struct s_ls_error
{
	char				*path;
	int					error_num;
	struct s_ls_error	*next;
}	t_ls_error;

/**
 * @brief Represents a linked list of errors.
 *
 * @details Stores pointers to the first and last errors to allow efficient
 * insertion at the end of the list.
 */
typedef struct s_ls_error_list
{
	t_ls_error	*first;
	t_ls_error	*last;
}	t_ls_error_list;

/**
 * @brief Stores the widths used to align long-format output columns.
 *
 * @details Each member contains the maximum width required by the
 * corresponding column when printing entries with the -l option.
 */
typedef struct s_ls_widths
{
	int	links;
	int	owner;
	int	group;
	int	size;
}	t_ls_widths;

int				read_directory(char *directory, t_ls_flags options);
int				add_entry(struct dirent *entry, t_ls_list *list, char *directory);
char			*join_path(char *directory, char *name);
void			sort_entries(t_ls_list *list);
int				parse_flags(int argc, char **argv, t_ls_flags *flags);
void			reverse_sort_entries(t_ls_list *list);
int				process_paths(int argc, char **argv, t_ls_flags options);
void			free_entries(t_ls_list *list);
void			time_sort_entries(t_ls_list *list);
void			reverse_time_sort_entries(t_ls_list *list);
int				add_error(t_ls_error_list *list, char *path, int error_num);
void			print_errors(t_ls_error *error);
void			free_errors(t_ls_error_list *list);
int				add_path(t_ls_list *list, char *path, struct stat metadata);
int				read_file(t_ls_entry *file, t_ls_flags options, t_ls_widths widths);
void			print_permissions(mode_t mode);
void			print_owner(uid_t uid, int width);
void			print_group(gid_t gid, int width);
void			print_date(time_t mtime);
t_ls_widths		get_widths(t_ls_entry *list, t_ls_flags options);
void			print_names(t_ls_entry *list, t_ls_flags options);
void			sort_list(t_ls_list *list, t_ls_flags options);
int				classify_paths(int argc, char **argv, t_ls_list *files, t_ls_list *directories, t_ls_error_list *errors, int *error);
int				process_files(t_ls_list *files, t_ls_flags options);
int				process_directories(t_ls_list *directories, t_ls_list *files, t_ls_flags options);
void			print_file_type(mode_t mode);
void			print_symlink(char *path);
void			print_entry(t_ls_entry *entry, t_ls_flags options, t_ls_widths widths);
void			append_entry(t_ls_list *list, t_ls_entry *node);
void			print_total(t_ls_entry *list, t_ls_flags options);
void			print_size(t_ls_entry *entry, t_ls_widths widths);

#endif