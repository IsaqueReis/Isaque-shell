#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "../../lib/nimps_mkdir.h"
#include "../../lib/nimps_getopt.h"
#include "../../lib/nimps_make_path.h"
#include "../../lib/nimps_list.h"

#define MAX_ARG_SIZE                                                       4096

int parent = 0;

char program_name[] = "nimps_rmdir";
char program_author[] = "isaqreis";
char program_version[] = "0.0.0.0.0.0.0.0.0.0.0.0.1";

void print_usage()
{
    printf("Uso: rmdir [-p] DIRECTORY...\n");
    printf("Remove diretórios. O diretório deve estar vazio.\n");
    printf("Os diretórios são removidos na ordem em que são especificados.\n");
    printf("\n");
    printf("-p, --parents     remove todos os diretórios especificados no caminho.\n");
    printf("\tExemplo: rmdir -p a/b/c = rmdir a/b/c && rmdir a/b && rmdir a\n");
    printf("--help     mostra a ajuda.\n");
    printf("--version  mostra a versão do programa.\n");
    exit(EXIT_SUCCESS);
}

void print_version()
{
    printf("\tPrograma:%s\n\tAutor: %s\n\tVersão: %s\n",program_name, program_author, program_version);
    exit(EXIT_SUCCESS);
}

void missing_operand()
{
    printf("%s: faltando argumentos\n", program_name);
    printf("Use '%s -h' para mais informação.\n", program_name);
    exit(EXIT_FAILURE);
}

int 
remove_directory(char *path)
{
    char *path_str = NULL;    //string contendo o diretorio
    char *tmp = NULL;         //armazena um diretorio temporário
    char **dir_tokens = NULL; //tokens do diretório (para um diretório composto)

    path_str = (char *) calloc((strlen(path) + 1), sizeof(char));
    if(!path_str)
    {
        fprintf(stderr, "allocation error!");
        exit(1);
    }
    strcpy(path_str, path);

    dir_tokens = nimps_split_line(path_str, "/");

    if((parent) && (len >= 1))
    {
        int k = len - 1;
        for(int i = k; i >= 0; i--)
        {
            tmp = nimps_make_path_from_list(dir_tokens);
            if(!tmp)
            {
                fprintf(stderr, "allocation error!\n");
                exit(1);
            }

            //remove directory
            if( (nimps_rmdir(tmp) == -1) )
                return -1;  
                
            dir_tokens[i] = NULL;
        }
        
    } 

    else
    {
        if( (nimps_rmdir(path) == -1) ) 
            return -1;    
    }
    
    return 0; 
}

void remove_directoryies(list args)
{
    for(int i = 1; i <= count(args); i++)
    {
        if( remove_directory ((char*) get(args, i)) == -1)
            perror(program_name);
    }

}


int main(int argc, char *argv[])
{
    char c = 0;
    list args = list_create();
    
    if(argc <= 1)
        missing_operand();

    while((c = nimps_getopt(argc, argv, "phv")) != -1)
    {
        switch(c)
        {
            case 'p' : parent = 1; break;
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
                
                add(arg, count(args)+1, args);
                
                break;
            }
            case 'h' : print_usage(); break;
            case 'v' : print_version(); break;
            case ':' : printf("argumento de flag inválido.\n"); print_usage(); break;
            case '?' : printf("flag incorreta inserida.\n"); print_usage(); break;
        }
    }

    remove_directoryies(args);
}