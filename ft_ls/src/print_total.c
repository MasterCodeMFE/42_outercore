#include "ft_ls.h"

/**
 * @brief Prints the total number of allocated blocks for visible entries.
 *
 * @details Iterates through the entry list and adds the number of allocated
 * blocks stored in st_blocks for each visible entry.
 *
 * Hidden entries are excluded unless the -a option is enabled.
 *
 * The st_blocks field is expressed in 512-byte blocks, while the default
 * total displayed by ls represents 1024-byte blocks. Therefore, the final
 * value is divided by 2 before being printed.
 *
 * @param list    List of entries whose allocated blocks will be calculated.
 * @param options Active ft_ls options.
 */
void	print_total(t_ls_entry *list, t_ls_flags options)
{
	long long	total;

	total = 0;
	while (list != NULL)
	{
		if (!(list->name[0] == '.' && options.a == 0))
			total += (long long)list->metadata.st_blocks;
		list = list->next;
	}
	printf("total %lld\n", total / 2);
}
