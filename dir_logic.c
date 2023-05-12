#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>


int pid_options, pid_process_dir;

void dir_logic(char dir_path[])
{
    struct stat dir_stat;
    char options[3];

    printf("\nSelect an option: ");
    scanf("%2s", options);
    
    if (!strstr(options, "-n") && !strstr(options, "-d") && !strstr(options, "-a") && !strstr(options, "-c"))
    {
     printf("\nInvalid option!\n");
     printf("\n------MENU------\n\n");
     printf("name (-n)\nsize (-d)\nfiles with .c extension (-c)\naccess rights (-a)\n");
     dir_logic(dir_path);
    }
        else
        { 
            if((pid_options=fork())==0)
            {
                if (strstr(options, "-n"))
                {
                    printf("\nDirectory name: %s\n", dir_path);
                }
                else if (strstr(options, "-d"))
                {
                    if (stat(dir_path, &dir_stat) == -1)
                    {
                        printf("Error: Unable to get directory status\n");
                        return;
                    }

                    printf("\nDirectory size: %ld bytes\n", dir_stat.st_size);
                }
                else if (strstr(options, "-a"))
                {
                    print_access_rights(dir_stat);
                }
                else if (strstr(options, "-c"))
                {
                    DIR *dir = opendir(dir_path);
                    if (!dir)
                    {
                        printf("Error: Unable to open directory\n");
                        return;
                    }

                    int count = 0;
                    struct dirent *entry;
                    while ((entry = readdir(dir)))
                    {
                        char file_path[500];
                        sprintf(file_path, "%s/%s", dir_path, entry->d_name);

                        if (stat(file_path, &dir_stat) == 0 && S_ISREG(dir_stat.st_mode))
                        {
                            char *ext = strrchr(entry->d_name, '.');
                            if (ext && strcmp(ext, ".c") == 0)
                            {
                                count++;
                            }
                        }
                    }

                    closedir(dir);
                    printf("\nTotal number of files with .c extension: %d\n", count);
                }

                exit(pid_options);
            }


            if(pid_options>0)
            {
                //char pid_str[10];
                int status;
                waitpid(pid_options, &status, 0);
                printf("\nThe process child ended with PID: %d with exit code %d.\n\n", pid_options, WEXITSTATUS(status));
            }

            if ((pid_process_dir=fork())==0)
            {
                char file_name[250];
                snprintf(file_name, 250, "%s/%s_file.txt", dir_path, dir_path);

                FILE* fp = fopen(file_name, "w");
                if (fp == NULL) {
                    printf("Failed to create the file\n");
                    exit(1);
                }

                fprintf(fp, "This is a sample text for the file");

                fclose(fp);

                exit(0);
            }

            if(pid_process_dir>0)
            {
                //char pid_str[10];
                int status;
                waitpid(pid_process_dir, &status, 0);
                printf("The process child ended with PID: %d with exit code %d.\n\n", pid_process_dir, WEXITSTATUS(status));
            }
        }
}