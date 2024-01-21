#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include "Shellfin.h"

/*

Challenge: Custom Mini Shell
Create a simple shell interface that can handle input commands in C. 
This shell should be able to execute basic commands like ls, pwd, echo, 
and handle arguments. It doesn't need to be as complex as bash or zsh, 
but it should demonstrate the fundamental concepts of process management 
and file descriptors in C.

Requirements:
Prompt Input: Your shell should display a prompt where users can type commands.
Command Execution: It should be able to execute simple commands like ls, pwd, and echo with arguments.
Exit Command: Allow the user to type exit to quit the shell.
Error Handling: Display an appropriate message for unrecognized commands.
Internal Command: Implement at least one internal command; for example, cd to change directories within your shell.
Bonus:
Pipes: Implement functionality for piping between commands, e.g., ls | sort.
Redirection: Implement input and output redirection, e.g., ls > files.txt or cat < files.txt.
Background Execution: Allow a command to run in the background by using &, for instance, sleep 10 &.
Guidelines:
Use fork(), exec(), and wait() system calls to execute commands.
Use getline() or similar functions to read input from the user.
Properly parse the user input to separate the command and its arguments.
Handle errors gracefully and ensure that your shell is robust against incorrect inputs.
This challenge will test your understanding of process control, inter-process communication, 
and the general architecture of the Unix/Linux operating system. It's a great way to get deeper 
into systems programming and understand how shells work internally.

*/

char* k = "[+]";
char* i = "[*]";
char* e = "[-]";

struct command commands[MAX_COMMANDS] = {
    {"ls\n", ls},
    {"pwd\n", pwd},
};

void readInput(char *message) {
    size_t bufferSize = 1000;
    size_t receivedLine;

    *message = *(char*) malloc(bufferSize * sizeof(char));
    if (message == NULL) {
        printf("%s Failed to allocate memory\n", e);
    }

    receivedLine = getline(&message, &bufferSize, stdin);
}

void obtainWorkingDirectory(char* cp, unsigned long size) {
    if (getcwd(cp, size) == NULL) {
        printf("%s Failed to retrieve current working directory\n", e);
    }
}

void cycle() {
    char msg[PATH_MAX];
    char currentPath[PATH_MAX];
    do {
        obtainWorkingDirectory(currentPath, sizeof(currentPath));
        printf("<%s>: ", currentPath);
        readInput(msg);

        /*for (int i = 0; i < strlen(msg) + 1; i++) {
            if (strcmp(&msg[i], " ") == 0) {

            }
        }*/

        for (int i = 0; i <= MAX_COMMANDS; i++) {
            if (commands[i].function != NULL) {
                if (strcmp(msg, commands[i].name) == 0) {
                    commands[i].function();
                }
            }
        };

    } while (strcmp(msg, "quit\n") != 0);
}

int main (void) {
    cycle();

    return 0;
}

