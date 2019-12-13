#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "../lib/nimps_mkdir.h"
#include "../lib/nimps_getopt.h"
#include "../lib/nimps_make_path.h"
#include "../lib/nimps_list.h"

#define MAX_ARG_SIZE                                                       4096

int parent = 0;
int verbose = 0;
int mode = 0;

char program_name[] = "mkdir";
char program_author[] = "isaqreis";
char program_version[] = "0.0.0.0.0.0.0.0.0.0.0.0.1";

void print_args(List args)
{
    for(int i = 1; i <= listNOI(args); i++)
    {
        printf("arg:%s\n", (char*) listGetI(args, i));
    }
}
void print_usage()
{
    printf("Uso: mkdir [OPTION]... DIRECTORY...\n");
    printf("Cria o DIRECTORIO(os), se eles já não existem.\n");
    printf("\n");
    printf("-m, --mode=MODE   seta o filemode (como em chmod), não a=rwx - umask\n");
    printf("-p, --parents     se não houver erro, cria a arvore de diretórios.\n");
    printf("-v, --verbose     printa uma mensagem pra cada diretório criado.\n");
    printf("--help     mostra a ajuda e saí.\n");
    printf("--version  mostra a versão do programa e saí.\n");
    exit(EXIT_SUCCESS);
}

void print_version()
{
    printf("Autor: %s\nVersão: %s\n", program_author, program_version);
    exit(EXIT_SUCCESS);
}

int 
make_directory(List dirs, uint32_t mode)
{
    int ret = 0;
    for(int i = 0; i < listNOI(dirs); i++)
    {
        if( (ret = _make_directory((char*) listGetI(dirs, i), mode)) != -1 )
        {
            printf("error\n");
            continue;
        }
    }
}

int 
_make_directory(char *path, uint32_t mode)
{
    char *path_str = (char *) calloc((strlen(path) + 1), sizeof(char));
    if(!path_str)
    {
        fprintf(stderr, "allocation error!");
        exit(1);
    }
    strcpy(path_str, path);

    char **dir_tokens = nimps_split_line(path_str, "/");

    if((parent) && (len >= 1))
    {
        char *tmp_path = strdup(dir_tokens[0]);
        if(!tmp_path)
        {
            errno = ENOMEM;
            return -1;
        }

        for(int i = 0; i < len; i++)
        {
            if( (nimps_mkdir(tmp_path, mode) == -1) )  
            {
                perror(program_name);
                return -1;    
            }

            if(verbose)
            {
                printf("%s: diretório '%s' criado\n", program_name, dir_tokens[i]);
            }

            if( (tmp_path = nimps_make_path(tmp_path, dir_tokens[i])) == NULL )
                return -1;
        }
    } 
    else
    {

        if( verify_path_name(path) )
        {
            if( (nimps_mkdir(path, mode) == -1) ) 
            {
                perror(program_name);
                return -1;    
            }

            if(verbose)
            {
                printf("%s: diretório '%s' criado\n", program_name, path);
            }
        }

        else 
        {
            printf("%s: nome de diretorio inválido. [%s]\n", program_name, path);
            return -1;
        }
    }
    
    return 0; 
}

int main(int argc, char *argv[])
{
    char c = 0;
    List args = listCreate();

    while((c = nimps_getopt(argc, argv, "m:phv")) != -1)
    {
        printf("%c\n", c);
        switch(c)
        {
            case 'm' : mode = atoi(optarg); break;
            case 'p' : parent = 1; break;
            case 'v' : verbose = 1; break;
            case 'h' : print_usage(); break;
            case '#' : 
            {
                
                char *arg = (char*) calloc(strlen(argv[optopt-1]) + 1, sizeof(char));
                if(!arg)
                {
                    fprintf(stderr, "allocation error!");
                    return 1;
                }
                strcpy(arg, argv[optopt-1]);
                //printf("\targ:%s\n", arg);
                
                listIns(arg, listNOI(args)+1, args);
                
                break;
            } 
            case ':' : printf("argumento da flag '%s' inválido.\n", argv[optopt]); print_usage(); break;
            case '?' : printf("flag incorreta inserida.\n"); print_usage(); break;
        }
    }
    
    make_directory(args, mode);
    return 0;
}