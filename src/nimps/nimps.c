#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "../lib/nimps_make_path.h"
#include "../lib/nimps_directory_helper.h"
#include "../lib/nimps_list.h"
#include "../lib/nimps_make_path.h"

#define PATH_MAX 4096
#define INPUT_BUFF_SIZE 4096
#define NIMPS_CFG_FILE "nimps.cfg"
#define NIMPS_HISTORY_FILE "nimps.hist"

char *current_bin, *working_directory, *initial_working_directory;
long shell_it_count = 0;

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
            
        output[cursor] = c;
        cursor++;
    }
}

void write_config_file()
{
    FILE *fp = NULL;
    char *s = NULL;
    char *filename = NULL;
    char static_file_name[] = NIMPS_CFG_FILE;

    if(shell_it_count == 0)
    {
        initial_working_directory = (char*) calloc(PATH_MAX, sizeof(char));
        if(!initial_working_directory)
            allocation_error();

        if( !(initial_working_directory = getcwd(s, PATH_MAX)) )
        {
            perror("getting working directory");
            exit(EXIT_FAILURE);
        }
    
        //tamanho = pathinicial + '/' + filename + '\0' 
        filename 
            = (char*) calloc(((strlen(initial_working_directory) 
                             + strlen(NIMPS_CFG_FILE)) + 50), sizeof(char));
        if(!filename)
            allocation_error();

        filename
            = nimps_make_multiple_path(initial_working_directory, static_file_name);
        printf("file name: %s\n", filename);
    }

    fp = fopen(filename, "w");
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

void att_enviroment()
{
    write_config_file();
    load_config();
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
    write_history(input);

    if(strcmp("exit", input) == 0)
        exit(EXIT_SUCCESS);
    else if(strcmp("pwd", input) == 0)
        printf("%s\n", working_directory);
    else if(strcmp("dir", input) == 0)
    {
        if( print_all_files("/bin") == -1)
            perror("print directoryies");
    }
    else if(strcmp("bin", input) == 0)
        printf("%s\n", current_bin);
    else if(strcmp("cd", input_tokens[0]) == 0)
    {
        printf("nimps cd!\n");

        if(input_tokens[1] == NULL)
            expeted_arg(input_tokens[0]);
        else    
            change_directory(input_tokens[1]);
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

                exec_and_wait(input_tokens, absolute_exec_path);
                num_of_equals++;
            }
        }

        if(num_of_equals == 0)
            printf("%s: comando não encontrado\n", input);
    }

    //atualiza o ambiente
    att_enviroment();
}

int 
main(int argc, char *argv[])
{
    att_enviroment();

    while(true)
    {
        printf("%s$ ", working_directory);
        process_input(buffered_input());

        //conta o numero de iterações do shell
        shell_it_count++;
    }
   
    return 0;
}