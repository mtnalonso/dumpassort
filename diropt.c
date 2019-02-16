#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "dumpassort.h"
#include "diropt.h"
#include "dirutils.h"
#include "reader.h"


void optimize_directory(const char *dirname);
void optimize_directory_entry(const char *dirname, const char *entryname);
int not_folder_ref_files(const char *filename);


void optimize_all(const char *dest_dir) {
    optimize_directory(dest_dir);
    return;
}


void optimize_directory(const char *dirname) {
    struct dirent *directory_entry;
    DIR *directory = opendir(dirname);
    char *filename;

    if (directory == NULL) {
        fprintf(stderr, "Could not open directory %s to optimize files.\n", dirname);
        return;
    }

    while ((directory_entry = readdir(directory)) != NULL) {
        if (not_folder_ref_files(directory_entry->d_name)) {
            optimize_directory_entry(dirname, directory_entry->d_name);
        }
    }

    closedir(directory);
    return;
}

void optimize_directory_entry(const char *dirname, const char *entryname) {
    char *filename;

    filename = (char *) malloc(strlen(dirname) + strlen(entryname) + 2);
    filename = strdup(dirname);
    strncat(filename, entryname, strlen(entryname));

    if (is_regular_file(filename)) {
        printf("-> %s\n", entryname);
        // TODO: check if file is grater than MAX
        // if -> create a dir with file name -> sort using reader
        // with that file as input
    } else if (is_directory(filename)){
        strncat(filename, "/", 1);
        printf("Directory:%s\n", filename);
        optimize_directory(filename);
    }
    free(filename);
}


int not_folder_ref_files(const char *filename) {
    return (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0);
}
