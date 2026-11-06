#include "ft_ls.h"

int	read_file(t_ls_entry *archivo, t_ls_flags options, int size_width)
{
	if (options.l == 1)
	{
		print_permissions(archivo->metadata.st_mode);
		printf(" %lu ", (unsigned long)archivo->metadata.st_nlink);
		print_owner(archivo->metadata.st_uid);
		print_group(archivo->metadata.st_gid);
		printf("%*lld ", size_width,
			(long long)archivo->metadata.st_size);
		print_date(archivo->metadata.st_mtime);
	}
	printf("%s", archivo->nombre);
	if (options.l == 1)
		printf("\n");
	else
		printf("  ");
	return (0);
}