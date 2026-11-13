#include "ft_ls.h"

/**
 * @brief Processes all file arguments.
 *
 * @details Sorts the file list according to the active options and processes
 * each file using read_file().
 *
 * When the -l option is enabled, the column widths required for long-format
 * output are calculated before printing the entries.
 *
 * When long-format output is disabled and the list contains files, a final
 * newline is printed after every file entry when long-format output is
 * disabled.
 *
 * The function preserves any error returned while processing the files.
 *
 * @param files   List containing the file arguments to process.
 * @param options Active ft_ls options.
 *
 * @return 0 on success, or 1 if an error occurs while processing a file.
 */
int	process_files(t_ls_list *files, t_ls_flags options)
{
	int			error;
	t_ls_widths	widths;
	t_ls_flags	width_options;
	t_ls_entry	*current;

	error = 0;
	widths.links = 1;
	widths.owner = 1;
	widths.group = 1;
	widths.size = 1;
	widths.device_major = 1;
	widths.device_minor = 1;
	sort_list(files, options);
	if (options.l == 1)
	{
		width_options = options;
		width_options.a = 1;
		widths = get_widths(files->first, width_options);
	}
	current = files->first;
	while (current != NULL)
	{
		if (read_file(current, options, widths) == 1)
			error = 1;
		if (options.l == 0)
			printf("\n");
		current = current->next;
	}
	return (error);
}
