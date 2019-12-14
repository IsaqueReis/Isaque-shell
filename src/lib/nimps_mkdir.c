#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "nimps_mkdir.h"

//mkdir
//-----
//cria um diretório dado seu path e o seu modo(chmod)
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

//rmdir
//-----
//remove um diretório dado seu caminho. O diretório só é removido se estiver vazio
int 
nimps_rmdir(const char *dir)
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

    ret = rmdir(tmp_dir);
    if(tmp_dir != dir)
        free(tmp_dir);

    return ret;
}