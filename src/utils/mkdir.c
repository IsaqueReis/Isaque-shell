#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "../lib/nimps_mkdir.h"
#include "../lib/nimps_getopt.h"
#include "../lib/make_path.h"

int make_directory(char *path, uint32_t mode)
{
    char *input = clean_path(path); //clean redundat parts of path
}

int main(int argc, char *argv[])
{
    int opt = 0;
    while((opt = nimps_getopt(argc, argv, "m:p:")) != - 1)
    {
        printf("%c", (char) opt);
    }

    printf("\n");

    printf("%d %d %c %s\n", optind, opterr, optopt, optarg);
    return 0;
}