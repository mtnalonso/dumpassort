#include <stdio.h>
#include <stdlib.h>
#include "reader.h"


void read_file(char *filename) {
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
        printf("%s", line);
    }

    free(line);
    exit(EXIT_SUCCESS);
}


const char *read_file_line() {
    char *line_buffer = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);

    free(line_buffer);
    return NULL;
}
