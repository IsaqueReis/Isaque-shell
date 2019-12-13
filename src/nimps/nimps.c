#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int 
main(int argc, char *argv[])
{
    char c = '@';
    while((c = getopt(argc, argv, "m:p") != -1))
    {
        printf("%c", c);
    }
    return 0;
}