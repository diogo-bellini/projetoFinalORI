#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ArvoreAVL.h"

#define SLOTS 53
#define MAX_LINE_LENGHT 300
#define MAX_WORD_LENGHT 100

typedef struct
{
    int m;
    ArvAVL** vetor;
}hash_table;

//Protótipos
void init_hash(hash_table*);
void libera_hash(hash_table*);
int fucao_hash(char*, hash_table*);
void processa_arquivo(FILE*, hash_table*);
void insere_tabela(hash_table*, char*, int);

void imprimir_tabela(hash_table* t);

//Função principal
int main(){
    hash_table minhaTabela;
    init_hash(&minhaTabela);
    printf("aaaaaaaaaaa");
    FILE* f = fopen("teste.txt","r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo!!");
        libera_hash(&minhaTabela);
        return -1;
    }

    processa_arquivo(f, &minhaTabela);

    fclose(f);

    imprimir_tabela(&minhaTabela);
    libera_hash(&minhaTabela);
    return 0;
}

//Implementação das funções
void init_hash(hash_table* t) {
    t->m = SLOTS;
    t->vetor = (ArvAVL**)malloc(sizeof(ArvAVL*) * t->m);
    if (t->vetor == NULL) {
        exit(1);
    }

    // Inicialize cada ponteiro no array
    for (int i = 0; i < t->m; i++) {
        t->vetor[i] = cria_ArvAVL(); // `cria_ArvAVL` retorna um ponteiro para `ArvAVL`
    }
}

void libera_hash(hash_table* t){
    for (int i = 0; i < t->m; i++)
    {
        libera_ArvAVL(t->vetor[i]);
        free(t->vetor[i]);
    }

    free(t->vetor);
}

int funcao_hash(char* word, hash_table* t){
    int numero = 0;
    int peso = 1;

    for (size_t i = 0; i < strlen(word); i++)
    {
        numero += word[i] * peso;
        peso *= 31;
    }

    return numero % t->m;
}

void insere_tabela(hash_table* t, char* word, int rrn){
    insere_ArvAVL(t->vetor[funcao_hash(word, t)], word, rrn);
}

void processa_arquivo(FILE* f, hash_table* t){
    char linha[MAX_LINE_LENGHT];

    long int linha_inicial;
    int rrn = 0;

    while (fgets(linha, sizeof(linha), f))
    {
        linha_inicial = ftell(f) - strlen(linha);

        char postagem[MAX_LINE_LENGHT -4];

        sscanf(linha, "%*d,%*d,%[^\n]", postagem);

        rrn = linha_inicial + (strlen(linha) - strlen(postagem));

        char* token = strtok(postagem, " ,;.!?&lt;>&gt;()[]{}\"\'\n");
        while (token != NULL)
        {
            char word[MAX_WORD_LENGHT];
            strcpy(word, token);
            word[MAX_WORD_LENGHT - 1] = '\0';

            if (strlen(word) > 0)
            {
                insere_tabela(t, word, rrn);
            }
        }
        token = strtok(NULL, " ,;.!?&lt;>&gt;()[]{}\"\'\n");
    }
}

void imprimir_tabela(hash_table* t) {
    for (int i = 0; i < t->m; i++) {
        printf("Slot %d:\n", i);
        imprimir_ArvAVL(t->vetor[i]);
        printf("\n");
    }
}




