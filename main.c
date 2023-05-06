#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#include "print_access_rights.c"
#include "regular_file_menu.c"
#include "files_menu.c"
#include "regular_file_logic.c"
#include "dir_logic.c"
#include "symbolic_link_logic.c"

#define MAX_OPTIONS 100
#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: %s [directory path]\n", argv[0]);
        return 1;
    }

    files_menu(argc, argv);

    return 0;
}
