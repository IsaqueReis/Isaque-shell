#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "nimps_mkdir.h"


int main(int argc, char *argv)
{
    if(nimps_mkdir("test2", 3) != 0)
    {
        perror("mkdir");
        return -1;
    }
}