#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include "../lib/nimps_make_path.h"
#include "../lib/nimps_directory_helper.h"
#include "../lib/nimps_list.h"
#include "../lib/nimps_make_path.h"

#define PATH_MAX 4096
#define INPUT_BUFF_SIZE 4096
#define NIMPS_CFG_FILE "nimps.cfg"
#define NIMPS_HISTORY_FILE "nimps.hist"

char *current_bin, *working_directory;

void allocation_error()
{
    fprintf(stderr, "allocation error!");
    exit(EXIT_FAILURE);
}

void file_error(char *filename)
{
    fprintf(stderr, "error processando o arquivo '%s'.\n", filename);
    perror("nimps");
    exit(EXIT_FAILURE);
}

void expeted_arg(char *builtin)
{
    fprintf(stderr, "argumento esperado para '%s'.\n", builtin);
    return;
}

void set_current_directory()
{
    char *s = (char*) calloc(PATH_MAX, sizeof(char));
    if(!s)
        allocation_error();

    if( !(s = getcwd(s, PATH_MAX)) )
    {
        perror("getting working directory");
        exit(EXIT_FAILURE);
    }

    free(working_directory);
    
    working_directory = (char*) calloc((strlen(s) + 1), sizeof(char));
    if(!working_directory)
        allocation_error();
    strcpy(working_directory, s);
    return;
}

void write_history(char *input)
{
    FILE *fp = fopen(NIMPS_HISTORY_FILE, "a");
    if(!fp)
        file_error(NIMPS_HISTORY_FILE);

    fprintf(fp, "%s\n", input);

    fclose(fp);
}

char *buffered_input()
{
    char *output = (char*) calloc(INPUT_BUFF_SIZE, sizeof(char));
    int c = '@';
    int cursor = 0;

    if(!output)
        allocation_error();

    while((c = getchar()))
    {
        if(cursor >= INPUT_BUFF_SIZE)
        {
            output = realloc(output, (cursor + INPUT_BUFF_SIZE));
            if(!output)
                allocation_error();
        }

        if(c == '\n' || c == EOF)
        {
            output[cursor] = '\0';
            return output;
        }

        if(c == '\t')
            exit(EXIT_FAILURE);
            
        output[cursor] = c;
        cursor++;
    }
}

void write_config_file()
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
        allocation_error();

    if( !(s = getcwd(s, PATH_MAX)) )
    {
        perror("getting working directory");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "$WD=%s\n", s);
    fprintf(fp, "$BIN=/bin\n");
    
    fclose(fp);

    return;
}


//carrega as variaveis de configuracao do arquivo
void load_config()
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

    while( (fscanf(fp, "%s", s)) != EOF )
    {
        char **tokens = nimps_split_line(s, "=");

        if( strcmp(tokens[0], "$WD") == 0 )
        {
            working_directory = (char*) calloc(strlen(tokens[1] + 1), sizeof(char));
            if(!working_directory)
                allocation_error();

            strcpy(working_directory, tokens[1]);
        }

        else if( strcmp(tokens[0], "$BIN") == 0 )
        {
            current_bin = (char*) calloc(strlen(tokens[1] + 1), sizeof(char));
            if(!working_directory)
                allocation_error();

            strcpy(current_bin, tokens[1]);
        }
        
    }

    return;
}

