//mkdir
//-----
//cria um diretório dado seu path e o seu modo(chmod).
int nimps_mkdir(const char *path, mode_t mode);

//rmdir
//-----
//remove um diretório dado seu caminho. O diretório só é removido se estiver vazio
int 
nimps_rmdir(const char *dir);