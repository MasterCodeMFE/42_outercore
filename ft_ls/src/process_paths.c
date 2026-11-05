#include "ft_ls.h"

int	process_paths(int argc, char **argv, t_ls_flags options)
{
    int i;
    i = optind;
    if(i == argc)
    {
        if (read_directory(".", options) == 1)
		    return (1);
    }
    else
    {
        while (i < argc)
        {
            if(options.R == 0 && argc - optind > 1)
                printf("%s:\n", argv[i]);
            if (read_directory(argv[i], options) == 1)
		        return (1);
            i ++;
            if (i < argc)
                printf("\n");
        }
    }
    return 0;
}