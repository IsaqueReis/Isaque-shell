#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "nimps_directory_helper.h"
#include "nimps_make_path.h"
#include "nimps_list.h"

#define DIR_FILE_LIST_BUFF 1024
#define LS_FILE_NAME_SIZE 4096
#define PASSWD_MAX_SIZE  4096
#define MAX_NBYTE_SIZE 64
#define SYMBOLIC_UMASK_SIZE 10
#define MAX_UID 11
#define MAX_USERNAME 128
#define MAX_TIMEBUFF 80
#define MAX_LIST_WORDS 10

//built-in strrev()
void nimps_strrev(char *s)
{
    char aux = '@';
    for(int i = 0, len = strlen(s) - 1 ; i < len; i++, len--)
    {
        aux=s[i];
        s[i] = s[len];
        s[len] = aux;
    }

    return;
} 

void 
get_bin_str(int n, char bin[MAX_NBYTE_SIZE])
{
    int valor = n;
    int i = 0;
    while(valor > 0)
    {
        int tmp = valor % 2;
        if(tmp == 0)
            bin[i] = '0';
        else
            bin[i] = '1';

        valor /= 2;
        i++;
    }

    bin[i] = '\0';
    nimps_strrev(bin);
    return;
}

//1000000  111   1  1  1  1  1  1  [bits from st_mode]
//0123456  789   10 11 12 13 14 15 [bit array index]
//-------  rwx   r  w  x  r  w  x  [mask array]
//filetype owher group    others   [type of permission]
//
//a conversão do array de bits obitidos do valor st_mode
//é feita conforme descrito na tabela acima.
//os 7 primeiros bits são desconsiderados pois servem para
//determinar o tipo de arquivo. Os outros 9 bits restantes
//são as permissões para respectivamente o dono, grupo e 
//os outros.
char 
*make_symbolic_umask(mode_t st_mode, char umask[SYMBOLIC_UMASK_SIZE + 1])
{
    char bit_string[MAX_NBYTE_SIZE + 1] = {'\0'};
    int bit_string_indexer = 7;
    int umask_indexer = 1;
    
    get_bin_str(st_mode, bit_string);

    //setando os valores padroes para umask
    for(int i = 0; i < 10; i++)
    {
        umask[i] = '-';
        umask[i + 1] = '\0'; 
    }

    //define o tipo de arquivo 
    if(S_ISBLK(st_mode))
        umask[0] = 'b';
    else if(S_ISCHR(st_mode))
        umask[0] = 'c';
    else if(S_ISDIR(st_mode))
        umask[0] = 'd';
    else if(S_ISFIFO(st_mode))
        umask[0] = 'p';
    else if(S_ISLNK(st_mode))
        umask[0] = 'l';
    else 
        umask[0] = '-';

    //3 iterações: dono, grupo e outros 
    for(int i = 0; i < 3; i++)
    {
        if(bit_string[bit_string_indexer] == '1')
            umask[umask_indexer] = 'r';
        if(bit_string[bit_string_indexer + 1] == '1')
            umask[umask_indexer + 1] = 'w';
        if(bit_string[bit_string_indexer + 2] == '1')
            umask[umask_indexer + 2] = 'x';    

        bit_string_indexer += 3;
        umask_indexer += 3;
    }

    return umask;
}

char**
get_user_info_by_id(uid_t userid)
{
    char **tokens = NULL;
    char buffer[PASSWD_MAX_SIZE] = {'\0'};
    char userid_buff[MAX_UID] = {'\0'};
    FILE *fp = fopen("/etc/passwd", "r");
    
    if(!fp)
    {
        printf("arquivo '/etc/passwd' não encontrado!\n");
        perror("passwd");
        return NULL;
    }

    while( (fscanf(fp, "%s", buffer)) != EOF )
    {
        tokens = nimps_split_line(buffer, ":");
        for(int i = 0; tokens[i] != NULL; i++)
        {
            if(tokens[2] != NULL)
            {
                sprintf(userid_buff, "%d", userid);
                if(strcmp(userid_buff, tokens[2]) == 0)
                    return tokens;
            }
        }
    }

    return NULL;
}

