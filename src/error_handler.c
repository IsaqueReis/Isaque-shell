#include <stdio.h>
#include <stdlib.h>

#include "../include/error_handler.h"

void fatal_error(char *message)
{
    perror(message);
    exit(1);
}