#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void score(char* file_path) {
    // Check if the file is a regular file with .c extension
    char* extension = strrchr(file_path, '.');
    if (extension == NULL || strcmp(extension, ".c") != 0) {
        //printf("\nThe file is not a regular file with .c extension.\n");
        return;
    }

    // Create a pipe for inter-process communication
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    // Fork a child process to execute the script
    pid_t pid_script = fork();
    if (pid_script == -1) {
        perror("fork");
        return;
    } else if (pid_script == 0) {
        // Child process: execute the script and send the output to the parent process
        close(pipefd[0]); // Close the read end of the pipe

        // Redirect the stdout and stderr to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        // Compile the file and print the number of errors and warnings
        char* args[] = {"gcc", "-Wall", "-o", "/dev/null", file_path, NULL};
        execvp("gcc", args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: read the output from the script and compute the score
        close(pipefd[1]); // Close the write end of the pipe

        // Read the output from the script
        char buffer[BUFFER_SIZE];
        int n = read(pipefd[0], buffer, BUFFER_SIZE);
        if (n == -1) {
            perror("read");
            return;
        }
        buffer[n] = '\0';

        // Parse the output to get the number of errors and warnings
        int num_errors = 0, num_warnings = 0;
        char* p = buffer;
        while ((p = strstr(p, "error:"))) {
            num_errors++;
            p += 6;
        }
        p = buffer;
        while ((p = strstr(p, "warning:"))) {
            num_warnings++;
            p += 8;
        }

        // Compute the score based on the number of errors and warnings
        int score = 0;
        if (num_errors == 0 && num_warnings == 0) {
            score = 10;
        } else if (num_errors > 0) {
            score = 1;
        } else if (num_warnings > 10) {
            score = 2;
        } else if(num_warnings <= 10){
            score = 2 + 8 * (10 - num_warnings) / 10;
        }

        // Write the score to the grades.txt file
        FILE* fp = fopen("grades.txt", "a");
        if (fp == NULL) {
            perror("fopen");
            return;
        }
        fprintf(fp, "%s: %d\n", file_path, score);
        fclose(fp);

        //printf("\n%s: %d\n", file_path, score);
    }
}
