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
    int i = 0;

    if (line_is_empty(line)) {
        return;
    }

    strncpy(filepath, output_dir, strlen(output_dir));

    while (1) {
        strncat(filepath, &line[i], 1);
        printf("filepath: %s\n", filepath);
        if (is_regular_file(filepath)) {
            // TODO: check size, if it does not pass the threshold, save
            printf("Saving \"%s\"\t=> %s\n", line, filepath);
            break;
        } else if (is_directory(filepath)) {
            strncat(filepath, "/", 1);
            i++;
        } else {
            fprintf(stderr, "Path %s does not exist", filepath);
            exit(EXIT_FAILURE);
        }
    }

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

