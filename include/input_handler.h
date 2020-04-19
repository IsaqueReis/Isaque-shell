//guarda o tamanho do vetor de tokens usados pelo split_line
extern unsigned long long len;

//imprime na tela um vetor de strings
void print_tokens(char **tokens);

//Divide a linha recebida em tokens definidas pela string 
//delim, o tamanho do vetor de tokens é retornado através 
//da variável len.
char **split_line(char *line, const char* delim);

//split_line ignorando a primeira token
char **split_line2(char *line, const char* delim);

//leitura da entrada padrão caractere por caractere
//se o número de caracteres lidos for maior do que 
//o limite do buffer, mais espaço é alocado para string.
//se o usuário teclar RET ou for o final do arquivo, 
//é colocado o delimitador de string '\0' e é retornada
//a string digitada pelo usuário.
char *read_line();

//retorna uma string contendo o diretório de trabalho
//atual.
char *wd_string();

//retorna uma string contendo o nome do usuario real.
char *real_user_string();

//retorna uma string contendo o nome do hostname atual.
char *domain_string();

//processa a entrada do usuário
int process_input(char *input);
