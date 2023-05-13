# OS-Project

This is a C program that takes multiple arguments representing paths to regular files/directories/symbolic links and provides the following functionalities:

    - For each file, the program displays the name and type of the file.
    - An interactive menu will be displayed with all the options available for that specific file type.
    - The user inputs the desired options for that file from the keyboard as a single string. The options will be preceded by the '-' sign and will contain multiple letters corresponding to each         option. If one of the letters of the string is not a valid option, an error message will be displayed, and the menu will appear again.
    - After entering the options string, the information about that file will be printed to the standard output.
    - The program creates a child process for each argument to handle the options introduced by the user (for each file type we have the corresponding options).
    - Additionally, the parent process creates a second child process whose functionality will be determined based on the type of the argument.
    - If the argument is a regular file with the .c extension, the second child executes a script that compiles the file and prints the number of errors and warnings to the standard output.
    - The parent process computes a score based on the number of errors and warnings and writes the message "<FILE_NAME>: <SCORE>" in a file named grades.txt.
    - If the argument is a regular file but doesn't have the .c extension, the second child prints the number of lines.
    - If the argument is a directory, the second child process executes a command for creating a file with the .txt extension and a specific name (e.g. <dir_name>_file.txt, where <dir_name> is the       name of the directory).
    - If the argument is a symbolic link, the second child process executes a command for changing the permissions of the symbolic link.
    - The parent process receives the return state of its child processes and prints the message "The process with PID <PID> has ended with the exit code <EXIT_CODE>".
    - For each argument, the created processes must run in parallel with each other.


