#include "nimps_list.h"

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
int 
list_directory();