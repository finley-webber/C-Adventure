#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>


typedef struct command{
    char* name;
    void (*function)();
} command;

void ls();
void pwd();

#ifndef SHELLFIN_H
#define SHELLFIN_H


#define MAX_COMMANDS 5

extern struct command commands[MAX_COMMANDS];

#endif