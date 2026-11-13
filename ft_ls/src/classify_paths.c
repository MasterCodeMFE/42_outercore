#include "ft_ls.h"

/**
 * @brief Classifies command-line paths into files, directories, and errors.
 *
 * @details Iterates through the non-option command-line arguments and
 * retrieves their metadata using lstat(). When -l is disabled, symlink
 * arguments that point to directories are classified as directories using
 * stat(), matching the behavior of ls when listing a symlink argument.
 *
 * Each path is classified as:
 *
 * - A directory, which is added to the directory list.
 * - A file or other non-directory entry, which is added to the file list.
 * - An inaccessible path, which is added to the error list.
 *
 * Errors associated with explicit command-line paths are considered serious
 * errors and set the program error status to 2.
 *
 * @param argc        Number of command-line arguments.
 * @param argv        Array containing the command-line arguments.
 * @param files       List where file entries are stored.
 * @param directories List where directory entries are stored.
 * @param errors      List where inaccessible paths are stored.
 * @param error       Current program error status.
 * @param options     Active ft_ls options.
 *
 * @return 0 on success, or 2 if an allocation or path processing error occurs.
 */
int	classify_paths(int argc, char **argv, t_ls_list *files, t_ls_list *directories, t_ls_error_list *errors, int *error, t_ls_flags options)
{
	int			i;
	struct stat	metadata;
	struct stat	target;

	i = optind;
	while (i < argc)
	{
		if (lstat(argv[i], &metadata) == -1)
		{
			if (add_error(errors, argv[i], errno) == 1)
				return (2);
			if (*error < 2)
				*error = 2;
		}
		else
		{
			if (options.l == 0 && S_ISLNK(metadata.st_mode) && stat(argv[i], &target) == 0 && S_ISDIR(target.st_mode))
				metadata = target;
			if (S_ISDIR(metadata.st_mode))
			{
				if (add_path(directories, argv[i], metadata) == 1)
					return (2);
			}
			else
			{
				if (add_path(files, argv[i], metadata) == 1)
					return (2);
			}
		}
		i++;
	}
	return (0);
}