#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "../lib/nimps_make_path.h"

#define PATH_MAX 4096
#define INPUT_BUFF_SIZE 4096
#define NIMPS_CFG_FILE "nimps.cfg"

void load_cwd()
{
    FILE *fp = NULL;
    char *s = NULL;

    fp = fopen(NIMPS_CFG_FILE, "w");
    if(!fp)
    {
        fprintf(stderr, "error opening the file '%s'!\n", NIMPS_CFG_FILE);
    }

    s = (char*) calloc(PATH_MAX, sizeof(char));
    if(!s)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    if( !(s = getcwd(s, PATH_MAX)) )
    {
        perror("getting working directory");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "$WD:%s\n", s);
    fclose(fp);

    return;
}

//retorna o path absoluto do diretório atual ou 
//null caso ocorra algum error
char 
*getwd_from_file()
{
    FILE *fp;
    char s[PATH_MAX] = {'\0'};
    char *output = NULL;
    fp = fopen(NIMPS_CFG_FILE, "r");
    if(!fp)
    {
        fprintf(stderr, "error opening the file '%s'!\n", NIMPS_CFG_FILE);
        perror(NIMPS_CFG_FILE);
        exit(EXIT_FAILURE);
    } 

    while( (fscanf(fp, "%[^\n]", s)) != EOF )
    {
        char **tokens = nimps_split_line(s, ":");
        if( strcmp(tokens[0], "$WD") == 0)
        {
            output = (char*) calloc(strlen(tokens[1] + 1), sizeof(char));
            if(!output)
            {
                fprintf(stderr,"allocation error!\n");
                exit(EXIT_FAILURE);
            }

            strcpy(output, tokens[1]);
            return output;
        }
    }

    return NULL;
}

int process_input(char *input)
{
    if(strcmp("exit", input) == 0)
        exit(EXIT_SUCCESS);
    else if(strcmp("pwd", input) == 0)
        printf("%s\n", getwd_from_file());
    else 
        printf("%s\n", input);
}

int 
main(int argc, char *argv[])
{
    load_cwd(); //load current working directory
    char *input = (char*) calloc(INPUT_BUFF_SIZE, sizeof(char));
    char c = '@';
    int cursor = 0;
    if(!input)
    {
        fprintf(stderr, "allocation error!");
        exit(EXIT_FAILURE);
    }

    while( (printf("> ") > 0) && (scanf("%4096s[^\n]", input) != EOF) )
    {
        getchar();
        process_input(input);
    }
    return 0;
}