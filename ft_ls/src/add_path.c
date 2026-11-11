#include "ft_ls.h"

/**
 * @brief Creates and appends a path entry to a list.
 *
 * @details Allocates a new t_ls_entry node and initializes it using the
 * provided path and metadata.
 *
 * The function performs the following operations:
 *
 * - Allocates memory for a new t_ls_entry node.
 * - Stores a copy of the path as the entry name.
 * - Stores a copy of the complete path.
 * - Assigns the provided file metadata.
 * - Appends the initialized node to the provided list.
 *
 * If a memory allocation fails, all previously allocated resources are
 * released before returning an error.
 *
 * @param list     List where the new entry will be appended.
 * @param path     Path used to initialize the entry.
 * @param metadata File metadata associated with the path.
 *
 * @return 0 on success, or 1 if a memory allocation fails.
 */
int	add_path(t_ls_list *list, char *path, struct stat metadata)
{
	t_ls_entry	*node;

	node = malloc(sizeof(t_ls_entry));
	if (node == NULL)
		return (1);
	node->name = strdup(path);
	if (node->name == NULL)
	{
		free(node);
		return (1);
	}
	node->path = strdup(path);
	if (node->path == NULL)
	{
		free(node->name);
		free(node);
		return (1);
	}
	node->metadata = metadata;
	append_entry(list, node);
	return (0);
}
