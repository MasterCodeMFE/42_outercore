#include "ft_ls.h"

int	main(int argc, char **argv)
{
    t_ls_flags options;
    parse_flags(argc, argv, &options);
    process_paths(argc, argv, options);
	return (0);
}