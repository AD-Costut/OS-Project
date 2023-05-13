#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

void process_regular_file(char filename[])
{
    const char *extension = ".c";
    char buffer[256];
    FILE *file;
    int line_number = 0;

    if (strcmp(filename + strlen(filename) - strlen(extension), extension) != 0)
    {
        file = fopen(filename, "r");
        if (file == NULL)
        {
            printf("Failed to open file %s.\n", filename);
        }

        while (fgets(buffer, 256, file) != NULL)
        {
            line_number++;
            // printf("%s:%d: %s", filename, line_number, buffer);
        }
        printf("\nThe number of lines is: %d\n", line_number);

        fclose(file);
    }
    else
    {
        char buffer[50];
        sprintf(buffer, "bash script.sh %s", filename);
        system(buffer);

        int first_val, second_val;
        FILE *fp1 = fopen("info_errors_warnings.txt", "r");
        if (fp1 == NULL)
        {
            printf("Error opening file\n");
        }
        fscanf(fp1, "%d %d", &first_val, &second_val);
        fclose(fp1);

        double score = 0.0;
        if (first_val == 0 && second_val == 0)
        {
            score = 10;
        }
        else if (first_val > 0)
        {
            score = 1;
        }
        else if (second_val > 10)
        {
            score = 2;
        }
        else if (second_val <= 10)
        {
            score = 2 + (10 - second_val) * 0.8;
        }

        FILE *fp = fopen("grades.txt", "a");
        if (fp == NULL)
        {
            perror("fopen");
        }
        fprintf(fp, "%s: %.2f\n", filename, score);
        fclose(fp);
    }

    printf("\n");
}
