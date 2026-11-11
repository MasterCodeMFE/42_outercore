#include "ft_ls.h"

/**
 * @brief Builds a full path from a directory path and an entry name.
 *
 * @details Allocates a new string containing the directory path followed
 * by the entry name.
 *
 * If the directory path does not already end with '/', a path separator
 * is added before appending the entry name.
 *
 * For example:
 *
 * directory = "/tmp/test"
 * name      = "file.txt"
 * result    = "/tmp/test/file.txt"
 *
 * @param directory Directory path used as the base path.
 * @param name      Name of the entry to append to the directory path.
 *
 * @return A newly allocated string containing the complete path,
 *         or NULL if memory allocation fails.
 */
char	*join_path(char *directory, char *name)
{
	char	*path;
	size_t	len;

	len = strlen(directory) + strlen(name) + 2;
	path = malloc(sizeof(char) * len);
	if (path == NULL)
		return (NULL);
	strcpy(path, directory);
	if (strlen(path) > 0 && path[strlen(path) - 1] != '/')
		strcat(path, "/");
	strcat(path, name);
	return (path);
}
