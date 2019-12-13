#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "nimps_make_path.h"

#define TOK_BUFSIZE 64
#define MAX_PATH_SIZE 4096


//Divide a linha recebida em tokens definidas pela string 
//delim, o tamanho do vetor de tokens é retornado através 
//da variável len.
unsigned long long len = 0;
char 
**nimps_split_line(char *line, const char* delim)
{
  int bufsize = TOK_BUFSIZE, k = 0;
  char **tokens = (char **) calloc(bufsize, sizeof(char*));
  if (!tokens) 
  {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  char *token = strtok(line, delim);

  while (token != NULL) 
  {
    tokens[k] = token;
    k++;

    if (k >= bufsize) 
    {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) 
      {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, delim);
  }

  tokens[k] = NULL;
  len = k;
  return tokens;
}

//Verifica se os caracteres do nome do path são válidos
int 
verify_path_name(char *s)
{
  for(int i = 0; s[i] != '\0'; i++)
  {
    if(s[i] == '/')
      return 0;
  }

  return 1;
} 

//cria um path unix dado dois paths válidos
char* 
nimps_make_path(char *fst_path, char *snd_path)
{
  char *tmp_path = NULL;

  if( verify_path_name(fst_path) && verify_path_name(snd_path) )
  {
    strcat(fst_path, "/");
    tmp_path = strdup(fst_path);
    if(!tmp_path)
    {
      errno = ENOMEM;
      return NULL;
    }
    
    strcat(tmp_path, snd_path);
    return tmp_path;
  } 
  
  return NULL;
}