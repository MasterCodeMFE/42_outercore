#include "ft_ls.h"

void	print_permissions(mode_t mode)
{
	if (S_ISDIR(mode))
		printf("d");
	else if (S_ISLNK(mode))
		printf("l");
	else
		printf("-");

	if (mode & S_IRUSR)
		printf("r");
	else
		printf("-");
	if (mode & S_IWUSR)
		printf("w");
	else
		printf("-");
	if (mode & S_IXUSR)
		printf("x");
	else
		printf("-");

	if (mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if (mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if (mode & S_IXGRP)
		printf("x");
	else
		printf("-");

	if (mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if (mode & S_IWOTH)
		printf("w");
	else
		printf("-");
	if (mode & S_IXOTH)
		printf("x");
	else
		printf("-");
}