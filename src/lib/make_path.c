#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "make_path.h"

#define TOK_BUFSIZE 64
#define MAX_PATH_SIZE 4096

char 
**nimps_split_line(char *line, const char* delim, int *len)
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
  *len = k;
  return tokens;
}

//non-permited characters: '/'
int verify_path(char *s)
{
  for(int i = 0; s[i] != '\0'; i++)
  {
    if(s[i] == '/')
      return 0;
  }

  return 1;
} 

char 
*clean_path(char *s)
{
  int k = 0;
  int len = 0;
  char buff[MAX_PATH_SIZE] = {'\0'};
  char *output = NULL;
  char **path_parts = nimps_split_line(s, "/.", &len);

  if(strlen(s) <= MAX_PATH_SIZE)
  {
    for(int i = 0; i < len; i++)
    {
      for(int j = 0; j < strlen(path_parts[i]); j++)
      {
        buff[k] = path_parts[i][j];
        k++;
      }
      if( i < len - 1)
        buff[k] = '/';
      k++;
    }
  }

  output = strdup(buff);
  if(!output)
  {
    fprintf(stderr,"allocation error!\n");
    exit(EXIT_FAILURE);
  }

  return output;
}

int 
nimps_make_path(char *fst_path, char *snd_path)
{
  if( verify_path(fst_path) && verify_path(snd_path) )
  {
    strcat(fst_path, "/"); strcat(fst_path, snd_path);
    return 1;
  } 
  
  return -1;
}