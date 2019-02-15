#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "dumpassort.h"
#include "reader.h"


void process_line(const char *line, const char *output_dir);
int line_is_empty(const char *line);
int is_regular_file(const char *path);
int is_directory(const char *path);


void read_file(const char *filename, const char *output_dir) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while((read = getline(&line, &len, fp)) != -1) {
        process_line(line, output_dir);
    }

    free(line);
    exit(EXIT_SUCCESS);
}


void process_line(const char *line, const char *output_dir) {
    char *filepath = (char *) malloc(sizeof(char) * (strlen(output_dir) + strlen(line)));

    if (line_is_empty(line)) {
        return;
    }

    strncpy(filepath, output_dir, strlen(output_dir));
    strncat(filepath, &line[0], 1);
    printf("filepath: %s\n", filepath);
    exit(EXIT_SUCCESS);

    printf("%s", line);
}


int line_is_empty(const char *line) {
    return (line == NULL || strlen(line) == 0);
}


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

