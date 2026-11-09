#include "ft_ls.h"

int	main(int argc, char **argv)
{
    t_ls_flags options;
    if (parse_flags(argc, argv, &options) != 0)
		return (1);
	return (process_paths(argc, argv, options));
}