list
get_dir_file_names(char *dirname)
{
    list files_list = list_create();
    DIR *d;
    struct dirent *dir;
    char *filename;

    d = opendir(dirname);
    if(!d)
        return NULL;
    while((dir = readdir(d)) != NULL)
    {
        filename = (char*) calloc((strlen(dir->d_name) + 1), sizeof(char));
        if(!filename)
        {
            fprintf(stderr, "allocation error!");
            exit(EXIT_FAILURE);
        }
        
        //ignorar ponto e ponto-ponto
        if( (strcmp(dir->d_name, ".")  == 0) || (strcmp(dir->d_name, "..") == 0) )
            continue;

        strcpy(filename, dir->d_name);
        add(filename, (count(files_list) + 1), files_list);
    }

    closedir(d);
    return files_list;
}

int 
print_all_files(char *dirname)
{
    list files_names = get_dir_file_names(dirname);

    for(int i = 0; i < count(files_names); i++)
        printf("%s\n", (char*) get(files_names, i));
    return 0;
}


//saida padrao do ls 
// |----------  |1     |root root  |1323              |Jul 26 19:41     |filename
//-|------------|------|-----------|------------------|-----------------|-------------
// |permissoes  |links |dono grupo |tamanho(em bytes) |data modificacao |nome arquivo 
// 
//todas essas informações sao obitidas a partir da struct stat, que contem os
//metadados da estrutura de dados inode representando o objeto do sistemas de
//arquivos que representa o arquivo ou diretório listado.
void
list_directory()
{
    char absolute_file_name[LS_FILE_NAME_SIZE];
    char mode_bits[MAX_NBYTE_SIZE] = {'\0'};
    char umask[SYMBOLIC_UMASK_SIZE + 1] = {'\0'};
    struct stat files_stat;
    struct tm ts;
    char time_buff[80];
    char **user_info;
    DIR *d;
    struct dirent *dir;

    d = opendir(".");
    if(!d)
        return;

    while((dir = readdir(d)) != NULL)
    {
        sprintf(absolute_file_name, "./%s", dir->d_name);
        stat(absolute_file_name, &files_stat);

        //printar umask
        make_symbolic_umask(files_stat.st_mode, umask);
        printf("%-10s ", umask);

        printf ("%ld ", files_stat.st_nlink);

        //printar usuario
        if( !(user_info = get_user_info_by_id(files_stat.st_uid)) )
            return;
            
        printf ("%s ", user_info[0]);

        printf("%-8zu ", files_stat.st_size);

        //printar tempo
        ts = *localtime(&files_stat.st_mtime);
        strftime(time_buff, sizeof(time_buff), "%b %d %H:%M", &ts);
        printf("%-12s ", time_buff);

        printf(" %s\n", dir->d_name);
    }

    closedir(d);
    return;
}

//lista os diretórios recursivamente.
//essa função se preocupa apenas em listar os nomes dos arquivos
//em cada diretório. Se for uma pasta, uma chamada recursiva é
//feita para listar o diretório abaixo. Caso não haja mais 
//diretórios para serem listados a função volta para o nó 
//mais acima da árvore.
//baseada na função no tutorial: 
//https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
void
list_directory_recursively(char *base_directory, const int root)
{
    char absolute_file_name[LS_FILE_NAME_SIZE];
    DIR *d;
    struct dirent *dir;

    d = opendir(base_directory);
    if(!d)
        return;

    while((dir = readdir(d)) != NULL)
    {
        if( (dir->d_name[0] != '.') )
        {
            for(int i = 0; i < root; i++)
            {
                if(i % 2 == 0 || i == 0)
                    printf("|");
                else 
                    printf(" ");
            }

            printf("%s\n", dir->d_name);
            sprintf(absolute_file_name, "%s/%s", base_directory, dir->d_name);
            list_directory_recursively(absolute_file_name, root + 2);
        }
    }
    closedir(d);
}





