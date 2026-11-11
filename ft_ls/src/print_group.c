#include "ft_ls.h"

/**
 * @brief Prints the group associated with a file.
 *
 * @details Resolves the group name associated with the provided GID using
 * getgrgid().
 *
 * If the group can be resolved, the group name is printed left-aligned
 * using the provided column width. Otherwise, the numeric GID is printed
 * instead.
 *
 * @param gid   Group identifier associated with the file.
 * @param width Width used to align the group column.
 */
void	print_group(gid_t gid, int width)
{
	struct group	*group;

	group = getgrgid(gid);
	if (group != NULL)
		printf("%-*s ", width, group->gr_name);
	else
		printf("%-*u ", width, (unsigned int)gid);
}
