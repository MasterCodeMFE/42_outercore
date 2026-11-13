#include "ft_ls.h"

/**
 * @brief Prints a single file entry.
 *
 * @details Prints the provided file entry using print_entry().
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
	return (0);
}
