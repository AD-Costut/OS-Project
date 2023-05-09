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
    const char* extension = ".c";
    char buffer[256];
    FILE* file;
    int line_number = 0;

    // Check if the filename ends with the extension
    if (strcmp(filename + strlen(filename) - strlen(extension), extension) != 0) {
        // Open the file for reading
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Failed to open file %s.\n", filename);
            return 1;
        }

        // Print the lines without the extension
        while (fgets(buffer, 256, file) != NULL) {
            line_number++;
            printf("%s:%d: %s", filename, line_number, buffer);
        }

        // Close the file
        fclose(file);
    } else {
        printf("%s is not a C file.\n", filename);
    }

    printf("\n\n");

}

void write_to_file(char content[]) {
    
    int fd = open("grade.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        printf("Failed to open file grade.txt.\n");
        return;
    }

    ssize_t nwritten = write(fd, content, strlen(content));
    if (nwritten == -1) {
        printf("Failed to write to file grade.txt.\n");
        close(fd);
        return;
    }

    close(fd);
}