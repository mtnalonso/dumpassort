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
void append_line_to_file(const char *line, const char *filename);


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
    int i = 0;

    if (line_is_empty(line)) {
        return;
    }

    strncpy(filepath, output_dir, strlen(output_dir));

    while (1) {
        strncat(filepath, &line[i], 1);
        if (is_regular_file(filepath)) {
            append_line_to_file(line, filepath);
            break;
        } else if (is_directory(filepath)) {
            strncat(filepath, "/", 1);
            i++;
        } else {
            fprintf(stderr, "Path %s does not exist", filepath);
            exit(EXIT_FAILURE);
        }
    }
}

void append_line_to_file(const char *line, const char *filename) {
    FILE *fp;
    fp = fopen(filename, "a");
    fprintf(fp, "%s", line);
    fclose(fp);
    return;
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

