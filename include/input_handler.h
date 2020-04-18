//leitura da entrada padrão caractere por caractere
//se o número de caracteres lidos for maior do que 
//o limite do buffer, mais espaço é alocado para string.
//se o usuário teclar RET ou for o final do arquivo, 
//é colocado o delimitador de string '\0' e é retornada
//a string digitada pelo usuário.
char *read_line();
