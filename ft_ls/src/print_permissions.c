#include "ft_ls.h"

static void	print_user_permissions(mode_t mode)
{
	if (mode & S_IRUSR)
		printf("r");
	else
		printf("-");
	if (mode & S_IWUSR)
		printf("w");
	else
		printf("-");
	if (mode & S_ISUID)
	{
		if (mode & S_IXUSR)
			printf("s");
		else
			printf("S");
	}
	else if (mode & S_IXUSR)
		printf("x");
	else
		printf("-");
}

static void	print_group_permissions(mode_t mode)
{
	if (mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if (mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if (mode & S_ISGID)
	{
		if (mode & S_IXGRP)
			printf("s");
		else
			printf("S");
	}
	else if (mode & S_IXGRP)
		printf("x");
	else
		printf("-");
}

static void	print_other_permissions(mode_t mode)
{
	if (mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if (mode & S_IWOTH)
		printf("w");
	else
		printf("-");
	if (mode & S_ISVTX)
	{
		if (mode & S_IXOTH)
			printf("t");
		else
			printf("T");
	}
	else if (mode & S_IXOTH)
		printf("x");
	else
		printf("-");
}

void	print_permissions(mode_t mode)
{
	print_user_permissions(mode);
	print_group_permissions(mode);
	print_other_permissions(mode);
}
