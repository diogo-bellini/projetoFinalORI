#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ArvoreAVL.h"
#include "Set.h"


#define SLOTS 10
#define MAX_LINE_LENGHT 350

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
void insere_tabela(hash_table*, char*, int, int);

Set* buscar_palavra(hash_table, char*);
char** tokenize(const char*, int*);
int precedencia(const char*);
char** infix_para_postfix(char**, int, int*);
Set* avaliar_postfix(hash_table, char**, int);
Set* avaliar_expressao(hash_table, const char*);
void realizaBusca(hash_table, char*);

//Função principal
int main(){
    hash_table minhaTabela;
    init_hash(&minhaTabela);

    FILE* f = fopen("teste.txt","r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo!!");
        libera_hash(&minhaTabela);
        return -1;
    }
    // else
    // {
    //     printf("Arquivo aberto com sucesso\n");
    // }

    processa_arquivo(f, &minhaTabela);


    int opcao = -1;

    while (opcao != 0)
    {
        printf("\nEscolha o que deseja fazer:\n0. Sair\n1. Pesquisar\n");
        scanf("%d", &opcao);

        char stringBusca[100];

        switch (opcao)
        {
        case 1:
            printf("\nBuscar: ");
            scanf("%s ", stringBusca);
            realizaBusca(minhaTabela, stringBusca);
            break;
        
        case 0:
            printf("\nSaindo...\n");
            break;
        default:
            printf("\nOpção Inválida\n");
            break;
        }
    }
    
    fclose(f);

    libera_hash(&minhaTabela);

    return 0;
}

//Implementação das funções
void init_hash(hash_table* t){
    t->m = SLOTS;
    t->vetor = (ArvAVL**)malloc(sizeof(ArvAVL*) * t->m);
    if (t->vetor == NULL) {
        exit(1);
    }
    // else{
    //     printf("Tabela hash inicializada com %d slots\n", t->m);
    // }

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
    unsigned long numero = 0;
    size_t len = strlen(word);

    for (size_t i = 0; i < len; i++) {
        // Multiplicando o valor do caractere pelo seu índice + 1 para dar peso à posição
        numero += (unsigned char)word[i] * (i + 1);
    }

    // Garantir que o número gerado esteja dentro dos limites da tabela
    return numero % t->m;
}

void insere_tabela(hash_table* t, char* word, int rrn, int tamanho){
    int index = funcao_hash(word, t);
    if (index < 0 || index >= t->m) {
        printf("Índice inválido na tabela hash: %d\n", index);
        return;
    }
    insere_ArvAVL(t->vetor[index], word, rrn, tamanho);
}

void processa_arquivo(FILE* f, hash_table* t){
    char linha[MAX_LINE_LENGHT];

    long int linha_inicial;
    int rrn = 0;

    while (fgets(linha, sizeof(linha), f))
    {
        //printf("Linha lida\n");
        linha[strcspn(linha, "\n")] = '\0';
        
        linha_inicial = ftell(f) - strlen(linha);

        char postagem[MAX_LINE_LENGHT -4];

        sscanf(linha, "%*d,%*d,%[^\n]", postagem);

        // if (strcmp(postagem, ""))
        // {
        //     printf("Postagem check\n");
        // }
        
        rrn = linha_inicial + (strlen(linha) - strlen(postagem));

        // if (rrn != 0)
        // {
        //     printf("RRN check\n");
        // }
        
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
                insere_tabela(t, token, rrn, strlen(postagem));
                //printf("Inserido: %s\n", token);
            }
            token = strtok(NULL, " ,.!?");
        }
    }
    //printf("Fim do arquivo\n");
}

Set* buscar_palavra(hash_table t, char* word){
    Set* conjunto;
    ArvAVL no;

    conjunto = criaSet();
    no = consulta_ArvAVL(*(t.vetor[funcao_hash(word, &t)]), word);

    for(int i = 0; i < no->num_rrn; i++)
        insereSet(conjunto, no->vetor_rrn[i]);

    return conjunto;
}

