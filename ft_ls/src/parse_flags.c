#include "ft_ls.h"

int	parse_flags(int argc, char **argv, t_ls_flags *flags)
{
    int option;
    flags->l = 0;
    flags->R = 0;
    flags->a = 0;
    flags->r = 0;
    flags->t = 0;
    option = getopt(argc, argv, "lRart");
    while (option != -1)
    {
        if (option == 'l')
        {
           flags->l = 1; 
        }
        if (option == 'R')
        {
           flags->R = 1; 
        }
        if (option == 'a')
        {
           flags->a = 1; 
        }
        if (option == 'r')
        {
           flags->r = 1; 
        }
        if (option == 't')
        {
           flags->t = 1; 
        }
        option = getopt(argc, argv, "lRart");
    }
    return(0);
}