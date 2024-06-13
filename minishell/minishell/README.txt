Objective:

Develop a basic shell program in C with built-in commands, color printing, signal handling, and fork/exec functionality for executing commands.

Main Task:

Implement a shell that prints the current working directory in blue and waits for user input.
The shell is invoked from the command line without arguments.
Prompt: Color Printing:

Display the current working directory in blue using defined color codes in minishell.c.
Use #define BLUE "\x1b[34;1m" and #define DEFAULT "\x1b[0m" for color management.

Commands:

Manually Implemented Commands:

cd: Changes the current directory. No argument or ~ changes to the user's home directory.
exit: Terminates the shell with EXIT_SUCCESS.
pwd: Prints the current working directory.
lf: Lists all files in the current directory, including hidden files but excluding . and ...
lp: Lists all current system processes with their PID, USER, and COMMAND, sorted by PID.
Other Commands: