#include <stdio.h>
#include <stdlib.h>
#include "ArvoreAVL_set.h" //inclui os Prot�tipos

struct NO{
    int info;
    int altura;
    struct NO *dir;
    struct NO *esq;
};

ArvAVL* cria_ArvAVL_set(){
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO_set(struct NO* no){
    if(no == NULL)
        return;
    libera_NO_set(no->esq);
    libera_NO_set(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvAVL_set(ArvAVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO_set(*raiz);//libera cada n�
    free(raiz);//libera a raiz
}

int altura_NO_set(struct NO* no){
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO_set(struct NO* no){
    return labs(altura_NO_set(no->esq) - altura_NO_set(no->dir));
}

int maior_set(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

int altura_ArvAVL_set(ArvAVL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = altura_ArvAVL_set(&((*raiz)->esq));
    int alt_dir = altura_ArvAVL_set(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return(alt_dir + 1);
}

int consulta_ArvAVL_set(ArvAVL *raiz, int valor){
    if(raiz == NULL){
        printf("Raiz é NULL\n");
        return 0;
    }
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->info){
            return 1;
        }
        if(valor > atual->info)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

//=================================
void RotacaoLL_set(ArvAVL *A){//LL
    //printf("RotacaoLL\n");
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior_set(altura_NO_set((*A)->esq),altura_NO_set((*A)->dir)) + 1;
    B->altura = maior_set(altura_NO_set(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR_set(ArvAVL *A){//RR
    //printf("RotacaoRR\n");
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior_set(altura_NO_set((*A)->esq),altura_NO_set((*A)->dir)) + 1;
    B->altura = maior_set(altura_NO_set(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR_set(ArvAVL *A){//LR
    RotacaoRR_set(&(*A)->esq);
    RotacaoLL_set(A);
}

void RotacaoRL_set(ArvAVL *A){//RL
    RotacaoLL_set(&(*A)->dir);
    RotacaoRR_set(A);
}

int insere_ArvAVL_set(ArvAVL *raiz, int valor){
    int res;
    if(*raiz == NULL){//�rvore vazia ou n� folha
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0;

        novo->info = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct NO *atual = *raiz;
    if(valor < atual->info){
        if((res = insere_ArvAVL_set(&(atual->esq), valor)) == 1){
            if(fatorBalanceamento_NO_set(atual) >= 2){
                if(valor < (*raiz)->esq->info ){
                    RotacaoLL_set(raiz);
                }else{
                    RotacaoLR_set(raiz);
                }
            }
        }
    }else{
        if(valor > atual->info){
            if((res = insere_ArvAVL_set(&(atual->dir), valor)) == 1){
                if(fatorBalanceamento_NO_set(atual) >= 2){
                    if((*raiz)->dir->info < valor){
                        RotacaoRR_set(raiz);
                    }else{
                        RotacaoRL_set(raiz);
                    }
                }
            }
        }else{
            //printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior_set(altura_NO_set(atual->esq),altura_NO_set(atual->dir)) + 1;

    return res;
}

void iterator_ArvAVL_set(ArvAVL *raiz, struct iterator **iter){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        iterator_ArvAVL_set(&((*raiz)->esq),iter);

        struct iterator* no;
        no = (struct iterator*) malloc(sizeof(struct iterator));
        no->valor = (*raiz)->info;
        no->prox = *iter;
        *iter = no;

        iterator_ArvAVL_set(&((*raiz)->dir),iter);
    }
}