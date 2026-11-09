#include "ft_ls.h"

void	print_file_type(mode_t mode)
{
	if (S_ISREG(mode))
		printf("-");
	else if (S_ISDIR(mode))
		printf("d");
	else if (S_ISLNK(mode))
		printf("l");
	else if (S_ISCHR(mode))
		printf("c");
	else if (S_ISBLK(mode))
		printf("b");
	else if (S_ISFIFO(mode))
		printf("p");
	else if (S_ISSOCK(mode))
		printf("s");
	else
		printf("?");
}
