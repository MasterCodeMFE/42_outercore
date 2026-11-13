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
 * @details Stores one entry and its place in a singly linked list.
 *
 * - name: Name shown to the user. For directory entries, this is only the
 *   component name, not the complete path.
 * - path: Path used for filesystem operations such as lstat(), readlink(),
 *   and opendir(). For directory contents it joins the parent path and name.
 * - metadata: struct stat returned by lstat(). If metadata_valid is zero,
 *   the structure is cleared except that st_mode may contain a type hint
 *   from readdir().
 * - metadata_valid: 1 when lstat() supplied complete metadata; 0 when
 *   metadata is unavailable and must not be used as if it were valid.
 * - next: Pointer to the next node, or NULL for the final node.
 */
typedef struct s_ls_entry
{
	char				*name;
	char				*path;
	struct stat			metadata;
	int					metadata_valid;
	struct s_ls_entry	*next;
}	t_ls_entry;

/**
 * @brief Represents a linked list of file system entries.
 *
 * @details Stores the endpoints of a singly linked list so append_entry()
 * can add a node in constant time.
 *
 * - first: Head node; NULL when the list is empty.
 * - last: Tail node; NULL when the list is empty. Its next pointer is NULL.
 */
typedef struct s_ls_list
{
	t_ls_entry	*first;
	t_ls_entry	*last;
}	t_ls_list;

/**
 * @brief Stores the command-line options supported by ft_ls.
 *
 * @details Each member is a boolean-style flag: 1 means enabled and 0 means
 * disabled. The flags represent the mandatory options:
 *
 * - l: Enables long-format output (-l).
 * - R: Enables recursive directory traversal (-R).
 * - a: Includes hidden entries (-a).
 * - r: Reverses the sorting order (-r).
 * - t: Sorts entries by modification time (-t).
 *
 * The structure records parsed intent; it does not store option arguments
 * because these five options take no values.
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
 * @details Stores one filesystem error and its place in the error list.
 *
 * - path: Allocated copy of the operand that could not be processed.
 * - error_num: errno value captured when the filesystem operation failed;
 *   print_errors() uses it to produce the diagnostic.
 * - next: Pointer to the next error node, or NULL for the last node.
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
 * @details Stores the endpoints of the error list so errors can be appended
 * in constant time and later printed in their original collection order.
 *
 * - first: Head error; NULL when there are no recorded errors.
 * - last: Tail error; NULL when there are no recorded errors. Its next
 *   pointer is NULL.
 */
typedef struct s_ls_error_list
{
	t_ls_error	*first;
	t_ls_error	*last;
}	t_ls_error_list;

/**
 * @brief Stores the widths used to align long-format output columns.
 *
 * @details Each member stores a character width computed from the entries
 * in one listing. These values control right-alignment in -l output.
 *
 * - links: Maximum number of characters needed for an entry's hard-link
 *   count.
 * - owner: Maximum displayed owner field width, using the account name or
 *   numeric UID fallback.
 * - group: Maximum displayed group field width, using the group name or
 *   numeric GID fallback.
 * - size: Maximum width of the normal size field. It is widened when needed
 *   to reserve space for a device's combined major/minor display.
 * - device_major: Maximum number of digits in a character or block device's
 *   major number.
 * - device_minor: Maximum number of digits in a character or block device's
 *   minor number.
 *
 * Device widths are separate because the two numbers are aligned
 * independently. All widths are at least 1.
 */
typedef struct s_ls_widths
{
	int	links;
	int	owner;
	int	group;
	int	size;
	int	device_major;
	int	device_minor;
}	t_ls_widths;

int				read_directory(char *directory, t_ls_flags options, int leading_blank);
int				add_entry(struct dirent *entry, t_ls_list *list, char *directory, t_ls_flags options);
char			*join_path(char *directory, char *name);
int				parse_flags(int argc, char **argv, t_ls_flags *flags);
int				process_paths(int argc, char **argv, t_ls_flags options);
void			free_entries(t_ls_list *list);
int				add_error(t_ls_error_list *list, char *path, int error_num);
void			print_errors(t_ls_error *error);
void			free_errors(t_ls_error_list *list);
int				add_path(t_ls_list *list, char *path, struct stat metadata);
int				read_file(t_ls_entry *file, t_ls_flags options, t_ls_widths widths);
void			print_permissions(mode_t mode);
void			print_owner(uid_t uid, int width);
void			print_group(gid_t gid, int width);
struct passwd	*lookup_user_by_uid(uid_t uid);
struct group	*lookup_group_by_gid(gid_t gid);
void			print_date(time_t mtime);
t_ls_widths		get_widths(t_ls_entry *list, t_ls_flags options);
void			print_names(t_ls_entry *list, t_ls_flags options, t_ls_widths widths);
void			sort_list(t_ls_list *list, t_ls_flags options);
int				classify_paths(int argc, char **argv, t_ls_list *files, t_ls_list *directories, t_ls_error_list *errors, int *error, t_ls_flags options);
int				process_files(t_ls_list *files, t_ls_flags options);
int				process_directories(t_ls_list *directories, t_ls_list *files, t_ls_flags options, int multiple);
void			print_file_type(mode_t mode);
void			print_symlink(char *path);
void			print_entry(t_ls_entry *entry, t_ls_flags options, t_ls_widths widths);
void			append_entry(t_ls_list *list, t_ls_entry *node);
void			print_total(t_ls_entry *list, t_ls_flags options);
void			print_size(t_ls_entry *entry, t_ls_widths widths);

#endif