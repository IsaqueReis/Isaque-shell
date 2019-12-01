#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


#define ARG_ERROR                                                              1
#define ALOC_ERROR                                                             2

char *optarg;
int optind = 1, opterr = 0, optopt = '?';
char getopt(int argc, char *const argv[], const char *optstring)
{
    for(int i = optind; i < argc; i++)
    {
        for(int j = 0; optstring[j] != '\0';)
        {
            printf("argv: [%s] - optstr[%c]\n", argv[i], optstring[j]);
            if(strlen(argv[i]) == 2) //verify if its a flag
            {
                if(argv[i][0] == '-' && argv[i][1] != '-') 
                {
                    printf("\targv: [%s] is a flag!\n", argv[i]);
                    if(argv[i][1] == optstring[j]) //match
                    {
                        printf("\targv: [%s] combines with flag[%c]\n", argv[i], optstring[j]);

                        if(optstring[j + 1] == ':')
                        {
                            printf("\topt [%c] requires argument!\n", optstring[j]);
                            //is a valid arg
                            if(strlen(argv[i + 1]) > 0 && argv[i + 1][0] != '-')
                            {
                                printf("\tflag [%s] have valid argument!\n", argv[i]);

                                optarg = strdup(argv[i + 1]);
                                if(!optarg)
                                {
                                    errno = ENOMEM;
                                    perror("getopt");
                                    exit(1);
                                }
                                printf("\targ: [%s]\n", optarg);
                                printf("\tret val: [%c]\n", optstring[j]);
                                j+= 2;
                            } 
                            
                            else 
                            {
                                printf("\tflag [%s] have invalid argument!\n", argv[i]);
                                printf("\targ: [%s]\n", argv[i + 1]);
                                printf("\tret val: [:]\n");
                                j+= 2;
                                
                            }
                        }
                        
                        else 
                        {
                            printf("\tret val: [%c]\n", optstring[j]);
                        }

                    }
                    else 
                    {
                         printf("\targv: [%s] dont combine with flag[%c]\n", argv[i], optstring[j]);
                         printf("\tret val: [?]\n");
                    }
                     
                }
                 
                else 
                {
                    printf("\targv: [%s] is not a flag\n", argv[i]);
                }
            }
            else if(strlen(argv[i]) > 2) 
            {
                /*
                if(argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] != '-')
                {
                    printf("\targv: [%s] is a flag!\n", argv[i]);
                } 
                
                else if (argv[i][0] == '-' && argv[i][1] != '-')
                {
                    printf("\targv: [%s] is a flag\n", argv[i]);
                }
                
                else
                {
                    printf("\targv: [%s] is not a flag\n", argv[i]);
                } 
                */
            } 

            else 
            {
                printf("\targv: [%s] is not a flag\n", argv[i]);
            }

            j++;
        }
        optind++;
    }
    return -1;
}


int main(int argc, char *argv[])
{
    int opt = 0;
    while((opt = getopt(argc, argv, "m:p")) != - 1)
    {
        //printf("%c", (char) opt);
    }

    printf("\n%s %d %d %c\n", optarg, optind, opterr, optopt);
    return 0;
}