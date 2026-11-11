#include "ft_ls.h"

/**
 * @brief Prints a single file entry.
 *
 * @details Prints the provided file entry using print_entry().
 *
 * When the -l option is disabled, two spaces are printed after the file
 * name to separate it from the next file in the output.
 *
 * @param file    File entry to print.
 * @param options Active ft_ls options.
 * @param widths  Structure containing the column widths used for
 *                long-format output.
 *
 * @return 0 on success.
 */
int	read_file(t_ls_entry *file, t_ls_flags options, t_ls_widths widths)
{
	print_entry(file, options, widths);
	if (options.l == 0)
		printf("  ");
	return (0);
}
