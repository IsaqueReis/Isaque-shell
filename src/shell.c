#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/input_handler.h"

int main(int argc, char *argv[])
{

    while(true)
    {
        printf("$ ");
        char *s = read_line();
        if(strlen(s) <= 0 || !(s))
            continue;
            
        printf("%s\n", s);
    }
    
    return 0;
}