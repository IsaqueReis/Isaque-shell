#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "nimps_getopt.h"

#define ARG_ERROR                                                              1
#define ALOC_ERROR                                                             2

char *optarg;
int optind = 1, opterr = 0, optopt = '?';
char nimps_getopt(int argc, char *const argv[], const char *optstring)
{
    char ret = '?';

    for(int i = optind; i < argc; i++)
    {
        for(int j = 0; optstring[j] != '\0';)
        {
            if(strlen(argv[i]) == 2)
            {
                if(argv[i][0] == '-' && argv[i][1] != '-') 
                {
                    if(argv[i][1] == optstring[j])
                    {
                        if(optstring[j + 1] == ':')
                        {
                            if(i < argc -1)
                            {
                                if(strlen(argv[i + 1]) > 0 && argv[i + 1][0] != '-')
                                {

                                    optarg = strdup(argv[i + 1]);
                                    if(!optarg)
                                    {
                                        errno = ENOMEM;
                                        perror("getopt");
                                        exit(1);
                                    }
                                    ret = optstring[j];
                                    optopt = optstring[j];
                                    j+= 2;
                                } 
                            
                                else 
                                {
                                    ret = ':';
                                    opterr = 1;
                                    optopt = ':';
                                    j+= 2;
                                }
                            } 
                            
                            else
                            {
                                 ret = ':';
                                 opterr = 1;
                                 optopt = ':';
                                 j+= 2;
                            } 
                        }
                        
                        else 
                        {
                            ret = optstring[j];
                            optopt = optstring[j];
                        }
                    }
                }
                 
                else 
                {
                    ret = '#';
                    
                }
            }

            else if(strlen(argv[i]) > 2) 
            {
                
                if(argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] != '-')
                {
                    
                    if(argv[i][2] == optstring[j])
                    {
                        if(optstring[j + 1] == ':')
                        {
                            if(i < argc -1)
                            {
                                if(strlen(argv[i + 1]) > 0 && argv[i + 1][0] != '-')
                                {

                                    optarg = strdup(argv[i + 1]);
                                    if(!optarg)
                                    {
                                        errno = ENOMEM;
                                        perror("getopt");
                                        exit(1);
                                    }

                                    ret = optstring[j];
                                    optopt = optstring[j];
                                    j+= 2;
                                } 
                            
                                else 
                                {
                                    ret = ':';
                                    opterr = 1;
                                    optopt = ':';
                                    j+= 2;
                                }
                            } 
                            
                            else
                            {
                                 ret = ':';
                                 opterr = 1;
                                 optopt = ':';
                                 j+= 2;
                            } 
                        }
                        
                        else 
                        {
                            ret = optstring[j];
                            optopt = optstring[j];
                        }
                    }
                }      
                else
                {
                    ret = '#';
                } 
                
            } 

            else 
            {
                ret = '#';
            }

            j++;
        }
        optind++;
        return ret;
    }
    return -1;
}