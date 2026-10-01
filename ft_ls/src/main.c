#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void get_permissions(mode_t mode, char *str)
{
    // 1. Tipo de archivo
    str[0] = '-';
    if (S_ISDIR(mode))
        str[0] = 'd';
    else if (S_ISLNK(mode))
        str[0] = 'l';
    else if (S_ISCHR(mode))
        str[0] = 'c';
    else if (S_ISBLK(mode))
        str[0] = 'b';
    else if (S_ISFIFO(mode))
        str[0] = 'p';
    else if (S_ISSOCK(mode))
        str[0] = 's';

    // 2. Permisos del Propietario (User)
    if (mode & S_IRUSR)
        str[1] = 'r';
    else
        str[1] = '-';

    if (mode & S_IWUSR)
        str[2] = 'w';
    else
        str[2] = '-';

    if (mode & S_IXUSR)
        str[3] = 'x';
    else
        str[3] = '-';

    // 3. Permisos del Grupo (Group)
    if (mode & S_IRGRP)
        str[4] = 'r';
    else
        str[4] = '-';

    if (mode & S_IWGRP)
        str[5] = 'w';
    else
        str[5] = '-';

    if (mode & S_IXGRP)
        str[6] = 'x';
    else
        str[6] = '-';

    // 4. Permisos de Otros (Others)
    if (mode & S_IROTH)
        str[7] = 'r';
    else
        str[7] = '-';

    if (mode & S_IWOTH)
        str[8] = 'w';
    else
        str[8] = '-';

    if (mode & S_IXOTH)
        str[9] = 'x';
    else
        str[9] = '-';

    str[10] = '\0'; // Terminador de cadena obligatòrio
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    DIR* dir;
    struct dirent* read_dir;
    struct stat file_info;
    char mode_str[11];
    struct passwd *pw;
    struct group *gr;
    char time_str[80];
    struct tm *time_info;
    long total_blocks;

 
    dir = opendir("./");
    read_dir = NULL;
    pw = NULL;
    gr = NULL;
    time_info = NULL;
    if (!dir)
        return (1);
    total_blocks = 0;
while ((read_dir = readdir(dir)) != NULL)
    {
        if (read_dir->d_name[0] != '.') // Ignoramos todo lo que empiece por '.'
        {
            if (stat(read_dir->d_name, &file_info) == 0)
            {
                total_blocks += file_info.st_blocks;
            }
        }
    }
    printf("total %ld\n", total_blocks / 2);
    rewinddir(dir);
    while ((read_dir = readdir(dir)) != NULL)
    { 
        if((stat(read_dir->d_name, &file_info)) == 0)
        {
            get_permissions(file_info.st_mode, mode_str);
            pw = getpwuid(file_info.st_uid);
            gr = getgrgid(file_info.st_gid);
            time_info = localtime(&(file_info.st_mtime));
            strftime(time_str, sizeof(time_str), "%b %d %H:%M", time_info);
            //if(read_dir->d_name[0] != '.' || strcmp(read_dir->d_name, ".") == 0 || strcmp(read_dir->d_name, "..") == 0)
            if (read_dir->d_name[0] != '.')
                printf("%s %lu %s %s %8lld %s    %s\n", mode_str, (unsigned long)file_info.st_nlink, pw->pw_name, gr->gr_name, (long long int)file_info.st_size, time_str, read_dir->d_name);
        }
    }
    closedir(dir);
    return (0);
}