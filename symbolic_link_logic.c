#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

int pid_options, pid_process_sym;

void symbolic_link_logic(int argc, char symlink_path[])
{
    struct stat symlink_stat;
    char options[3];

    printf("\nSelect an option: ");
    scanf("%s", options);
    if (strlen(options) == 2)
    {
        if (!strstr(options, "-n") && !strstr(options, "-l") && !strstr(options, "-d") &&
            !strstr(options, "-t") && !strstr(options, "-a"))
        {
            printf("\nInvalid option!\n");
            printf("\n------MENU------\n\n");
            printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");
            symbolic_link_logic(argc, symlink_path);
        }
        else
        {
            if ((pid_options = fork()) == 0)
            {
                if (strstr(options, "-n"))
                {
                    printf("\nSymbolic link name: %s\n", symlink_path);
                }
                else if (strstr(options, "-l"))
                {
                    if (unlink(symlink_path) == -1)
                    {
                        printf("Error deleting symbolic link: %s\n", strerror(errno));
                        return;
                    }

                    printf("\nSymbolic link deleted successfully\n");
                    return;
                }
                else if (stat(symlink_path, &symlink_stat) == -1)
                {
                    printf("Error: Unable to get symbolic link status\n");
                    return;
                }
                else if (strstr(options, "-d"))
                {
                    printf("\nSymbolic link size: %ld bytes\n", symlink_stat.st_size);
                }
                else if (strstr(options, "-t"))
                {
                    char resolved_path[250];
                    if (realpath(symlink_path, resolved_path) == NULL)
                    {
                        printf("Error: Unable to get resolved path\n");
                        return;
                    }

                    struct stat target_stat;
                    if (stat(resolved_path, &target_stat) == -1)
                    {
                        printf("Error: Unable to get target file status\n");
                        return;
                    }

                    printf("\nTarget file size: %ld bytes\n", target_stat.st_size);
                }
                else if (strstr(options, "-a"))
                {
                    print_access_rights(symlink_stat);
                }
                exit(pid_options);
            }

            if (pid_options > 0)
            {
                int status;
                waitpid(pid_options, &status, 0);
                printf("\nThe process child ended with PID: %d with exit code %d.\n", pid_options, WEXITSTATUS(status));
            }

            if ((pid_process_sym = fork()) == 0)
            {
                struct stat symlink_stat;
                if (stat(symlink_path, &symlink_stat) == -1)
                {
                    printf("Error: Unable to get symbolic link status\n");
                    exit(1);
                }

                symlink_stat.st_mode |= S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP;
                symlink_stat.st_mode &= ~(S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);

                if (chmod(symlink_path, symlink_stat.st_mode) == -1)
                {
                    printf("Error changing permissions of the symbolic link\n");
                    exit(1);
                }

                exit(0);
            }

            if (pid_process_sym > 0)
            {
                int status;
                waitpid(pid_process_sym, &status, 0);
                printf("\nThe process child ended with PID: %d with exit code %d.\n\n\n", pid_process_sym, WEXITSTATUS(status));
            }
        }
    }
    else
    {
        printf("\nInvalid option!\n");
        printf("\n------MENU------\n\n");
        printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");
        symbolic_link_logic(argc, symlink_path);
    }
}