#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


char *optarg;
int optind, opterr, optopt;
char getopt(int argc, char *const argv[], const char *optstring)
{
    for(int i = optind; optstring[i] != '\0'; i++)
    {
        if(optstring[i] == ':')
        {
            optind++;
            continue;
        }

        //search arg
        for(int j = 1; j < argc; j++)
        {
            printf("argv: %s x optstring %c.\n", argv[j], optstring[i]);
            if(strlen(argv[j]) == 2) //chek a simple flag value
            {
                if(argv[j][0] == '-' && argv[j][1] != '-')
                {
                   printf("%s its a simple flag\n", argv[j]);

                    if(optstring[i] == argv[j][1]) //match
                    {
                        if(optstring[i + 1] == ':')
                        {
                            printf("flag %s have an arg\n", argv[j]);
                            optarg = strdup(argv[j + 1]);
                            if(!optarg)
                            {
                                errno = ENOMEM;
                                perror("getopt");
                                exit(1);
                            }

                            if(optarg[0] == '-' || strlen(optarg) == 0)
                            {
                                printf("argument %s of flag %s incorrect\n", optarg, argv[j]);
                            } 

                            else 
                            {
                                printf("argument %s of flag %s correct\n", optarg, argv[j]);
                            }
                        }

                        else 
                        {
                            optopt = optstring[i];
                            optind++;
                            return optstring[i];
                        }
                        
                    } 
                    else
                    {
                        continue;
                    }
                }
            }

            else 
            {
                if(argv[j][0] == '-' && argv[j][1] == '-')
                {
                    printf("%s its a flag\n", argv[j]);

                    if(optstring[i] == argv[j][2]) //match
                    {
                        if(optstring[i + 1] == ':')
                        {
                            printf("flag %s have an arg\n", argv[j]);
                            optarg = strdup(argv[j + 1]);
                            if(!optarg)
                            {
                                errno = ENOMEM;
                                perror("getopt");
                                exit(1);
                            }

                            if(optarg[0] == '-' || strlen(optarg) == 0)
                            {
                                printf("argument %s of flag %s incorrect\n", optarg, argv[j]);
                                opterr = 1;
                                optopt = optstring[i];
                                optind++;
                                return ':';
                            } 

                            else 
                            {
                                printf("argument %s of flag %s correct\n", optarg, argv[j]);
                                optopt = optstring[i];
                                optind++;
                                return optstring[i];
                            }
                        } else 
                        {
                            optopt = optstring[i];
                            optind++;
                            return optstring[i];
                        }
                        
                    } 
                    else
                    {
                        continue;
                    }
                } 
            }   
        }//for argc loop
        //printf("\n");
        optind++;
        optopt = optstring[i];
        return '?';
    }//for

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