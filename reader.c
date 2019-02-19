#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>

#include "dumpassort.h"
#include "reader.h"
#include "dirutils.h"

#define N_THREADS 50


void process_line(const char *line, const char *output_dir);
int line_is_empty(const char *line);
void append_line_to_file(const char *line, const char *filename);
void create_subdirectory(const char *output_dir, const char *folder);
void create_directory_default_files(const char *output_dir);
void *read_file_thread(void *p_file_reader);


struct file_reader_st {
    pthread_t thid;
    pthread_mutex_t *p_mutex;
    FILE *fp;
    const char *output_dir;
};


void read_file(const char *filename, const char *output_dir) {
    struct file_reader_st file_reader[N_THREADS];
    FILE *fp;
    pthread_mutex_t reader_mutex;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&reader_mutex, NULL);

    for (int i=0; i < N_THREADS; i++) {
        file_reader[i].fp = fp;
        file_reader[i].p_mutex = &reader_mutex;
        file_reader[i].output_dir = output_dir;
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&file_reader[i].thid, NULL, read_file_thread, (void *) &file_reader[i]);
    }

    for (int i= 0; i < N_THREADS; i++) {
        pthread_join(file_reader[i].thid, NULL);
    }

    fclose(fp);
    pthread_mutex_destroy(&reader_mutex);
    printf("\n");
    printf("[+] The file \"%s\" has been imported!\n", filename);
    return;
}


void *read_file_thread(void *p_file_reader) {
    struct file_reader_st *file_reader = p_file_reader;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        if (feof(file_reader->fp)) {
            break;
        }

        pthread_mutex_lock(file_reader->p_mutex);
        read = getline(&line, &len, file_reader->fp);
        pthread_mutex_unlock(file_reader->p_mutex);

        if (read != -1) {
            process_line(line, file_reader->output_dir);
        }
    }

    free(line);
    return NULL;
}


void presetup(const char *output_dir, int sublevel) {
    if (is_directory(output_dir) < 1) {
        mkdir(output_dir, 0700);
        printf("[+] Created dest directory: %s\n", output_dir);

        if (sublevel) {
            for (int i = 0; i < strlen(dest_folders); i++) {
                create_subdirectory(output_dir, &dest_folders[i]);
            }
        } else {
            create_directory_default_files(output_dir);
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
    char current_char[2];
    int i = 0;

    if (line_is_empty(line)) {
        return;
    }

    strncpy(filepath, output_dir, strlen(output_dir));

    while (1) {
        current_char[0] = line[i];
        current_char[1] = '\0';

        if (isalpha(current_char[0])) {
            current_char[0] = tolower(current_char[0]);
        } else if (isdigit(current_char[0]) == 0) {
            current_char[0] = '+';
        }

        strncat(filepath, current_char, 1);

        if (is_regular_file(filepath)) {
            append_line_to_file(line, filepath);
            break;
        } else if (is_directory(filepath)) {
            strncat(filepath, "/", 1);
            i++;
        } else {
            fprintf(stderr, "[-] Could not import token: %s", line);
            break;
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

