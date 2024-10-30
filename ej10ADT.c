#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "./ej10ADT.h"

#define BLOQUE 10

typedef struct node{
    elemType elem;
    size_t cant;
    struct node * tail;
} TNode;

typedef TNode * TList;

typedef struct bagCDT{
    TList list;
    cmp fun;
    size_t dim;
} bagCDT;

void freeBag( bagADT bag) {
}
 
/* Retorna un nuevo bag de elementos genéricos. Al inicio está vacío */
bagADT newBag( cmp fun ){
    bagADT newBag = malloc(sizeof(bagCDT));
    newBag->list = NULL;
    newBag->dim = 0;
    newBag->fun = fun;
    return newBag;
}
 
static TList
addRec( TList list, elemType elem, unsigned* count, cmp fun){
    int c;
    if ( list == NULL || (c = fun(elem, list->elem)) < 0 ){
        TList newNode = malloc(sizeof(TNode));
        newNode->cant = 1;
        newNode->elem = elem;
        newNode->tail = list;
        *count = newNode->cant;
        return newNode;
    }
    if ( c > 0 ){
        list->tail = addRec(list->tail, elem, count, fun);
        return list;
    } else {
        list->cant++;
        *count = list->cant;
        return list;
    }
}

/* Inserta un elemento. Retorna cuántas veces está
** elem en el conjunto luego de haberlo insertado (p.e. si es la primera inserción retorna 1).
*/
unsigned int add(bagADT bag, elemType elem){
    unsigned count;
    bag->list = addRec(bag->list, elem, &count, bag->fun);
    return count;
}

static TList
deleteRec( TList list, elemType elem, unsigned* count, cmp fun){
    int c;
    if ( list == NULL || (c = fun(elem, list->elem)) < 0 ){
        *count = 0;
        return list;
    }
    if ( c > 0 ){
        list->tail = deleteRec(list->tail, elem, count, fun);
        return list;
    } else {
        if ( list->cant == 1 ){
            TList aux = list->tail;
            *count = 0;
            free(list);
            return aux;
        } else {
            list->cant--;
            *count = list->cant;
            return list;
        }
    }
}

/* Remueve una aparición de un elemento. Retorna cuántas veces está
** elem en el conjunto luego de haberlo borrado
*/
unsigned int delete(bagADT bag, elemType elem){
    unsigned count;
    bag->list = deleteRec(bag->list, elem, &count, bag->fun);
    return count;
}

/* Retorna cuántas veces aparece el elemento en el bag */
unsigned int count(const bagADT bag, elemType elem){
    TList current = bag->list;
    int howMany = 0;
    while ( current != NULL && !howMany ){
        if ( bag->fun(elem, current->elem) == 0 )
            howMany = current->cant;
        else 
            current = current->tail;
    }
    return howMany;
}
 
/* Retorna la cantidad de elementos distintos que hay en el bag */
unsigned int size(const bagADT bag){
    TList current = bag->list;
    int count = 0;
    while ( current != NULL ){
        count++;
        current = current->tail;
    }
    return count;
}
 
/* Retorna el elemento que aparece más veces. Si hay más de uno 
** con esa condición, retorna cualquiera de los dos. 
** Precondicion: el bag no debe estar vacio. En caso de estar vacío, aborta
** la ejecución
*/
elemType mostPopular(bagADT bag){
    TList current = bag->list;
    assert( current != NULL);
    int max = 0;
    elemType most;
    while ( current != NULL ){
        if ( current->cant > max ){ 
            most = current->elem;
            max = current->cant;
        }
        current = current->tail;
    }
    return most;
}