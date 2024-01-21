#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include "Shellfin.h"

void ls() {

    char cp[PATH_MAX];
    getcwd(cp, sizeof(cp));


    DIR *directory;
    struct dirent *d;

    directory = opendir(cp);

    if(directory) {
        while ((d = readdir(directory)) != NULL) {
            printf("%s\n", d->d_name);
        }
        closedir(directory);
    } else {
        printf("[-] Failed to open directory\n");
    }
}

void pwd() {
    char cp[PATH_MAX];
    getcwd(cp,sizeof(cp));

    printf("%s\n", cp);
}