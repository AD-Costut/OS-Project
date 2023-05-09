#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define PATH_MAX 250

void symbolic_link_logic(char symlink_path[]) {
    struct stat symlink_stat;
    char options[3];

    printf("Select an option: ");
    scanf("%s", options);

    if (strstr(options, "-n")) {
        printf("Symbolic link name: %s\n", symlink_path);
    }

    if (strstr(options, "-l")) {
        if (unlink(symlink_path) == -1) {
            printf("Error deleting symbolic link: %s\n", strerror(errno));
            return;
        }

        printf("Symbolic link deleted successfully\n");
        return;
    }

    if (stat(symlink_path, &symlink_stat) == -1) {
        printf("Error: Unable to get symbolic link status\n");
        return;
    }

    if (strstr(options, "-d")) {
        printf("Symbolic link size: %ld bytes\n", symlink_stat.st_size);
    }

    if (strstr(options, "-t")) {
        char resolved_path[PATH_MAX];
        if (realpath(symlink_path, resolved_path) == NULL) {
            printf("Error: Unable to get resolved path\n");
            return;
        }

        struct stat target_stat;
        if (stat(resolved_path, &target_stat) == -1) {
            printf("Error: Unable to get target file status\n");
            return;
        }

        printf("Target file size: %ld bytes\n", target_stat.st_size);
    }

    if (strstr(options, "-a")) {
        print_access_rights(symlink_stat);
    }
}