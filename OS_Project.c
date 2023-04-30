#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define MAX_OPTIONS 100
#define BUFFER_SIZE 256

void print_access_rights(struct stat file_stat) {
    printf("Access rights: ");
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x\n" : "-\n");
}
void regular_file_menu(char* file_path) {
    int file_descriptor = open(file_path, O_RDONLY);

    if (file_descriptor == -1) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    struct stat file_stat;
    if (fstat(file_descriptor, &file_stat) == -1) {
        printf("Error getting file stats: %s\n", strerror(errno));
        close(file_descriptor);
        return;
    }

    printf("File name: %s\n", file_path);
    printf("File size: %lld bytes\n", (long long int) file_stat.st_size);
    printf("Hard link count: %ld\n", (long) file_stat.st_nlink);
    printf("Time of last modification: %s", ctime(&file_stat.st_mtime));
    print_access_rights(file_stat);

    char options[MAX_OPTIONS];
    printf("Enter options string: ");
    fgets(options, MAX_OPTIONS, stdin);

    int length = strlen(options);
    if (options[length - 1] == '\n') {
        options[length - 1] = '\0';
    }

    if (strstr(options, "-n")) {
        printf("File name: %s\n", file_path);
    }

    if (strstr(options, "-d")) {
        printf("File size: %lld bytes\n", (long long) file_stat.st_size);
    }

    if (strstr(options, "-h")) {
        printf("Hard link count: %ld\n", (long) file_stat.st_nlink);
    }

    if (strstr(options, "-m")) {
        printf("Time of last modification: %s", ctime(&file_stat.st_mtime));
    }

    if (strstr(options, "-a")) {
        print_access_rights(file_stat);
    }

    if (strstr(options, "-l")) {
        printf("Enter symlink name: ");
        char symlink_name[BUFFER_SIZE];
        fgets(symlink_name, BUFFER_SIZE, stdin);

        char resolved_path[BUFFER_SIZE];
        if (readlink(symlink_name, resolved_path, BUFFER_SIZE) == -1) {
            printf("Error reading symlink: %s\n", strerror(errno));
            return;
        }

        printf("Symlink points to: %s\n", resolved_path);
    }

    close(file_descriptor);
}
void directory_menu(char* dir_path) {
    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error opening directory: %s\n", strerror(errno));
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char* file_name = entry->d_name;
        if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
            continue;
        }

        char file_path[BUFFER_SIZE];
        snprintf(file_path, BUFFER_SIZE, "%s/%s", dir_path, file_name);

        struct stat file_stat;
        if (lstat(file_path, &file_stat) == -1) {
            printf("Error getting file stats: %s\n", strerror(errno));
            continue;
        }

        if (S_ISREG(file_stat.st_mode)) {
            printf("Regular file: %s\n", file_name);
            regular_file_menu(file_path);
        } else if (S_ISDIR(file_stat.st_mode)) {
            printf("Directory: %s\n", file_name);
            directory_menu(file_path);
        } else if (S_ISLNK(file_stat.st_mode)) {
            printf("Symbolic link: %s\n", file_name);
            if (strstr(file_name, ".txt") != NULL) {
                char buffer[BUFFER_SIZE];
                ssize_t num_bytes = readlink(file_path, buffer, BUFFER_SIZE);
                if (num_bytes == -1) {
                    printf("Error reading symbolic link: %s\n", strerror(errno));
                } else {
                    buffer[num_bytes] = '\0';
                    printf("Link target: %s\n", buffer);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [directory path]\n", argv[0]);
        return 1;
    }

    directory_menu(argv[1]);

    return 0;
}
