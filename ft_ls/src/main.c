#include "ft_ls.h"

/**
 * @brief Entry point of the ft_ls program.
 *
 * @details Initializes the program options, parses the command-line
 * arguments, and processes the requested files and directories.
 *
 * The function performs the following operations:
 *
 * - Parses the mandatory ft_ls options.
 * - Stops execution immediately if option parsing fails.
 * - Processes the remaining file and directory arguments.
 * - Returns the error status produced during execution.
 *
 * Exit status values:
 *
 * - 0: Successful execution.
 * - 1: Minor error found during processing.
 * - 2: Serious error, such as an invalid option or inaccessible
 *      command-line argument.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array containing the command-line arguments.
 *
 * @return The final ft_ls exit status.
 */
int	main(int argc, char **argv)
{
	t_ls_flags	options;
	int			error;

	error = parse_flags(argc, argv, &options);
	if (error != 0)
		return (error);
	return (process_paths(argc, argv, options));
}
