#include "ft_ls.h"

/**
 * @brief Parses the command-line options supported by ft_ls.
 *
 * @details Initializes all option flags and processes the command-line
 * arguments using getopt().
 *
 * Supported options:
 *
 * - -l: Enables long-format output.
 * - -R: Enables recursive directory traversal.
 * - -a: Includes hidden entries.
 * - -r: Reverses the sorting order.
 * - -t: Sorts entries by modification time.
 *
 * If getopt() detects an unsupported option, the function returns a
 * serious error status and the program stops before processing paths.
 *
 * @param argc  Number of command-line arguments.
 * @param argv  Array containing the command-line arguments.
 * @param flags Structure where the parsed options are stored.
 *
 * @return 0 on success, or 2 if an invalid option is found.
 */
int	parse_flags(int argc, char **argv, t_ls_flags *flags)
{
	int	option;

	flags->l = 0;
	flags->R = 0;
	flags->a = 0;
	flags->r = 0;
	flags->t = 0;
	option = getopt(argc, argv, "lRart");
	while (option != -1)
	{
		if (option == '?')
			return (2);
		if (option == 'l')
			flags->l = 1;
		if (option == 'R')
			flags->R = 1;
		if (option == 'a')
			flags->a = 1;
		if (option == 'r')
			flags->r = 1;
		if (option == 't')
			flags->t = 1;
		option = getopt(argc, argv, "lRart");
	}
	return (0);
}
