#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ArvoreAVL.h"

#define SLOTS 10
#define MAX_LINE_LENGHT 3000

typedef struct
{
    int m;
    ArvAVL** vetor;
}hash_table;

//Protótipos
void init_hash(hash_table*);
void libera_hash(hash_table*);
int funcao_hash(char*, hash_table*);
void processa_arquivo(FILE*, hash_table*);
void insere_tabela(hash_table*, char*, int);

//void imprimir_tabela(hash_table* t);

//Função principal
int main(){

    printf("aaaaaaaaaaa\n");
    hash_table minhaTabela;
    printf("BBBBB\n");
    init_hash(&minhaTabela);
    printf("CCCCCCC\n");

    FILE* f = fopen("/Users/diogobellini/Desktop/UFSCar/ORI/projetoFinalORI/teste.txt","r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo!!");
        libera_hash(&minhaTabela);
        return -1;
    }
    else
    {
        printf("Arquivo aberto com sucesso\n");
    }

    processa_arquivo(f, &minhaTabela);

    fclose(f);

    //imprimir_tabela(&minhaTabela);
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
    else{
        printf("Tabela hash inicializada com %d slots\n", t->m);
    }

    // Inicialize cada ponteiro no array
    for (int i = 0; i < t->m; i++) {
        t->vetor[i] = cria_ArvAVL(); // `cria_ArvAVL` retorna um ponteiro para `ArvAVL
        if (t->vetor[i] == NULL) {
            printf("Erro ao criar a árvore AVL no slot %d\n", i);
            exit(1);
        }
    }
}

void libera_hash(hash_table* t){
    for (int i = 0; i < t->m; i++)
    {
        libera_ArvAVL(t->vetor[i]);
    }
    free(t->vetor);
}

int funcao_hash(char* word, hash_table* t){
    unsigned long numero = 0;  // Usar unsigned long para evitar problemas com valores negativos
    int peso = 1;

    for (size_t i = 0; i < strlen(word); i++) {
        numero = numero * 31 + (unsigned char)word[i];
    }

    // Garantir que o número gerado esteja dentro dos limites da tabela
    return numero % t->m;
}

void insere_tabela(hash_table* t, char* word, int rrn){
    int index = funcao_hash(word, t);
    if (index < 0 || index >= t->m) {
        printf("Índice inválido na tabela hash: %d\n", index);
        return;
    }
    insere_ArvAVL(t->vetor[index], word, rrn);
}

void processa_arquivo(FILE* f, hash_table* t){
    char linha[MAX_LINE_LENGHT];

    long int linha_inicial;
    int rrn = 0;

    while (fgets(linha, sizeof(linha), f))
    {
        printf("Linha lida\n");
        linha[strcspn(linha, "\n")] = '\0';
        
        linha_inicial = ftell(f) - strlen(linha);

        char postagem[MAX_LINE_LENGHT -4];

        sscanf(linha, "%*d,%*d,%[^\n]", postagem);

        if (strcmp(postagem, ""))
        {
            printf("Postagem check\n");
        }
        
        rrn = linha_inicial + (strlen(linha) - strlen(postagem));

        if (rrn != 0)
        {
            printf("RRN check\n");
        }
        
        char* token = strtok(postagem, " ,.!?");
        while (token != NULL)
        {
            while (*token && isspace(*token)) token++;
            if (*token == '\0') { // Se a string estiver vazia após remover espaços
                token = strtok(NULL, " ,.!?");
                continue;
            }
            char* end = token + strlen(token) - 1;
            while (end > token && isspace(*end)) end--;
            *(end + 1) = '\0';

            if (strlen(token) > 0)
            {
                insere_tabela(t, token, rrn);
                //printf("Inserido: %s\n", token);
            }
            token = strtok(NULL, " ,.!?");
        }
    }
    printf("Fim do arquivo\n");
}

// void imprimir_tabela(hash_table* t) {
//     for (int i = 0; i < t->m; i++) {
//         printf("Slot %d:\n", i);
//         imprimir_ArvAVL(t->vetor[i]);
//         printf("\n");
//     }
// }




