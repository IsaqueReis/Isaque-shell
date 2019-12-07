//split line in tokens by delim and returns len of tokens array
char 
**nimps_split_line(char *line, const char* delim, int *len);

//clean redundant parts of a path
char 
*clean_path(char *s);

//make a unix path, return -1 in case of error
int 
nimps_make_path(char *fst_path, char *snd_path);