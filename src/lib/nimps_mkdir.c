#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nimps_mkdir.h"


int 
nimps_mkdir(const char *dir, mode_t mode)
{
    int ret = 0;                        
    char *tmp_dir;
    size_t len = strlen(dir);

    if(len && dir[len - 1] == '/')
    {
        tmp_dir = strdup(dir);
        if(!tmp_dir)
        {
            errno = ENOMEM;
            return -1;
        }

        tmp_dir[len - 1] = '\0'; 
    }

    else 
    {
       tmp_dir = (char *) dir; 
    }

    ret = mkdir(tmp_dir, mode);
    if(tmp_dir != dir)
        free(tmp_dir);

    return ret;
}