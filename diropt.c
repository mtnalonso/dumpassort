#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "dumpassort.h"

void optimize_directory(const char *dirname);
int not_folder_ref_files(const char *filename);


void optimize_all(const char *dest_dir) {
    optimize_directory(dest_dir);
    return;
}


void optimize_directory(const char *dirname) {
    struct dirent *directory_entry;
    DIR *directory = opendir(dirname);

    if (directory == NULL) {
        fprintf(stderr, "Could not open directory %s to optimize files.\n", dirname);
        return;
    }

    while ((directory_entry = readdir(directory)) != NULL) {
        if (not_folder_ref_files(directory_entry->d_name)) {
            printf("-> %s\n", directory_entry->d_name);
        }
    }

    closedir(directory);
    return;
}


int not_folder_ref_files(const char *filename) {
    return (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0);
}
