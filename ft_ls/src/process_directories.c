#include "ft_ls.h"

/**
 * @brief Processes all directory arguments.
 *
 * @details Sorts the directory list according to the active options and
 * processes each directory using read_directory().
 *
 * When more than one explicit path was provided, or when files and
 * directories are mixed, the directory path is printed as a header before
 * its contents. The multiple argument counts the original command-line
 * operands, including paths that could not be classified. When the -R
 * option is enabled, directory headers are handled by read_directory().
 * The separator before a recursive directory header is deferred to
 * read_directory(), allowing it to be omitted if the directory cannot be
 * opened.
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
 * @param multiple    1 when more than one explicit path was provided.
 *
 * @return The highest error status encountered while processing directories.
 */
int	process_directories(t_ls_list *directories, t_ls_list *files, t_ls_flags options, int multiple)
{
	int			error;
	int			current_error;
	int			previous_directory;
	t_ls_entry	*current;

	error = 0;
	previous_directory = 0;
	sort_list(directories, options);
	current = directories->first;
	while (current != NULL)
	{
		if ((multiple || files->first != NULL) && options.R == 0)
			printf("%s:\n", current->path);
		current_error = read_directory(current->path, options, options.R == 1 && previous_directory);
		if (options.R == 1 && current_error != 2)
			previous_directory = 1;
		if (current_error > error)
			error = current_error;
		current = current->next;
		if (current != NULL && options.R == 0)
			printf("\n");
	}
	return (error);
}
