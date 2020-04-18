#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input_handler.h"

#define INPUT_BUFF_SIZE                                                    1048

char *read_line()
{
    char *output = (char*) calloc(INPUT_BUFF_SIZE, sizeof(char));
    int c = '@';
    int cursor = 0;

    if(!output)
    {
        fprintf(stderr,"allocation error!");
        exit(1);
    }

    while((c = getchar()))
    {
        if(cursor >= INPUT_BUFF_SIZE)
        {
            output = realloc(output, (cursor + INPUT_BUFF_SIZE));
            if(!output)
            {
                fprintf(stderr,"allocation error!");
                exit(1);
            }
        }

        if(c == '\n' || c == EOF)
        {
            output[cursor] = '\0';
            return output;
        }

        output[cursor] = c;
        cursor++;
    }
    return output;
}