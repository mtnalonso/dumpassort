#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "reader.h"
#include "dumpassort.h"


static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"destination", required_argument, 0, 'd'},
    {0, 0, 0, 0}
};
static char *destination_folder = NULL;
static char **input_files = NULL;
static int n_input_files = 0;


void parse_arguments(int argc, char **argv);
void handle_next_argument(int argument, struct option *long_options, int option_index);
void print_help();
void set_input_files(int argc, char **argv, int optind);
void validate_arguments();
char *get_current_directory();


int main (int argc, char **argv) {
    parse_arguments(argc, argv);
    for (int i=0; i < n_input_files; i++) {
        read_file(input_files[i], destination_folder);
    }
    free(destination_folder);
    return 0;
}


void parse_arguments(int argc, char **argv) {
    int option_index;
    int argument;

    while (1) {
        option_index = 0;
        argument = getopt_long(argc, argv, "hd:", long_options, &option_index);
        if (argument != -1) {
            handle_next_argument(argument, long_options, option_index);
        } else {
            break;
        }
    }

    set_input_files(argc, argv, optind);
    validate_arguments();
}


void handle_next_argument(int argument, struct option *long_options, int option_index) {
    switch (argument) {
        case 'h':
            print_help();
            exit(EXIT_SUCCESS);
            break;
        case 'd':
            destination_folder = optarg;
            break;
        default:
            abort();
    }
}


void print_help() {
    printf("\n");
    printf("dumpassort\n\n");
    printf("Usage:\t./dumpassort dumpfile1 ... -d DESTINATION-DIRECTORY\n");
    printf("\n");
    printf("Options:\n");
    printf("\t-d, --destination\tDestination directory to store the entries sorted.\n");
    printf("\t\t\t\tIf no destination is given, it will use the default folder '%s'\n", DEFAULT_DIR_NAME);
    printf("\n");
}


void set_input_files(int argc, char **argv, int optind) {
    int i = 0;
    if (optind < argc) {
        input_files = malloc((optind) * sizeof(char *));
        while (optind < argc) {
            input_files[i++] = argv[optind++];
            n_input_files = i;
        }
    } else {
        perror("No input provided!\n");
        print_help();
        exit(1);
    }
}

void validate_arguments() {
    if (destination_folder == NULL) {
        int destination_folder_len = strlen(DEFAULT_DIR_NAME) + PATH_MAX + 3;
        destination_folder = (char *) malloc(sizeof(char *) * destination_folder_len);

        char *cwd = get_current_directory();

        strncpy(destination_folder, cwd, strlen(cwd));
        strncat(destination_folder, "/", 1);
        strncat(destination_folder, DEFAULT_DIR_NAME, strlen(DEFAULT_DIR_NAME));
        strncat(destination_folder, "/", 1);
        printf("[*] No destination folder specified.\n");
    } else {
        printf("Destination folder: '%s'\n", destination_folder);
    }
}


char *get_current_directory() {
    static char cwd[PATH_MAX + 1];
    getcwd(cwd, PATH_MAX + 1);

    if (strlen(cwd) != 0) {
        return cwd;
    } else {
        perror("Error getting cwd");
        exit(EXIT_FAILURE);
    }
}

