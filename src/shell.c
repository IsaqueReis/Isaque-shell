#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/input_handler.h"

int main(int argc, char *argv[])
{
    while(true)
    {
        printf("\033[1;32m%s@%s:\033[1;36m%s\033[0m$ ",
                real_user_string(), domain_string(), wd_string());

        char *s = read_line();
        
        if(strlen(s) <= 0 || !(s))
            continue;
            
        process_input(s);
    }
    
    return 0;
}