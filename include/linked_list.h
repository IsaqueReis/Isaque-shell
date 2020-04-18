#include <stdint.h>

//cria uma nova lista encadeada
void* new_list();

//retorna o número de elementos da lista
uint64_t count(void *l);

//destroi a lista encadeada passada como argumento
void list_dispose(void *l);

//retorna o elemento no indice indicado
//se a lista estiver vazia ou o elemento tiver um indice maior 
//que o número total de elementos da lista, retorna NULL
void *find_at_index(void *l, int index);

//retorna o elemento procurado
//se o elemento não existir na lista, retorna NULL
void *find_item(void *l, void *element, int (*cmp) (const void *a, const void *b));

//insere o item na posição(cabeça, cauda ou corpo) da lista
void add_item(void *item, int position, void *l);

//remove o elemento da lista
void remove_item(void *l, void *element, int (*cmp) (const void *a, const void *b));