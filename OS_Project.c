#include <stdlib.h>
#include <ctype.h> // Testing & mapping characters : isalnum(int c)
#include <errno.h> // Declaration of global variable errno
#include <fcntl.h> // File descriptor
#include <unistd.h> // Miscellanous functions
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // Structure of the data returned by fstat(), lstat() and stat()

int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    if (argc < 2) 
    {
        printf("Usage: %s file1 file2 ...\n", argv[0]);
        return 1;
    }

    return 0;
}