#include "ft_ls.h"

/**
 * @brief Reads all entries from an open directory.
 *
 * @details Iterates through the directory stream using readdir() and adds
 * each visible entry to the provided list. When -a is disabled, hidden
 * entries are skipped before add_entry() so they are not looked up or
 * reported as errors even though they will not be printed.
 *
 * add_entry() returns one of three statuses:
 *
 * - 0 when the entry was added successfully.
 * - 1 when an allocation fails; reading stops and a serious error is
 *   returned.
 * - 2 when lstat() fails; the entry is either retained with unknown
 *   metadata for EACCES or skipped for other errors, and a minor error is
 *   remembered.
 *
 * @param open_directory Open directory stream returned by opendir().
 * @param list           List where successfully read entries are stored.
 * @param directory      Path of the directory being read.
 * @param options        Active ft_ls options, used to filter hidden entries.
 *
 * @return 0 if all entries were read, 1 if one or more entries could not be
 *         inspected, or 2 if memory allocation failed.
 */
static int	read_entries(DIR *open_directory, t_ls_list *list, char *directory, t_ls_flags options)
{
	struct dirent	*entry;
	int				result;
	int				minor;

	minor = 0;
	entry = readdir(open_directory);
	while (entry != NULL)
	{
		if (options.a == 0 && entry->d_name[0] == '.')
		{
			entry = readdir(open_directory);
			continue ;
		}
		result = add_entry(entry, list, directory, options);
		if (result == 1)
			return (2);
		if (result == 2)
			minor = 1;
		entry = readdir(open_directory);
	}
	return (minor);
}

/**
 * @brief Determines whether an entry can be processed recursively.
 *
 * @details An entry can be processed recursively only when all the following
 * conditions are met:
 *
 * - The entry is identified as a directory by lstat() or, when lstat()
 *   fails with EACCES, by the d_type hint supplied by readdir().
 * - The entry is not ".".
 * - The entry is not "..".
 * - The entry is not hidden, unless the -a option is enabled.
 *
 * Excluding "." and ".." prevents infinite recursive traversal. A directory
 * type hint is used only to attempt opendir(); the recursive open still
 * determines whether the directory can actually be processed.
 *
 * @param entry   Directory entry to evaluate.
 * @param options Active ft_ls options.
 *
 * @return 1 if the entry can be processed recursively, otherwise 0.
 */
static int	can_recurse(t_ls_entry *entry, t_ls_flags options)
{
	if (!S_ISDIR(entry->metadata.st_mode))
		return (0);
	if (strcmp(entry->name, ".") == 0)
		return (0);
	if (strcmp(entry->name, "..") == 0)
		return (0);
	if (options.a == 0 && entry->name[0] == '.')
		return (0);
	return (1);
}

static int	read_dir(char *directory, t_ls_flags options, int leading_blank);

/**
 * @brief Processes eligible subdirectories recursively.
 *
 * @details Iterates through the directory entry list and recursively
 * processes every eligible subdirectory using read_dir(). The nested call
 * prints its header only after successfully opening the subdirectory, so a
 * failed opendir() does not leave an extra blank line in the output.
 *
 * Errors found while processing subdirectories are treated as minor errors
 * because the failing paths were discovered during recursive traversal
 * instead of being provided explicitly as command-line arguments.
 *
 * Processing continues with the remaining entries even if one subdirectory
 * cannot be opened.
 *
 * @param list    List containing the directory entries.
 * @param options Active ft_ls options.
 * @return 0 if all subdirectories are processed successfully,
 *         otherwise 1 if a recursive error occurs.
 */
static int	recurse_directories(t_ls_list *list, t_ls_flags options)
{
	int			error;
	int			current_error;
	t_ls_entry	*current;

	error = 0;
	current = list->first;
	while (current != NULL)
	{
		if (can_recurse(current, options) == 1)
		{
			current_error = read_dir(current->path, options, 1);
			if (current_error != 0)
				error = 1;
		}
		current = current->next;
	}
	return (error);
}

/**
 * @brief Reads, sorts, prints, and recursively processes a directory.
 *
 * @details Opens the requested directory and stores its entries in a
 * temporary linked list. The header's leading blank line is emitted only
 * after the directory has been opened and its entries have been read.
 *
 * After reading the directory, the entries are:
 *
 * - Sorted according to the active options.
 * - Printed using the selected output format.
 * - Preceded by the total allocated block count when -l is enabled.
 * - Processed recursively when -R is enabled.
 *
 * When -R is enabled, the directory path is printed as a header before its
 * contents. For nested directories, a separating blank line is included in
 * that header, but only after opendir() succeeds.
 *
 * If opendir() fails, an error message is written to stderr and the
 * function returns a serious error status. If lstat() fails with EACCES,
 * the name is retained; long format displays unknown metadata and the
 * remaining entries are still sorted and printed. Other lstat() failures
 * are reported and skipped. A memory allocation failure aborts processing.
 * Errors encountered while recursively processing discovered
 * subdirectories are converted into minor errors by recurse_directories().
 *
 * All entries allocated while reading the directory are released before
 * the function returns.
 *
 * @param directory Path of the directory to process.
 * @param options   Active ft_ls options.
 * @param leading_blank 1 to separate this header from earlier directory
 *                       output, otherwise 0.
 * @return 0 on success, 1 for a recursive or entry metadata error, or 2
 *         for a serious directory processing error.
 */
static int	read_dir(char *directory, t_ls_flags options, int leading_blank)
{
	DIR			*open_directory;
	t_ls_list	list;
	int			error;
	t_ls_widths	widths;

	list.first = NULL;
	list.last = NULL;
	error = 0;
	open_directory = opendir(directory);
	if (open_directory == NULL)
	{
		fprintf(stderr, "ft_ls: cannot open directory '%s': %s\n", directory, strerror(errno));
		return (2);
	}
	error = read_entries(open_directory, &list, directory, options);
	if (error == 2)
	{
		closedir(open_directory);
		free_entries(&list);
		return (2);
	}
	closedir(open_directory);
	sort_list(&list, options);
	widths.links = 1;
	widths.owner = 1;
	widths.group = 1;
	widths.size = 1;
	widths.device_major = 1;
	widths.device_minor = 1;
	if (options.l == 1)
		widths = get_widths(list.first, options);
	if (options.R == 1)
		printf("%s%s:\n", leading_blank ? "\n" : "", directory);
	if (options.l == 1)
		print_total(list.first, options);
	print_names(list.first, options, widths);
	if (options.R == 1 && recurse_directories(&list, options) != 0)
		error = 1;
	free_entries(&list);
	return (error);
}

/**
 * @brief Processes a top-level directory.
 *
 * @details Starts directory processing with the requested header spacing.
 * Discovered subdirectories are handled internally by read_dir().
 *
 * @param directory Path of the directory to process.
 * @param options   Active ft_ls options.
 * @param leading_blank 1 to separate this header from earlier output.
 *
 * Column widths are calculated independently for each directory listing.
 * @return 0 on success, 1 for a minor error, or 2 for a serious error.
 */
int	read_directory(char *directory, t_ls_flags options, int leading_blank)
{
	return (read_dir(directory, options, leading_blank));
}
