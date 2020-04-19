#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdint.h>
#include <stdbool.h>

#include "../include/input_handler.h"
#include "../include/error_handler.h"
#include "../include/process_handler.h"

#define INPUT_BUFF_SIZE                                                    1048
#define PATH_MAX                                                           4098
#define STRING_MAX_SIZE                                                    4098
#define FILE_NAME_MAX_SIZE                                                 1024
#define HOST_NAME_MAX                                                        64
#define TOK_BUFSIZE                                                          64
#define BIN_PATH                                                         "/bin"

unsigned long long len = 0;


void print_tokens(char *tokens[])
{
    for(int i = 0; tokens[i] != NULL; i++)
        printf("%s\n", tokens[i]);

    return;
}

char **split_line(char *line, const char* delim)
{
    int bufsize = TOK_BUFSIZE, k = 0;
    char **tokens = (char **) calloc(bufsize, sizeof(char*));   

    if (!tokens) 
        fatal_error("allocation");    

    char *token = strtok(line, delim);  

    while (token != NULL) 
    {
        tokens[k] = token;
        k++;  
        if (k >= bufsize) 
        {
          bufsize += TOK_BUFSIZE;
          tokens = realloc(tokens, bufsize * sizeof(char*));
          if (!tokens) 
            fatal_error("allocation");
        } 
        token = strtok(NULL, delim);
    }   

    tokens[k] = NULL;
    len = k;

    return tokens;
}

char **split_line2(char *line, const char* delim)
{
    int bufsize = TOK_BUFSIZE, k = 0;
    bool fst_interaction = true;
    char **tokens = (char **) calloc(bufsize, sizeof(char*));   

    if (!tokens) 
        fatal_error("allocation");    

    char *token = strtok(line, delim);  

    while (token != NULL) 
    {
        if(fst_interaction)
        {
            token = strtok(NULL, delim);
            fst_interaction = false;
        } 
        
        else 
        {
            tokens[k] = token;
            k++;  
            if (k >= bufsize) 
            {
              bufsize += TOK_BUFSIZE;
              tokens = realloc(tokens, bufsize * sizeof(char*));
              if (!tokens) 
                fatal_error("allocation");
            } 
            token = strtok(NULL, delim);
        }
    }   

    tokens[k] = NULL;
    len = k;

    return tokens;
}

char *read_line()
{
    char *output = (char*) calloc(INPUT_BUFF_SIZE, sizeof(char));
    int c = '@';
    int cursor = 0;

    if(!output)
        fatal_error("allocation");

    while((c = getchar()))
    {
        if(cursor >= INPUT_BUFF_SIZE)
        {
            output = realloc(output, (cursor + INPUT_BUFF_SIZE));
            if(!output)
                fatal_error("allocation");
        }

        if(c == '\n' || c == EOF)
        {
            output[cursor] = '\0';
            return output;
        }

        output[cursor] = c;
        cursor++;
    }

    return output;
}

char *wd_string()
{
    char *wd = (char*) calloc(PATH_MAX, sizeof(char));

    if((!wd))
        fatal_error("allocation");
    
    if( !(wd = getcwd(wd, PATH_MAX)) ) 
        fatal_error("get current working directory");

    return wd;
}

char *real_user_string()
{
    char *user = (char*) calloc(STRING_MAX_SIZE, sizeof(char));
    struct passwd *p = getpwuid(getuid());

    if((!user))
        fatal_error("allocation");

    if(!p)
        fatal_error("get password database");

    strcpy(user, p->pw_name); //copia o nome do usuário em /etc/passwd

    return user;
}

char *domain_string()
{
    char *hostname = (char*) calloc(HOST_NAME_MAX, sizeof(char));

    if(!hostname)
        fatal_error("allocation");

    if( getdomainname(hostname, sizeof(hostname)) != 0 )
        fatal_error("get hostname");
    
    return hostname;
}

int process_input(char *input)
{
    size_t input_len = strlen(input);
    char *input_copy    = (char*) calloc(input_len + 1, sizeof(char)); 

    strcpy(input_copy, input);                          //faz uma copia da string de input

    char **input_tokens = split_line(input, " ");       //entrada dividida em tokens
    char **exec_tokens  = split_line2(input_copy, " "); //executável no diretório atual

    char *bin_path      = (char*) calloc(STRING_MAX_SIZE, sizeof(char));
    char *cwd           = (char*) calloc(STRING_MAX_SIZE, sizeof(char));
    
    if(!(bin_path) || !(cwd) || !(input_copy))
        fatal_error("allocation");
    
    strcpy(bin_path, BIN_PATH);             
    

    if( strcmp("exit", input) == 0 )                //sair do shell
    {
        exit(EXIT_SUCCESS);
    }

    else if( strcmp("!exec", input_tokens[0]) == 0) //executar um programa no diretório atual
    {
        strcpy(cwd, wd_string());
        strcat(cwd, "/");
        strcat(cwd, input_tokens[1]);
        exec_and_wait(exec_tokens, cwd);

        free(cwd);
    } 

    else                                            //procura executaveis na pasta de binários padrão
    {
        strcat(bin_path, "/");
        strcat(bin_path, input_tokens[0]); 
        exec_and_wait(input_tokens, bin_path);

        free(bin_path);
    }
}