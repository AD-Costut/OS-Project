#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

void files_menu(int argc, char *dir_path[])
{
    struct stat file_stat;

    for (int i = 1; i < argc; i++)
    {
        if (lstat(dir_path[i], &file_stat) != -1) {

            if (S_ISREG(file_stat.st_mode))
            {
                printf("%s is a regular file\n", dir_path[i]);
                printf("\n------MENU------\n\n");
                printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");
                regular_file_logic(dir_path[i], file_stat);
            }
            else if (S_ISDIR(file_stat.st_mode))
            {
                printf("%s is a directory\n", dir_path[i]);
                printf("\n------MENU------\n\n");
                printf("name (-n)\nsize (-d)\nfiles with .c extension (-c)\naccess rights (-a)\n");
                dir_logic(dir_path[i]);
            }
            else if (S_ISLNK(file_stat.st_mode))
            {
                printf("%s is a symbolic link\n", dir_path[i]);
                printf("\n------MENU------\n\n");
                printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");
                symbolic_link_logic(dir_path[i]);
            }
        }
    }
}