#include "ft_ls.h"

/**
 * @brief Swaps the data stored in two adjacent entries.
 *
 * @details Exchanges the name, path, and metadata of the provided entry
 * with the data stored in the following entry.
 *
 * The linked-list nodes and their next pointers are not moved. Only the
 * data stored inside the adjacent nodes is exchanged.
 *
 * @param entry Entry whose data will be swapped with the next entry.
 */
static void	swap_entries(t_ls_entry *entry)
{
	char		*name;
	char		*path;
	struct stat	metadata;

	name = entry->name;
	path = entry->path;
	metadata = entry->metadata;
	entry->name = entry->next->name;
	entry->path = entry->next->path;
	entry->metadata = entry->next->metadata;
	entry->next->name = name;
	entry->next->path = path;
	entry->next->metadata = metadata;
}

/**
 * @brief Sorts entries alphabetically in ascending order.
 *
 * @details Compares adjacent entry names using strcmp(). When two entries
 * are in the wrong order, their stored data is exchanged and the traversal
 * restarts from the beginning of the list.
 *
 * This produces the default alphabetical order used by ft_ls.
 *
 * @param list List containing the entries to sort.
 */
void	sort_entries(t_ls_list *list)
{
	t_ls_entry	*entry;

	entry = list->first;
	while (entry != NULL)
	{
		if (entry->next != NULL
			&& strcmp(entry->name, entry->next->name) > 0)
		{
			swap_entries(entry);
			entry = list->first;
			continue ;
		}
		entry = entry->next;
	}
}

/**
 * @brief Sorts entries alphabetically in descending order.
 *
 * @details Compares adjacent entry names using strcmp(). When two entries
 * are not in reverse alphabetical order, their stored data is exchanged
 * and the traversal restarts from the beginning of the list.
 *
 * This sorting order is used when the -r option is enabled without -t.
 *
 * @param list List containing the entries to sort.
 */
void	reverse_sort_entries(t_ls_list *list)
{
	t_ls_entry	*entry;

	entry = list->first;
	while (entry != NULL)
	{
		if (entry->next != NULL
			&& strcmp(entry->name, entry->next->name) < 0)
		{
			swap_entries(entry);
			entry = list->first;
			continue ;
		}
		entry = entry->next;
	}
}

/**
 * @brief Compares two entries by modification time.
 *
 * @details Compares the modification timestamps of two entries using both
 * seconds and nanoseconds.
 *
 * The most recently modified entry is ordered first. If both timestamps
 * are identical, the entry names are compared alphabetically using
 * strcmp() as a tie-breaker.
 *
 * @param first  First entry to compare.
 * @param second Second entry to compare.
 *
 * @return A negative value if the first entry must appear before the second,
 *         a positive value if the first entry must appear after the second,
 *         or zero if both entries are equivalent.
 */
static int	compare_time(t_ls_entry *first, t_ls_entry *second)
{
	if (first->metadata.st_mtim.tv_sec > second->metadata.st_mtim.tv_sec)
		return (-1);
	if (first->metadata.st_mtim.tv_sec < second->metadata.st_mtim.tv_sec)
		return (1);
	if (first->metadata.st_mtim.tv_nsec > second->metadata.st_mtim.tv_nsec)
		return (-1);
	if (first->metadata.st_mtim.tv_nsec < second->metadata.st_mtim.tv_nsec)
		return (1);
	return (strcmp(first->name, second->name));
}

/**
 * @brief Sorts entries by modification time.
 *
 * @details Orders entries from the most recently modified to the oldest,
 * using both seconds and nanoseconds from st_mtim.
 *
 * If two entries have the same modification timestamp, their names are
 * used as an alphabetical tie-breaker.
 *
 * This sorting order is used when the -t option is enabled.
 *
 * @param list List containing the entries to sort.
 */
void	time_sort_entries(t_ls_list *list)
{
	t_ls_entry	*entry;

	entry = list->first;
	while (entry != NULL)
	{
		if (entry->next != NULL
			&& compare_time(entry, entry->next) > 0)
		{
			swap_entries(entry);
			entry = list->first;
			continue ;
		}
		entry = entry->next;
	}
}

/**
 * @brief Sorts entries by modification time in reverse order.
 *
 * @details Orders entries from the oldest modification time to the most
 * recent, using both seconds and nanoseconds from st_mtim.
 *
 * Entries with identical timestamps are ordered by name in reverse
 * alphabetical order.
 *
 * This sorting order is used when the -t and -r options are enabled
 * together.
 *
 * @param list List containing the entries to sort.
 */
void	reverse_time_sort_entries(t_ls_list *list)
{
	t_ls_entry	*entry;

	entry = list->first;
	while (entry != NULL)
	{
		if (entry->next != NULL
			&& compare_time(entry, entry->next) < 0)
		{
			swap_entries(entry);
			entry = list->first;
			continue ;
		}
		entry = entry->next;
	}
}
