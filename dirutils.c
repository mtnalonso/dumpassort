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
