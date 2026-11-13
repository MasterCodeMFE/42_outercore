#include "ft_ls.h"

/**
 * @brief Compares two entries according to the active sorting options.
 *
 * @details This function defines the ordering used by every merge operation.
 * It evaluates the options in the following order:
 *
 * - If -t is enabled, compare st_mtim.tv_sec first. A larger value means
 *   the entry is newer and must come first.
 * - If the seconds are equal, compare st_mtim.tv_nsec to distinguish
 *   modification times within the same second.
 * - If the timestamps are equal, or -t is disabled, compare the names with
 *   strcmp() so names are ordered alphabetically.
 * - If -r is enabled, reverse the sign of the comparison result. This
 *   reverses both the timestamp order and the alphabetical tie-breaker.
 *
 * For example, with -t an entry modified at 12:00 comes before one modified
 * at 11:00. With -tr, the 11:00 entry comes first. If two entries have the
 * same timestamp, their names decide their order; -r reverses that order
 * as well.
 *
 * This function only reads the entries and does not change their names,
 * paths, metadata, or links. Both pointers must refer to valid entries.
 *
 * @param first   First entry to compare. Its name must be set; its metadata
 *                is used when -t is enabled.
 * @param second  Second entry to compare. Its name must be set; its metadata
 *                is used when -t is enabled.
 * @param options Active options that select and modify the comparison order.
 *
 * @return A negative value if first must appear before second, a positive
 *         value if first must appear after second, or zero when neither
 *         entry precedes the other under the selected options. The exact
 *         magnitude is not significant; callers use only its sign.
 */
static int	compare_entries(t_ls_entry *first, t_ls_entry *second, t_ls_flags options)
{
	int	result;

	result = 0;
	if (options.t)
	{
		if (first->metadata.st_mtim.tv_sec > second->metadata.st_mtim.tv_sec)
			result = -1;
		else if (first->metadata.st_mtim.tv_sec < second->metadata.st_mtim.tv_sec)
			result = 1;
		else if (first->metadata.st_mtim.tv_nsec
			> second->metadata.st_mtim.tv_nsec)
			result = -1;
		else if (first->metadata.st_mtim.tv_nsec
			< second->metadata.st_mtim.tv_nsec)
			result = 1;
	}
	if (result == 0)
		result = strcmp(first->name, second->name);
	if (options.r)
	{
		if (result < 0)
			return (1);
		if (result > 0)
			return (-1);
	}
	return (result);
}

/**
 * @brief Merges two sorted entry lists into one sorted list.
 *
 * @details Both input lists must already be sorted with compare_entries()
 * using the same options passed to this function. The merge proceeds from
 * the heads of the lists and performs the following steps:
 *
 * - Compare the first unmerged entry from left with the first unmerged
 *   entry from right.
 * - Link the entry that sorts first to the output and advance that input
 *   list.
 * - If the entries compare equal, take the entry from left first. Since
 *   left precedes right in the original split, this preserves their relative
 *   order and makes the merge stable.
 * - Repeat until one input is empty, then link the other input's remaining
 *   nodes to the output. That remainder is already sorted.
 *
 * The local start node is a sentinel: tail initially points to it so the
 * first output node can be attached without a separate special case. The
 * sentinel itself is not part of either input and is never returned. Entry
 * nodes are not allocated, freed, or copied; only their next links change.
 *
 * The loop processes at most one node per iteration, and attaching the
 * remaining chain takes constant time. Merging n total entries therefore
 * takes O(n) time.
 *
 * @param left    Head of the first sorted input list; may be NULL.
 * @param right   Head of the second sorted input list; may be NULL.
 * @param options Sorting options used to order both input lists.
 *
 * @return The head of the merged list. Returns NULL when both inputs are
 *         empty.
 */
static t_ls_entry	*merge_entries(t_ls_entry *left, t_ls_entry *right, t_ls_flags options)
{
	t_ls_entry	start;
	t_ls_entry	*tail;

	start.next = NULL;
	tail = &start;
	while (left != NULL && right != NULL)
	{
		if (compare_entries(left, right, options) <= 0)
		{
			tail->next = left;
			left = left->next;
		}
		else
		{
			tail->next = right;
			right = right->next;
		}
		tail = tail->next;
	}
	if (left != NULL)
		tail->next = left;
	else
		tail->next = right;
	return (start.next);
}

/**
 * @brief Sorts an entry list using merge sort.
 *
 * @details The function uses the following recursive merge-sort steps:
 *
 * - Return immediately if the list has zero or one entry; such a list is
 *   already sorted.
 * - Find the midpoint with two pointers. slow advances one node per
 *   iteration while fast advances two, so slow stops at the end of the
 *   first half.
 * - Save the start of the second half and set slow->next to NULL to split
 *   the original chain into two independent lists.
 * - Recursively sort each half with the same options.
 * - Pass the two sorted halves to merge_entries() and return the merged
 *   head.
 *
 * For example, A -> B -> C -> D is split into A -> B and C -> D, each half
 * is sorted recursively, and the two results are merged. Each level does
 * O(n) work and halves the list, giving O(n log n) total time and O(log n)
 * recursive call depth. The algorithm reuses the existing nodes and
 * rearranges only their next links.
 *
 * @param first   Head of the list to sort; may be NULL.
 * @param options Sorting options passed to each comparison and merge.
 *
 * @return The head of the sorted list. Returns NULL if first is NULL.
 */
static t_ls_entry	*merge_sort_entries(t_ls_entry *first, t_ls_flags options)
{
	t_ls_entry	*slow;
	t_ls_entry	*fast;
	t_ls_entry	*right;

	if (first == NULL || first->next == NULL)
		return (first);
	slow = first;
	fast = first->next;
	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	right = slow->next;
	slow->next = NULL;
	first = merge_sort_entries(first, options);
	right = merge_sort_entries(right, options);
	return (merge_entries(first, right, options));
}

/**
 * @brief Sorts all entries in a list using the selected ordering.
 *
 * @details This is the public entry point used by the rest of ft_ls. It
 * delegates the actual ordering to merge_sort_entries(), which applies the
 * name, modification-time, and reverse-order rules selected by options.
 *
 * The function handles an empty list separately: first is already NULL, so
 * last is reset to NULL and the function returns. For a non-empty list, it
 * performs these operations:
 *
 * - Replace list->first with the head returned by merge_sort_entries().
 * - Traverse the sorted chain to find its final node.
 * - Store that node in list->last so future appends still use the correct
 *   tail.
 *
 * Sorting takes O(n log n) time. Finding the final node takes O(n), which
 * does not change the overall complexity. The nodes and their names, paths,
 * and metadata are reused; no entries are copied or allocated, and only
 * the next links are rearranged.
 *
 * @param list    Initialized list whose entries will be sorted.
 * @param options Active ft_ls options. The t member selects modification-
 *                time sorting; the r member reverses the selected order.
 */
void	sort_list(t_ls_list *list, t_ls_flags options)
{
	t_ls_entry	*entry;

	if (list->first == NULL)
	{
		list->last = NULL;
		return ;
	}
	list->first = merge_sort_entries(list->first, options);
	entry = list->first;
	while (entry->next != NULL)
		entry = entry->next;
	list->last = entry;
}
