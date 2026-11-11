#include "ft_ls.h"

/**
 * @brief Reads all entries from an open directory.
 *
 * @details Iterates through the directory stream using readdir() and adds
 * each discovered entry to the provided list.
 *
 * The add_entry() function stores the entry name, complete path, and file
 * metadata. If an entry cannot be added, the function stops processing and
 * returns a serious error status.
 *
 * @param open_directory Open directory stream returned by opendir().
 * @param list           List where the directory entries will be stored.
 * @param directory      Path of the directory being read.
 *
 * @return 0 on success, or 2 if an entry cannot be added.
 */
static int	read_entries(DIR *open_directory, t_ls_list *list,
		char *directory)
{
	struct dirent	*entry;

	entry = readdir(open_directory);
	while (entry != NULL)
	{
		if (add_entry(entry, list, directory) == 1)
			return (2);
		entry = readdir(open_directory);
	}
	return (0);
}

/**
 * @brief Determines whether an entry can be processed recursively.
 *
 * @details An entry can be processed recursively only when all the following
 * conditions are met:
 *
 * - The entry is a directory.
 * - The entry is not ".".
 * - The entry is not "..".
 * - The entry is not hidden, unless the -a option is enabled.
 *
 * Excluding "." and ".." prevents infinite recursive traversal.
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

/**
 * @brief Processes eligible subdirectories recursively.
 *
 * @details Iterates through the directory entry list and recursively
 * processes every eligible subdirectory using read_directory().
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
 *
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
			printf("\n");
			current_error = read_directory(current->path, options);
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
 * temporary linked list.
 *
 * After reading the directory, the entries are:
 *
 * - Sorted according to the active options.
 * - Printed using the selected output format.
 * - Preceded by the total allocated block count when -l is enabled.
 * - Processed recursively when -R is enabled.
 *
 * When -R is enabled, the directory path is printed as a header before
 * its contents.
 *
 * If opendir() fails, an error message is written to stderr and the
 * function returns a serious error status. Errors encountered while
 * recursively processing discovered subdirectories are converted into
 * minor errors by recurse_directories().
 *
 * All entries allocated while reading the directory are released before
 * the function returns.
 *
 * @param directory Path of the directory to process.
 * @param options   Active ft_ls options.
 *
 * @return 0 on success, 1 for a recursive error, or 2 for a serious
 *         directory processing error.
 */
int	read_directory(char *directory, t_ls_flags options)
{
	DIR			*open_directory;
	t_ls_list	list;
	int			error;

	list.first = NULL;
	list.last = NULL;
	error = 0;
	open_directory = opendir(directory);
	if (open_directory == NULL)
	{
		fprintf(stderr, "ft_ls: cannot open directory '%s': %s\n",
			directory, strerror(errno));
		return (2);
	}
	error = read_entries(open_directory, &list, directory);
	if (error != 0)
	{
		closedir(open_directory);
		free_entries(&list);
		return (error);
	}
	closedir(open_directory);
	sort_list(&list, options);
	if (options.R == 1)
		printf("%s:\n", directory);
	if (options.l == 1)
		print_total(list.first, options);
	print_names(list.first, options);
	if (options.R == 1)
		error = recurse_directories(&list, options);
	free_entries(&list);
	return (error);
}
