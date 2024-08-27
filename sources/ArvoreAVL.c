#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreAVL.h" //inclui os Prot�tipos

ArvAVL* cria_ArvAVL() {
    ArvAVL* raiz = (ArvAVL*)malloc(sizeof(ArvAVL));
    if (raiz != NULL) {
        *raiz = NULL;
    }

    return raiz;
}

void libera_NO(struct NO* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no->word); // Libera a memória alocada para a word
    free(no->vetor_rrn); // Libera a memória alocada para o vetor de RRNs
    free(no);
    no = NULL;
}

void libera_ArvAVL(ArvAVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);//libera cada n�
    *raiz = NULL;
    free(raiz);//libera a raiz
}

int altura_NO(struct NO* no){
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO(struct NO* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

ArvAVL consulta_ArvAVL(ArvAVL raiz, char* valor) {
    if (raiz == NULL)
        return NULL;
    struct NO* atual = raiz;
    while (atual != NULL) {
        int cmp = strcmp(valor, atual->word);
        if (cmp == 0) {
            return atual; // Palavra encontrada
        }
        if (cmp > 0) {
            atual = atual->dir; // Palavra é maior, vá para a direita
        } else {
            atual = atual->esq; // Palavra é menor, vá para a esquerda
        }
    }
    return NULL; // Palavra não encontrada
}

//=================================
void RotacaoLL(ArvAVL *A){//LL
    //printf("RotacaoLL\n");
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvAVL *A){//RR
    //printf("RotacaoRR\n");
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR(ArvAVL *A){//LR
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvAVL *A){//RL
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

int insere_ArvAVL(ArvAVL *raiz, char* valor, int rrn, int tamanho){
    int res;
    if(*raiz == NULL){//�rvore vazia ou n� folha
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL){
            return 0;
        }

        novo->word = (char*) malloc(strlen(valor) + 1); // Aloca memória para a word
        if (novo->word == NULL) {
            free(novo);
            return 0;
        }

        strcpy(novo->word, valor);
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;

        novo->num_rrn = 1;
        novo->capacidade_rrn = 10;
        novo->vetor_rrn = (int*)malloc(sizeof(int) * novo->capacidade_rrn); // Aloca memória para o vetor de RRNs - já inicia com 10 para usar menos realloc
        if (novo->vetor_rrn == NULL) {
            free(novo->word);
            free(novo);
            return 0;
        }
        novo->tamanho_postagem  = (int*)malloc(sizeof(int) * novo->capacidade_rrn);
        if (novo->tamanho_postagem == NULL) {
            free(novo->word);
            free(novo->vetor_rrn);
            free(novo);
            return 0;
        }
        novo->vetor_rrn[0] = rrn;
        novo->tamanho_postagem[0] = tamanho;

        *raiz = novo;
        return 1;
    }

    struct NO *atual = *raiz;
    if(strcmp(valor, atual->word) < 0){
        if((res = insere_ArvAVL(&(atual->esq), valor, rrn, tamanho)) == 1){
            if(fatorBalanceamento_NO(atual) >= 2){
                if(strcmp(valor, (*raiz)->esq->word) < 0){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }
    else
    {
        if(strcmp(valor, atual->word) > 0){
            if((res = insere_ArvAVL(&(atual->dir), valor, rrn, tamanho)) == 1){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if(strcmp(valor, (*raiz)->dir->word) > 0){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{ //Word já existe
            if (atual->num_rrn == atual->capacidade_rrn) // Se vetor de RRN já estiver cheio
            {
                atual->capacidade_rrn *= 2; // Dobra a capacidade
                atual->vetor_rrn = (int*)realloc(atual->vetor_rrn, sizeof(int) * atual->capacidade_rrn);
                if (atual->vetor_rrn == NULL){
                    return 0;
                }
                atual->tamanho_postagem = (int*)realloc(atual->tamanho_postagem, sizeof(int) * atual->capacidade_rrn);
                if (atual->tamanho_postagem == NULL){
                    return 0;
                }
            }
            atual->vetor_rrn[atual->num_rrn] = rrn;
            atual->tamanho_postagem[atual->num_rrn] = tamanho;
            atual->num_rrn++;

            return 1;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}