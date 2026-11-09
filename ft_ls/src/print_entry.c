#include "ft_ls.h"

void	print_entry(t_ls_entry *entry, t_ls_flags options, int size_width)
{
	if (options.l == 1)
	{
        print_file_type(entry->metadata.st_mode);
		print_permissions(entry->metadata.st_mode);
		printf(" %lu ", (unsigned long)entry->metadata.st_nlink);
		print_owner(entry->metadata.st_uid);
		print_group(entry->metadata.st_gid);
		printf("%*lld ", size_width,
			(long long)entry->metadata.st_size);
		print_date(entry->metadata.st_mtime);
	}
	printf("%s", entry->nombre);
	if (options.l == 1 && S_ISLNK(entry->metadata.st_mode))
		print_symlink(entry->ruta);
	if (options.l == 1)
		printf("\n");
}
