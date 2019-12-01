#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


#define ARG_ERROR 1
#define ALOC_ERROR 2

char *optarg;
int optind, opterr, optopt, argind = 0;
char getopt(int argc, char *const argv[], const char *optstring)
{
    char ret = '@';
    for(int i = argind; i < argc; i++)
    {
        //verify if arg contains optstring
        for(int j = 0; optstring[j] != '\0'; j++)
        {
            printf("argv: %s x optstring %c.\n", argv[i], optstring[j]);
            if(strlen(argv[i]) == 2) //simple flag value
            {
                if(argv[i][0] == '-' && argv[i][1] != '-')
                {
                    printf("%s its a simple flag\n", argv[i]);
                    if(optstring[j] == argv[i][1]) //match 
                    {
                        if(optstring[j + 1] == ':')
                        {
                            printf("flag %s have an arg\n", argv[i]);
                            optarg = strdup(argv[j + 1]);
                            if(!optarg)
                            {
                                errno = ENOMEM;
                                perror("getopt");
                                opterr = ALOC_ERROR;
                                exit(opterr);
                            }//if(!optarg)

                            if(optarg[0] == '-' || strlen(optarg) == 0)
                            {
                                printf("argument %s of flag %s incorrect\n", optarg, argv[i]);
                                opterr = ARG_ERROR;
                                optopt = optstring[j];
                                optind = j;
                                ret = ':';
                                break;
                            }//if(optarg[0] == '-' || strlen(optarg) == 0)

                            else 
                            {
                                printf("argument %s of flag %s correct\n", optarg, argv[i]);
                                optopt = optstring[j];
                                optind++;
                                optind = j;
                                ret = optstring[j];
                                break;
                            }
                        }//if(optstring[j + 1] == ':')

                        else 
                        {
                            optopt = optstring[j];
                            optind = j;
                            ret = optstring[j];
                            break;
                        }
                    }//if(optstring[j] == argv[i][1])

                    else 
                    {
                        continue;
                    }
                }
            }// if(strlen(argv[i]) == 2)

            else 
            {
                if(argv[i][0] == '-' && argv[i][1] == '-')
                {
                    printf("%s its a flag\n", argv[i]);

                    if(optstring[i] == argv[j][2]) //match
                    {
                        if(optstring[j + 1] == ':')
                        {
                            printf("flag %s have an arg\n", argv[i]);
                            optarg = strdup(argv[j + 1]);
                            if(!optarg)
                            {
                                errno = ENOMEM;
                                perror("getopt");
                                opterr = ALOC_ERROR;
                                exit(opterr);
                            }//if(!optarg)

                            if(optarg[0] == '-' || strlen(optarg) == 0)
                            {
                                printf("argument %s of flag %s incorrect\n", optarg, argv[i]);
                                opterr = ARG_ERROR;
                                optopt = optstring[j];
                                optind = j;
                                ret = ':';
                                break;
                            }//if(optarg[0] == '-' || strlen(optarg) == 0)

                            else 
                            {
                                printf("argument %s of flag %s correct\n", optarg, argv[i]);
                                optopt = optstring[j];
                                optind++;
                                optind = j;
                                ret = optstring[j];
                                break;
                            }
                        }//if(optstring[j + 1] == ':')

                        else 
                        {
                            optopt = optstring[j];
                            optind = j;
                            ret = optstring[j];
                            break;
                        }
                    }//if(optstring[i] == argv[j][2])
                }//(argv[i][0] == '-' && argv[i][1] == '-')
            }
            optopt = optstring[j];
            optind = j;
            ret = '?';
        }//for(int j = optind; optstring[j] != '\0'; j++)
        return ret;
        argind++;
    }//for(int i = 1; i < argc; i++)

    return -1;
}


int main(int argc, char *argv[])
{
    int opt = 0;
    while((opt = getopt(argc, argv, "m:p")) != - 1)
    {
        printf("%c", (char) opt);
    }

    printf("\n%s\n", optarg);
    return 0;
}