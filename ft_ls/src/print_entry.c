#include "ft_ls.h"

/**
 * @brief Prints a single entry using the selected output format.
 *
 * @details Prints the entry name and, when the -l option is enabled,
 * prints the complete long-format information associated with the entry.
 *
 * Long-format output includes:
 *
 * - File type.
 * - File permissions.
 * - Number of hard links.
 * - Owner.
 * - Group.
 * - File size or device major and minor numbers.
 * - Modification date.
 * - Entry name.
 * - Symbolic link target, when applicable.
 *
 * When -l is disabled, only the entry name is printed.
 *
 * @param entry   Entry to print.
 * @param options Active ft_ls options.
 * @param widths  Column widths used to align long-format output.
 */
void	print_entry(t_ls_entry *entry, t_ls_flags options, t_ls_widths widths)
{
	if (options.l == 1)
	{
		print_file_type(entry->metadata.st_mode);
		print_permissions(entry->metadata.st_mode);
		printf(" %*lu ", widths.links,
			(unsigned long)entry->metadata.st_nlink);
		print_owner(entry->metadata.st_uid, widths.owner);
		print_group(entry->metadata.st_gid, widths.group);
		print_size(entry, widths);
		print_date(entry->metadata.st_mtime);
	}
	printf("%s", entry->name);
	if (options.l == 1 && S_ISLNK(entry->metadata.st_mode))
		print_symlink(entry->path);
	if (options.l == 1)
		printf("\n");
}
