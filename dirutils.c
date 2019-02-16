#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>


int is_regular_file(const char *path) {
    struct stat file_stat;
    stat(path, &file_stat);
    return S_ISREG(file_stat.st_mode);
}


int is_directory(const char *path) {
    struct stat file_stat;
    stat(path, &file_stat);
    return S_ISDIR(file_stat.st_mode);
}


void remove_file(const char *filename) {
    if (remove(filename) != 0) {
        fprintf(stderr, "Could not remove file %s.\n", filename);
    }
}


int get_file_size_bytes(const char *filename) {
    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_size;
}
