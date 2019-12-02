#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/nimps_mkdir.h"
#include "../lib/nimps_getopt.h"

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