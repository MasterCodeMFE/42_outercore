#include "ft_ls.h"

/**
 * @brief Derives a file-type hint from the directory entry.
 *
 * @details Some filesystems provide d_type even when lstat() cannot inspect
 * the entry because the process lacks search permission on its parent.
 * The hint is used only to preserve the type character in long-format
 * output and to decide whether recursive opendir() should be attempted.
 * It is not treated as complete metadata.
 *
 * @param entry Directory entry returned by readdir().
 *
 * @return Corresponding S_IF* mode bits, or 0 if the type is unavailable.
 */
static mode_t	entry_type_hint(struct dirent *entry)
{
#ifdef DT_DIR
	if (entry->d_type == DT_DIR)
		return (S_IFDIR);
#endif
#ifdef DT_REG
	if (entry->d_type == DT_REG)
		return (S_IFREG);
#endif
#ifdef DT_LNK
	if (entry->d_type == DT_LNK)
		return (S_IFLNK);
#endif
#ifdef DT_CHR
	if (entry->d_type == DT_CHR)
		return (S_IFCHR);
#endif
#ifdef DT_BLK
	if (entry->d_type == DT_BLK)
		return (S_IFBLK);
#endif
#ifdef DT_FIFO
	if (entry->d_type == DT_FIFO)
		return (S_IFIFO);
#endif
#ifdef DT_SOCK
	if (entry->d_type == DT_SOCK)
		return (S_IFSOCK);
#endif
	return (0);
}

/**
 * @brief Creates and appends a new entry to the directory entry list.
 *
 * @details Allocates a new t_ls_entry node and initializes it with the
 * information obtained from the directory entry.
 *
 * The function performs the following operations:
 *
 * - Allocates memory for a new t_ls_entry node.
 * - Copies the entry name returned by readdir().
 * - Builds the complete path using join_path().
 * - Retrieves the entry metadata using lstat().
 * - Appends the initialized node to the provided list.
 *
 * If memory allocation fails, all previously allocated resources are
 * released and the function returns a fatal error. If lstat() fails with
 * EACCES, the name and path are retained with invalid, zeroed metadata.
 * Long-format output can then display the name and unknown metadata, while
 * normal output can list the name without reporting an unnecessary error.
 *
 * Other lstat() failures are reported and skipped. A minor error is returned
 * so the caller can continue reading the directory.
 *
 * @param entry     Directory entry returned by readdir().
 * @param list      List where the new entry will be appended.
 * @param directory Path of the directory containing the entry.
 * @param options   Active ft_ls options.
 *
 * @return 0 if the entry was added without a reportable error, 1 if memory
 *         allocation fails, or 2 if lstat() fails and an error is reported.
 */
int	add_entry(struct dirent *entry, t_ls_list *list, char *directory, t_ls_flags options)
{
	t_ls_entry	*node;

	node = malloc(sizeof(t_ls_entry));
	if (node == NULL)
		return (1);
	node->name = strdup(entry->d_name);
	if (node->name == NULL)
	{
		free(node);
		return (1);
	}
	node->path = join_path(directory, entry->d_name);
	if (node->path == NULL)
	{
		free(node->name);
		free(node);
		return (1);
	}
	node->metadata_valid = 0;
	if (lstat(node->path, &node->metadata) == -1)
	{
		if (errno == EACCES)
		{
			memset(&node->metadata, 0, sizeof(node->metadata));
			node->metadata.st_mode = entry_type_hint(entry);
			if (options.l == 1)
				fprintf(stderr, "ft_ls: cannot access '%s': %s\n", node->path, strerror(errno));
			append_entry(list, node);
			if (options.l == 1)
				return (2);
			return (0);
		}
		fprintf(stderr, "ft_ls: cannot access '%s': %s\n", node->path, strerror(errno));
		free(node->path);
		free(node->name);
		free(node);
		return (2);
	}
	node->metadata_valid = 1;
	append_entry(list, node);
	return (0);
}