char** tokenize(const char* expressao, int* count) { //retorna vetor com strings das palavras da busca
    char** tokens = malloc(100 * sizeof(char*));
    *count = 0;

    const char* delimitadores = " ()";
    char* copia = strdup(expressao);
    char* token = strtok(copia, delimitadores);

    while (token != NULL) {
        tokens[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, delimitadores);
    }

    free(copia);
    return tokens;
}

// Precedência: NOT > AND > OR
int precedencia(const char* operador) {
    if (strcmp(operador, "NOT") == 0) return 3;
    if (strcmp(operador, "AND") == 0) return 2;
    if (strcmp(operador, "OR") == 0) return 1;
    return 0;
}

char** infix_para_postfix(char** tokens, int count, int* postfix_count) {
    char** postfix = malloc(count * sizeof(char*));
    char* pilha[100];
    int pilha_topo = -1;
    int j = 0;

    for (int i = 0; i < count; i++) {
        if (isalpha(tokens[i][0])) {
            postfix[j++] = tokens[i];
        } else if (strcmp(tokens[i], "NOT") == 0 || strcmp(tokens[i], "AND") == 0 || strcmp(tokens[i], "OR") == 0) {
            while (pilha_topo >= 0 && precedencia(pilha[pilha_topo]) >= precedencia(tokens[i])) {
                postfix[j++] = pilha[pilha_topo--];
            }
            pilha[++pilha_topo] = tokens[i];
        } else if (strcmp(tokens[i], "(") == 0) {
            pilha[++pilha_topo] = tokens[i];
        } else if (strcmp(tokens[i], ")") == 0) {
            while (pilha_topo >= 0 && strcmp(pilha[pilha_topo], "(") != 0) {
                postfix[j++] = pilha[pilha_topo--];
            }
            pilha_topo--; // Remove '(' da pilha
        }
    }

    while (pilha_topo >= 0) {
        postfix[j++] = pilha[pilha_topo--];
    }

    *postfix_count = j;
    return postfix;
}

Set* avaliar_postfix(hash_table t, char** postfix, int count) {
    Set* pilha[100];
    int pilha_topo = -1;
    int not_key = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(postfix[i], "NOT") == 0) {
            not_key = 1;
        } else if (strcmp(postfix[i], "AND") == 0) {
            Set* set1 = pilha[pilha_topo--];
            Set* set2 = pilha[pilha_topo--];

            if(not_key){
                pilha[++pilha_topo] = interseccaoSetNotS1(set1, set2);
            } else {
                pilha[++pilha_topo] = interseccaoSet(set1, set2);
            }
            not_key = 0;
        } else if (strcmp(postfix[i], "OR") == 0) {
            Set* set1 = pilha[pilha_topo--];
            Set* set2 = pilha[pilha_topo--];

            if(not_key){
                pilha[++pilha_topo] = uniaoSetNotS1(set1, set2);
            } else {
                pilha[++pilha_topo] = uniaoSet(set1, set2);
            }
            not_key = 0;
        } else {
            pilha[++pilha_topo] = buscar_palavra(t, postfix[i]);
        } 
    }

    return pilha[pilha_topo];
}

Set* avaliar_expressao(hash_table t, const char* expressao) {
    int token_count;
    char** tokens = tokenize(expressao, &token_count);

    int postfix_count;
    char** postfix = infix_para_postfix(tokens, token_count, &postfix_count);

    Set* resultado = avaliar_postfix(t, postfix, postfix_count);

    // Liberar memória
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);

    return resultado;
}

void realizaBusca(hash_table t, char* expressao){
    Set* conjunto = criaSet();
    int rrn;

    conjunto = avaliar_expressao(t, expressao);

    printf("Chegou aq");

    if(conjunto != NULL){
        for(beginSet(conjunto); !endSet(conjunto); nextSet(conjunto)){
            getItemSet(conjunto, &rrn);
            //buscar na hash pela postagem com esse rrn e printar
            printf("%d",rrn);
        }
    }


}