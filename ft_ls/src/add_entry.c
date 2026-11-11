#include "ft_ls.h"

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
 * If any memory allocation or lstat() operation fails, all previously
 * allocated resources are released before returning an error.
 *
 * @param entry     Directory entry returned by readdir().
 * @param list      List where the new entry will be appended.
 * @param directory Path of the directory containing the entry.
 *
 * @return 0 on success, or 1 if memory allocation or lstat() fails.
 */
int	add_entry(struct dirent *entry, t_ls_list *list, char *directory)
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
	if (lstat(node->path, &node->metadata) == -1)
	{
		free(node->path);
		free(node->name);
		free(node);
		return (1);
	}
	append_entry(list, node);
	return (0);
}
