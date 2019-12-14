#include <stdio.h>
#include <stdlib.h>

#include "nimps_list.h"

struct list
{
    void *p;                //ponteiro para o primeiro elemento
    void *u;                //ponteiro para o último elemento
    int n;                  //numero de elementos na lista
};
typedef struct list* LIST;

struct node
{
    void *i;                //endereço do item a ser guardado
    struct node *next;      //ponteiro para o proximo item
};
typedef struct node* NODE;


list 
list_create ()
{
    LIST l = (LIST) calloc(1, sizeof(LIST));
    l->p = l->u = NULL;
    l->n = 0;

    return l;
}

void 
list_dealloc (list list)
{
    LIST l = (LIST) list;
    NODE current = NULL;            //nó atual
    NODE tmp_node = NULL;          //nó temporário

    if(l == NULL) //lista já está vazia 
        return;
                    
    current = l->p;  //pega o endereço do primeiro nó
    while(current != NULL)
    {
        tmp_node = current->next;   //guarda o endereço do proximo nó em tmp
        free(current);              //desaloca a memoria do nó atual
        current = tmp_node;        //seta o nó atual com o próximo elemento
    }

    free(l);                        //desaloca a memoria do cabeçalho da lista
}

list_item 
get (list list, int p)
{
    LIST l = (LIST) list;
    NODE current = l->p;

    /*se a lista estiver vazia ou o elemento procurado
      tiver um indice maior que o numero de elementos da
      lista*/
    if((l == NULL) || (p > l->n))
        return NULL;
    
    for(int i = 1; i < p; i++)
        current = current->next;
    
    return current->i;
}

int 
add (list_item i, int p, list list)
{
    LIST l = (LIST) list;
    NODE current = NULL; //nó atual
    NODE tmp_node = NULL; //nó temporário

    tmp_node = (NODE) calloc(1, sizeof(NODE));
    if(tmp_node == NULL)
        return 911;

    tmp_node -> i = i;
   
    if(p == 1)  //inserir na cabeça da lista
    {
        tmp_node->next = l->p;  //o proximo elemento será o que antes era o primeiro elemento da lista
        l->p = tmp_node;        //o primeiro elemento da lista se torna o nó com o list_item i

        /*o último elemento está vazio? Se sim sete-o como o nó que criamos, 
         se não, deixe estar como está*/
        l->u = ((l -> u == NULL) ? (tmp_node) : (l->u)); 

    } else if(p > l->n) //se p for maior que o número de elementos, insira na cauda
    {
        tmp_node->next = NULL;     //fim da lista
        current = l->u;            //dá ao nó atual o endereço do ultimo elemento
        if(current == NULL)        //se não houver último elemento
            l->p = l->u = tmp_node;//esse nó será o primeiro e último elemento da lista
        else
            l->u = current->next = tmp_node;  

    } else {//está em algum lugar da lista que não é a cabeça nem a cauda
        current = l->p;
        for(int i = 0; i < p; i++)
            current = current-> next;
        tmp_node->next = current->next;
        current->next = tmp_node;
    }
    l->n++;
    return 0;
}

int 
count (list list)
{
    LIST l = (LIST) list;
    return l->n;
}

list_item 
list_rmv (list_item i, list list)
{
    LIST l  = (LIST) list;
    NODE current = NULL;
    NODE previous = NULL;
    
    previous = l->p;
    if(previous != NULL && previous->i == i)
    {
        l->p = previous->next;
        l->n--;
        free(previous);
        return i;
    }

    current = previous;
    while(current != NULL && current->i != i)
    {
        previous = current;
        current = current->next;
    }
    
    if(current == NULL)
        return NULL;
    
    previous->next = current->next;
    l->n--;
    return current;
}

list_item 
list_search (key_item k, list list, int (*cmp) (const list_item x, const list_item y))
{
    LIST l = (LIST) list;
    NODE current = NULL;
    if(k == NULL || l == NULL)
        return NULL;
    if(l->p == NULL)
        return NULL;
    
    for(current = l->p; current != NULL; current = current->next)
        if(cmp(k, current->i))
            return current->i;
    return NULL;
}
