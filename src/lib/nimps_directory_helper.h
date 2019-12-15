#include "nimps_list.h"

#define DIR_FILE_LIST_BUFF 1024
#define LS_FILE_NAME_SIZE 4096
#define PASSWD_MAX_SIZE  4096
#define MAX_NBYTE_SIZE 64
#define SYMBOLIC_UMASK_SIZE 10
#define MAX_UID 11
#define MAX_USERNAME 128
#define MAX_TIMEBUFF 80

list
get_dir_file_names(char *dirname);

int 
print_all_files(char *dirname);

//saida padrao do ls 
// |----------  |1     |root root  |1323              |Jul 26 19:41     |filename
//-|------------|------|-----------|------------------|-----------------|-------------
// |permissoes  |links |dono grupo |tamanho(em bytes) |data modificacao |nome arquivo 
// 
//todas essas informações sao obitidas a partir da struct stat, que contem os
//metadados da estrutura de dados inode representando o objeto do sistemas de
//arquivos que representa o arquivo ou diretório listado.
void
list_directory();

//lista os diretórios recursivamente.
//essa função se preocupa apenas em listar os nomes dos arquivos
//em cada diretório. Se for uma pasta, uma chamada recursiva é
//feita para listar o diretório abaixo. Caso não haja mais 
//diretórios para serem listados a função volta para o nó 
//mais acima da árvore.
//baseada na função no tutorial: 
//https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
void
list_directory_recursively(char *base_directory, const int root);