#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "nimps_mkdir.h"
#include "nimps_getopt.h"

int main(int argc, char *argv[])
{
    int opt = 0;
    while((opt = getopt(argc, argv, "m:p:")) != - 1)
    {
        printf("%c", (char) opt);
    }

    printf("\n");

    printf("%d %d %c %s\n", optind, opterr, optopt, optarg);
    return 0;
}

