#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <sys/wait.h>

#include "process_regular_file.c"
#include "print_access_rights.c"

#define BUFFER_SIZE 256

int pid_options, pid_process_file;

void regular_file_logic(char file_path[], struct stat file_stat)
{
    char buffer[256] = "";
    char options[3];

    printf("\nSelect an option: ");
    scanf("%s", options);
     
    if (!strstr(options, "-n") && !strstr(options, "-d") && !strstr(options, "-h") && 
    !strstr(options, "-m") && !strstr(options, "-a") && !strstr(options, "-l"))
    {
            printf("\nInvalid option!\n");
            printf("\n------MENU------\n\n");
            printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");
            regular_file_logic(file_path, file_stat);
    }
        else
        {
        if((pid_options=fork())==0)
        {

            if (strstr(options, "-n"))
            {
                printf("File name: %s\n", file_path);
            }
            else if (strstr(options, "-d"))
            {
                printf("File size: %ld bytes\n", file_stat.st_size);
            }
            else if (strstr(options, "-h"))
            {
                printf("Hard link count: %ld\n", (long)file_stat.st_nlink);
            }
            else if (strstr(options, "-m"))
            {
                printf("Time of last modification: %s", ctime(&file_stat.st_mtime));
            }
            else if (strstr(options, "-a"))
            {
                print_access_rights(file_stat);
            }
            else if (strstr(options, "-l"))
            {
                printf("\nEnter symlink name: ");
                char symlink_name[BUFFER_SIZE];
                scanf("%s", symlink_name);

                if(symlink(file_path, symlink_name)==-1){
                    perror("symlink");
                }
                printf("Symlink points to: %s\n", file_path);
            }

            exit(pid_options);

        }

        if(pid_options>0)
        {
            char pid_str[10];
            int status;
            waitpid(pid_options, &status, 0);
            printf("\nThe process child ended with PID: %d with exit code %d.\n\n", pid_options, WEXITSTATUS(status));
        }

        if((pid_process_file=fork())==0)
        {
            process_regular_file(file_path);
            exit(pid_process_file);
        }

        if(pid_process_file>0)
        {
            char pid_str[10];
            int status;
            waitpid(pid_process_file, &status, 0);
            printf("The process child ended with PID: %d with exit code %d.\n\n", pid_process_file, WEXITSTATUS(status));
        }
    }

}
