#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "nimps_directory_helper.h"
#include "nimps_list.h"

#define DIR_FILE_LIST_BUFF 1024
#define LS_FILE_NAME_SIZE 4096
#define ARCHITECTURE 64

void int_to_bin(int n, char bin[ARCHITECTURE])
{
 int valor = n;
 int i = 0;
    while(valor > 0)
    {int tmp = valor % 2;
        if(tmp == 0)
            bin[i] = '0';
        else
            bin[i] = '1';

        valor /= 2;
        i++;
    }
    bin[i] = '\0';
    return;
}//devolve uma string com o valor binario invetido

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

void 
list_directory()
{
    list files_names = get_dir_file_names(".");
    char absolute_file_name[LS_FILE_NAME_SIZE];
    char bin_value[ARCHITECTURE] = {'\0'};

    struct stat files_stat;

    for(int i = 0; i < count(files_names); i++)
    {
        sprintf(absolute_file_name, "./%s", (char*) get(files_names, i));
        stat(absolute_file_name, &files_stat);
        
        int_to_bin(files_stat.st_mode, bin_value);
        printf("%s ", bin_value);
        
        printf("%zu", files_stat.st_size);
        printf(" %s\n", (char*) get(files_names, i));
    }
}





