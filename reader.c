#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "reader.h"


void process_line(const char *line);
int line_is_empty(const char *line);
int file_exits(const char *filename);


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
        process_line(line);
    }

    free(line);
    exit(EXIT_SUCCESS);
}


void process_line(const char *line) {
    char *filepath = (char *) malloc(sizeof(char) * strlen(line));

    if (line_is_empty(line)) {
        return;
    }

    struct stat path_stat;
        

    printf("%s", line);
}


int line_is_empty(const char *line) {
    return (line == NULL || strlen(line) == 0);
}


