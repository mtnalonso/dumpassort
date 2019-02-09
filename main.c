#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>

#define DEFAULT_DIR_NAME "pwds"

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"destination", required_argument, 0, 'd'},
    {0, 0, 0, 0}
};
static char *destination_folder = NULL;
static char **input_files = NULL;


void parse_arguments(int argc, char **argv);
void handle_next_argument(int argument, struct option *long_options, int option_index);
void print_help();
void validate_arguments();
int exists_default_directory();


int main (int argc, char **argv) {
    parse_arguments(argc, argv);
    return 0;
}


void parse_arguments(int argc, char **argv) {
    int argument;

    while (1) {
        int option_index = 0;
        argument = getopt_long(argc, argv, "hd:", long_options, &option_index);
        if (argument != -1) {
            handle_next_argument(argument, long_options, option_index);
        } else {
            break;
        }
    }
    if (optind < argc) {
        printf("Files:\n");
        while (optind < argc) {
            printf ("%s\n", argv[optind++]);
        }
    } else {
        fprintf(stderr, "No input provided!\n");
        print_help();
        exit(1);
    }
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


void validate_arguments() {
    if (destination_folder == NULL) {
        destination_folder = DEFAULT_DIR_NAME;
        printf("[*] No destination folder specified.");
        if (exists_default_directory()) {
            printf(" Using default directory ./%s\n", DEFAULT_DIR_NAME);
        } else {
            printf("[+] Creating default directory ./%s\n", DEFAULT_DIR_NAME);
        }
    } else {
        printf("Destination folder: '%s'\n", destination_folder);
    }
}

int exists_default_directory() {
    DIR* default_dir = opendir(DEFAULT_DIR_NAME);
    if (default_dir) {
        closedir(default_dir);
        return 1;
    } else if (ENOENT == errno) {
        return 0;
    } else {
        fprintf(stderr, "Error checking default output directory.\n");
        exit(1);
    }
}
