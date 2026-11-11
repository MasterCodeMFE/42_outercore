#include "ft_ls.h"

/**
 * @brief Processes all directory arguments.
 *
 * @details Sorts the directory list according to the active options and
 * processes each directory using read_directory().
 *
 * When multiple directories are provided, or when files and directories
 * are mixed, the directory path is printed as a header before its contents.
 * When the -R option is enabled, directory headers are handled directly by
 * read_directory().
 *
 * The function preserves the highest error status returned while processing
 * the directories:
 *
 * - 0: All directories were processed successfully.
 * - 1: A minor error occurred during recursive processing.
 * - 2: A serious error occurred while processing an explicit directory.
 *
 * @param directories List containing the directory arguments to process.
 * @param files       List containing the file arguments.
 * @param options     Active ft_ls options.
 *
 * @return The highest error status encountered while processing directories.
 */
int	process_directories(t_ls_list *directories, t_ls_list *files,
		t_ls_flags options)
{
	int			error;
	int			current_error;
	t_ls_entry	*current;

	error = 0;
	sort_list(directories, options);
	current = directories->first;
	while (current != NULL)
	{
		if ((directories->first != directories->last
				|| files->first != NULL)
			&& options.R == 0)
			printf("%s:\n", current->path);
		current_error = read_directory(current->path, options);
		if (current_error > error)
			error = current_error;
		current = current->next;
		if (current != NULL)
			printf("\n");
	}
	return (error);
}
