//Divide a linha recebida em tokens definidas pela string 
//delim, o tamanho do vetor de tokens é retornado através 
//da variável len.
extern unsigned long long len;
char 
**nimps_split_line(char *line, const char* delim);

//Verifica se os caracteres do nome do path são válidos
int 
verify_path_name(char *s);

//cria um path unix dado dois paths válidos
char* 
nimps_make_path(char *fst_path, char *snd_path);