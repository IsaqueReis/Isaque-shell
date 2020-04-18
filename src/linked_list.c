#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../include/error_handler.h"

//cabeçalho da lista encadeada simples
typedef struct linked_list_header
{
    void *first;    //ponteiro para o primeiro elemento da lista
    void *last;     //ponteiro para o último elemento da lsita
    uint64_t count; //número de elementos 
} *list;

//estrutura que representa os nós da lista encadeada
typedef struct linked_list_node
{
    void *data;         //endereço do item a ser guardado
    struct node *next;  //ponteiro para o próximo elemento
} *node;

//cria uma nova lista encadeada
list new_list()
{
    list l = (list) calloc(1, sizeof(list));
    l->first = l->last = NULL;
    l->count = 0;

    return l;
}

//retorna o número de elementos da lista
uint64_t count(void *l)
{
    list _l = (list) l;
    return _l->count;
}


//destroi a lista encadeada passada como argumento
void list_dispose(void *l)
{
    list _l = (list) l;         //converte o ponteiro para o tipo list
    node current = NULL;        //guara o nó atual
    node tmp = NULL;            //guarda o próximo nó

    if(_l == NULL)              //a lista já está vazia
        return;
    
    current = _l->first;        //pega o endereço do primeiro nó

    while(current != NULL)
    {
        tmp = current->next;    //guarda o endereço do próximo nó em tmp
        free(current);          //desaloca a memória do nó atual
        current = tmp;          //seta o nó atual para o próximo elemento
    }

    free(_l);                   //desaloca a memória do cabeçalho
}

//retorna o elemento no indice indicado
//se a lista estiver vazia ou o elemento tiver um indice maior 
//que o número total de elementos da lista, retorna NULL
void *find_at_index(void *l, int index)
{
    list _l = (list) l;
    node current = _l->first;

    if( (_l == NULL) || (index > _l->count) )
        return NULL;
    
    for(int i = 0; i < index; i++)
        current = current->next;
    
    return current->data;
}

//retorna o elemento procurado
//se o elemento não existir na lista, retorna NULL
void *find_item(void *l, void *element, int (*cmp) (const void *a, const void *b))
{
    list _l = (list) l;
    node current = _l->first;

    if(cmp(current->data, element) == 0)
        return current->data;
    
    while(current != NULL)
    {
        if(cmp(current->data, element) == 0)
            return current->data;
        current = current->next;
    }

    if(current == NULL)
        return NULL;
    
    return NULL;
}

//insere o item na posição(cabeça, cauda ou corpo) da lista
void add_item(void *item, int position, void *l)
{
    list _l = (list) l;
    node current = NULL;        //nó atual
    node tmp     = NULL;        //nó temporário que será usado para iserir o valor na lista

    tmp = (node) calloc(1, sizeof(node));

    if(!tmp)
        fatal_error("allocation");
    
    tmp->data = item;

    if(position <= 1)                  //inserir na cabeça da lista
    {
        tmp->next = _l->first;         
        _l->first = tmp;

        _l->last = ((_l->last == NULL) ? (tmp) : (_l->last));
    } 

    else if(position > _l->count)      //inserir na cauda da lista
    {
        tmp->next = NULL;
        current = _l->last;

        if(current == NULL)
            _l->first = _l->last = tmp;
        else 
            _l->last = current->next = tmp;
    }
    
    else                               //inserir no corpo da lista
    {
        current = _l->first;
        for(int i = 0; i < position; i++)
            current = current->next;
        tmp->next = current->next;
        current->next = tmp;
    }

    _l->count++;

    return;
}

//remove o elemento da lista
void remove_item(void *l, void *element, int (*cmp) (const void *a, const void *b))
{
    list _l = (list) l;
    node current = NULL;
    node previous = NULL;

    previous = _l->first;
    if(previous != NULL && (cmp(previous->data, element) == 0))
    {
        _l->first = previous->next;
        _l->count--;
        free(previous);

        return;
    }

    current = previous;
    while(current != NULL && (cmp(current->data, element) != 0))
    {
        previous = current;
        current = current->next;
    }

    if(current == NULL)
        return;
    
    previous->next = current->next;
    _l->count--;
    free(current);

    return;
}