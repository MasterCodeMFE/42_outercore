#include "ft_ls.h"

void	print_group(gid_t gid)
{
	struct group	*group;

	group = getgrgid(gid);
	if (group != NULL)
		printf("%s ", group->gr_name);
	else
		printf("%u ", (unsigned int)gid);
}
