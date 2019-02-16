#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "dumpassort.h"
#include "reader.h"


void process_line(const char *line, const char *output_dir);
int line_is_empty(const char *line);
int is_regular_file(const char *path);
int is_directory(const char *path);
void append_line_to_file(const char *line, const char *filename);
void create_subdirectory(const char *output_dir, const char *folder);
void create_directory_default_files(const char *output_dir);


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
}

void presetup(const char *output_dir) {
    if (is_directory(output_dir) < 1) {
        mkdir(output_dir, 0700);
        printf("[+] Created default dest directory: %s\n", output_dir);

        for (int i = 0; i < strlen(dest_folders); i++) {
            create_subdirectory(output_dir, &dest_folders[i]);
        }
    }   
}

void create_subdirectory(const char *output_dir, const char *folder) {
    char *sub_dir = (char *) malloc(strlen(output_dir) + 1);

    sub_dir = strdup(output_dir);
    strncat(sub_dir, folder, 1);

    if (is_directory(sub_dir) < 1) {
        mkdir(sub_dir, 0700);
        create_directory_default_files(sub_dir);
    }

    free(sub_dir);
}


void create_directory_default_files(const char *output_dir) {
    for (int i = 0; i < strlen(dest_folders); i++) {
        char *filename = (char *) malloc(strlen(output_dir) + 2);
        filename = strdup(output_dir);
        strncat(filename, "/", 1);
        strncat(filename, &dest_folders[i], 1);

        if (is_regular_file(filename) < 1) {
            int ftouch = open(filename, O_RDWR|O_CREAT, 0666);
            close(ftouch);
        }
        free(filename);
    }
    return;
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
        } else if (strlen(output_dir) >= strlen(line)) {
            fprintf(stderr, "Path %s does not exist", filepath);
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Symbol email: %s", line);
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

