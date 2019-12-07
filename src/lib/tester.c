#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "nimps_mkdir.h"
#include "nimps_getopt.h"
#include "make_path.h"

int main(int argc, char *argv[])
{
    char path[] = "e/.////.///c";

    char *s = clean_path(path);

    printf("cleanded : %s \n", s);

    char s1[] = "asssads";
    char s2[] = "safsda";
    if(nimps_make_path(s1, s2) != -1)
        printf("%s\n", s1);
    return 0;
}

