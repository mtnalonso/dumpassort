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
int has_max_file_size(const char *filename);


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
    char *filename = NULL;
    char *import_temp_filename = NULL;
    char import_label[] = "import_";

    filename = (char *) malloc(strlen(dirname) + strlen(entryname) + 2);
    filename = strdup(dirname);
    strncat(filename, entryname, strlen(entryname));

    if (is_regular_file(filename)) {
        if (has_max_file_size(filename)) {
            import_temp_filename = (char *) malloc(strlen(filename) + strlen(import_label) + 2);
            import_temp_filename = strdup(dirname);
            strncat(import_temp_filename, import_label, strlen(import_label));
            strncat(import_temp_filename, entryname, strlen(entryname));

            rename(filename, import_temp_filename);

            strncat(filename, "/", 1);

            presetup(filename, 0);
            read_file(import_temp_filename, filename);
            remove_file(import_temp_filename);

            free(import_temp_filename);
        }
    } else if (is_directory(filename)){
        strncat(filename, "/", 1);
        optimize_directory(filename);
    }

    free(filename);
    return;
}


int not_folder_ref_files(const char *filename) {
    return (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0);
}

int has_max_file_size(const char *filename) {
    return (get_file_size_bytes(filename) >= MAX_FILE_SIZE);
}