//baseada na versao de https://brennan.io/2015/01/16/write-a-shell-in-c/
int 
exec_and_wait(char **args, char *path)
{
    pid_t pid, wpid;
    int status = 0;

    pid = fork();
    if(pid == 0)
    {
        //executa o arquivo
        if( execvp(path, args) == -1)
            perror("nimps");
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        perror("nimps");
    }
    else 
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int 
change_directory(char *path_to)
{
    int ret = 0;
    if( (ret = chdir(path_to)) != 0)
        return ret;
}

int process_input(char *input)
{
    //divide a input do usuario em tokens
    char **input_tokens = nimps_split_line(input, " ");
    //file descriptors dos redirecionamentos
    int out = 0;
    int in = 0;

    //flags para fazer a desalocação apenas quando houver I/O
    int i_flag = 0;
    int o_flag = 0;

    //salva as stdout e stdin originais
    int saved_stdout = dup(fileno(stdout)); 
    int saved_stdin  = dup(fileno(stdin));

    for(int i = 0; input_tokens[i] != NULL; i++)
    {
        if(strcmp(input_tokens[i], ">") == 0) //se houver um redirecionamento de entrada
        {
            if(input_tokens[i + 1] != NULL)
            {
                out = open(input_tokens[i + 1], O_RDWR|O_CREAT);
                if(out == -1)
                    perror(input_tokens[i + 1]);
                if ( (dup2(out, fileno(stdout))) < 0)//agora a stdout é o arquivo de saida
                    close(out);
               o_flag = 1;
            }
        }

        if(strcmp(input_tokens[i], "<") == 0) //se houver um redirecionamento de entrada
        {
            if(input_tokens[i + 1] != NULL)
            {
                in = open(input_tokens[i + 1], O_RDONLY);
                if(in == -1)
                    perror(input_tokens[i + 1]);
                if ( (dup2(in, fileno(stdin))) < 0)//agora a stdin é o arquivo de entrada
                    close(in);
                i_flag = 1;
            }
        }
    }

    if(strcmp("exit", input) == 0)
        exit(EXIT_SUCCESS);
    else if(strcmp("pwd", input) == 0)
        printf("%s\n", working_directory);
    else if(strcmp("dir", input) == 0)
    {
        if( print_all_files(current_bin) == -1)
            perror("print directoryies");
    }

    else if(strcmp("cd", input_tokens[0]) == 0)
    {
        if(input_tokens[1] == NULL)
            expeted_arg(input_tokens[0]);
        else    
            if ( change_directory(input_tokens[1]) != 0 )
                perror("nimps cd");
    } 

    else if(strcmp("ls", input_tokens[0]) == 0)
    {
        list_directory();
    }
    else
    {
        list exec_files = get_dir_file_names(current_bin);
        int num_of_equals = 0; //conta o numero de matches com a pasta de executaveis
        
        for(int i = 0; i < count(exec_files); i++)
        {
            if(strcmp(input_tokens[0], (char*) get(exec_files, i)) == 0)
            {
                char *absolute_exec_path = 
                    nimps_make_multiple_path(current_bin, input_tokens[0]);
                if(!absolute_exec_path)
                    allocation_error();
                printf("exec_path: %s\n", absolute_exec_path);
                printf("args: ");
                for(int i = 0; input_tokens[i] != NULL; i++)
                    printf("%s ", input_tokens[i]);
                printf("\n");
                exec_and_wait(input_tokens, absolute_exec_path);
                num_of_equals++;
            }
        }

        if(num_of_equals == 0)
            printf("%s: comando não encontrado\n", input);
    }

    //atualiza o ambiente
    set_current_directory();
    //fechar o redirecionamento de saido caso houver
    if(i_flag && o_flag)
    {
        //restaura a antiga stdout e stdin
        dup2(saved_stdin, fileno(stdin)); 
        dup2(saved_stdout, fileno(stdout)); 
        close(saved_stdout);
        close(saved_stdin);
        close(out);
        close(in);
        
    }

    else if(i_flag)
    {
        dup2(saved_stdin, fileno(stdin)); 
        close(saved_stdin);
        close(in);
    } 
    
    else if(o_flag)
    {
        dup2(saved_stdout, fileno(stdout)); 
        close(saved_stdout);
        close(out);
    } 
        
}

int 
main(int argc, char *argv[])
{
    write_config_file();
    load_config();
    while(true)
    {
        printf("%s$ ", working_directory);

        char *s = buffered_input();
        if(strlen(s) <= 0 || !(s))
            continue;
            
        process_input(s);
    }
   
    return 0;
}