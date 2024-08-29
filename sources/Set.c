#include <stdio.h>
#include <stdlib.h>
#include "Set.h" //inclui os Prot�tipos
#include "ArvoreAVL_set.h" //inclui os Prot�tipos

//Defini��o do tipo Set
struct set{
    ArvAVL* arv;
    int qtd;
    struct iterator *iter;
};

Set* criaSet(){
    Set* s = (Set*) malloc(sizeof(Set));
    if(s != NULL){
        s->arv = cria_ArvAVL_set();
        s->qtd = 0;
        s->iter = NULL;
    }
    return s;
}

void liberaSet(Set* s){
    if(s != NULL){
        libera_ArvAVL_set(s->arv);

        struct iterator* no;
        while(s->iter != NULL){
            no = s->iter;
            s->iter = s->iter->prox;
            free(no);
        }

        free(s);
    }
}

int insereSet(Set* s, int num){
    if(s == NULL)
        return 0;

    if(insere_ArvAVL_set(s->arv,num)){
        s->qtd++;
        return 1;
    }else
        return 0;
}

int tamanhoSet(Set* s){
    if(s == NULL)
        return 0;

    return s->qtd;
}

int consultaSet(Set* s, int num){
    if(s == NULL)
        return 0;

    return consulta_ArvAVL_set(s->arv,num);
}

void beginSet(Set *s){
    if(s == NULL)
        return;

    s->iter = NULL;
    iterator_ArvAVL_set(s->arv, &(s->iter));
}

int endSet(Set *s){
    if(s == NULL)
        return 1;
    if(s->iter == NULL)
        return 1;
    else
        return 0;
}

void nextSet(Set *s){
    if(s == NULL)
        return;
    if(s->iter != NULL){
        struct iterator *no = s->iter;
        s->iter = s->iter->prox;
        free(no);
    }
}

void getItemSet(Set *s, int *num){
    if(s == NULL)
        return;
    if(s->iter != NULL)
        *num = s->iter->valor;
}


Set* uniaoSet(Set* A, Set* B){
    if(A == NULL || B == NULL)
        return NULL;
    int x;
    Set *C = criaSet();

    for(beginSet(A); !endSet(A); nextSet(A)){
        getItemSet(A, &x);
        insereSet(C,x);
    }

    for(beginSet(B); !endSet(B); nextSet(B)){
        getItemSet(B, &x);
        insereSet(C,x);
    }

    return C;
}

Set* interseccaoSet(Set* A, Set* B){
    if(A == NULL || B == NULL)
        return NULL;
    int x;
    Set *C = criaSet();

    if(tamanhoSet(A) < tamanhoSet(B)){
        for(beginSet(A); !endSet(A); nextSet(A)){
            getItemSet(A, &x);
            if(consultaSet(B,x))
                insereSet(C,x);
        }
    }else{
        for(beginSet(B); !endSet(B); nextSet(B)){
            getItemSet(B, &x);
            if(consultaSet(A,x))
                insereSet(C,x);
        }
    }
    return C;
}

Set* interseccaoSetNotS1(Set* A, Set* B){ // Realiza a intersecção entre dois conjuntos com um deles negado
    if(A == NULL || B == NULL)
        return NULL;
    int x;
    Set *C = criaSet();

    for(beginSet(B); !endSet(B); nextSet(B)){
        getItemSet(B, &x);
        if(!consultaSet(A,x))
            insereSet(C,x);
    }

    return C;
}