#include "ft_ls.h"

/**
 * @brief Processes all file and directory paths requested by the user.
 *
 * @details Initializes the file, directory, and error lists, then processes
 * the remaining command-line arguments after option parsing.
 *
 * If no explicit path is provided, the current directory is processed.
 * Otherwise, the provided paths are classified into files, directories,
 * and inaccessible paths before being processed.
 *
 * The function preserves the highest error status encountered:
 *
 * - 0: All paths were processed successfully.
 * - 1: A minor error occurred during processing.
 * - 2: A serious error occurred while processing an explicit path.
 *
 * All allocated error and entry lists are freed before returning.
 *
 * @param argc    Number of command-line arguments.
 * @param argv    Array containing the command-line arguments.
 * @param options Active ft_ls options.
 *
 * @return The highest error status encountered while processing the paths.
 */
int	process_paths(int argc, char **argv, t_ls_flags options)
{
	int				error;
	int				current_error;
	int				multiple;
	t_ls_error_list	errors;
	t_ls_list		files;
	t_ls_list		directories;

	errors.first = NULL;
	errors.last = NULL;
	files.first = NULL;
	files.last = NULL;
	directories.first = NULL;
	directories.last = NULL;
	error = 0;
	if (optind == argc)
	{
		current_error = read_directory(".", options, 0);
		if (current_error > error)
			error = current_error;
	}
	else
	{
		multiple = (argc - optind > 1);
		current_error = classify_paths(argc, argv, &files, &directories, &errors, &error, options);
		if (current_error > error)
			error = current_error;
		if (current_error == 0)
		{
			print_errors(errors.first);
			current_error = process_files(&files, options);
			if (current_error > error)
				error = current_error;
			if (files.first != NULL && directories.first != NULL)
				printf("\n");
			current_error = process_directories(&directories, &files, options, multiple);
			if (current_error > error)
				error = current_error;
		}
	}
	free_errors(&errors);
	free_entries(&files);
	free_entries(&directories);
	return (error);
}
