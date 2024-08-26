#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ArvoreAVL.h"

#define SLOTS 10
#define MAX_LINE_LENGHT 350

typedef struct
{
    int m;
    ArvAVL** vetor;
}hash_table;

// typedef struct{
//     int** rrns;
//     int** tamanhos;
// };

//Protótipos
void init_hash(hash_table*);
void libera_hash(hash_table*);
int funcao_hash(char*, hash_table*);
void processa_arquivo(FILE*, hash_table*);
void insere_tabela(hash_table*, char*, int, int);
void remover_parenteses(char*);
void trim_spaces(char *);

//Função principal
int main(){
    hash_table minhaTabela;
    init_hash(&minhaTabela);

    char input[MAX_LINE_LENGHT];
    int index, i = 0;
    char* argumentos[MAX_LINE_LENGHT];
    ArvAVL no;

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

        switch (opcao)
        {
        case 1:
            //lógica de pesquisa
            printf("Escreva sua pesquisa:\n");
            while (getchar() != '\n'); // Limpar o buffer de entrada

            if (fgets(input, sizeof(input), stdin) != NULL)
            {
                input[strcspn(input, "\n")] = '\0';
                remover_parenteses(input);
                trim_spaces(input);

                //printf("%s\n", input);

                for (int j = 0; j < MAX_LINE_LENGHT; j++)
                {
                    argumentos[j] = NULL;
                }

                char *token = strtok(input, " ");
                while (token != NULL)
                {  
                    argumentos[i] = token;
                    i++;
                    token = strtok(NULL, " ");
                }
                i = 0;

                if (strcmp(argumentos[0],"AND") == 0 || strcmp(argumentos[0],"OR") == 0 || strcmp(argumentos[0],"NOT") == 0)
                {
                    printf("\nNão é possível começar a pesquisa com algum operador!!\n");
                    continue;
                }
                else{
                    int k = 0;
                    while (argumentos != NULL)
                    {
                        index = funcao_hash(argumentos[k], &minhaTabela);
                        no = consulta_ArvAVL(*minhaTabela.vetor[index], argumentos[k]);
                        if (no != NULL)
                        {
                            
                        }
                        
                        k += 2;
                    }
                }

                //index = funcao_hash(input, &minhaTabela);

                // no = consulta_ArvAVL(*minhaTabela.vetor[index] , input);
                // if(no != NULL){
                //     printf("RRN nó: %d\n", no->vetor_rrn[0]); 
                // }
                
                

            }
            else
            {
                printf("Erro ao ler a entrada, tente novamente!!!\n");
            }

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
void init_hash(hash_table* t) {
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

int funcao_hash(char* word, hash_table* t) {
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

        //printf("Linha inicial: %ld\n", linha_inicial);

        char postagem[MAX_LINE_LENGHT -4];

        sscanf(linha, "%*d,%*d,%[^\n]", postagem);

        // if (strcmp(postagem, ""))
        // {
        //     printf("Postagem check\n");
        // }
        
        rrn = linha_inicial + (strlen(linha) - strlen(postagem));

        //printf("RRn: %d\n", rrn);

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

void remover_parenteses(char* str) {
    int i, j = 0;
    int tamanho = strlen(str);

    for (i = 0; i < tamanho; i++) {
        if (str[i] != '(' && str[i] != ')') {
            str[j++] = str[i];  // Copia o caractere se não for '(' ou ')'
        }
    }
    str[j] = '\0';  // Termina a string
}

// Função para remover espaços extras de uma string
void trim_spaces(char *str) {
    char *end;

    // Remover espaços à esquerda
    while (*str && isspace((unsigned char)*str)) str++;

    // Se a string está vazia
    if (*str == 0)
        return;

    // Remover espaços à direita
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate a string
    *(end + 1) = 0;
}

// typedef struct {
//     int vetor_rrn[MAX_VETOR];
//     int tamanho;
// } ResultadoBusca;

// ResultadoBusca realizar_operacao(ResultadoBusca r1, ResultadoBusca r2, char operador) {
//     ResultadoBusca resultado;
//     int i, j;
//     resultado.tamanho = 0;

//     if (operador == 'A') { // AND
//         for (i = 0; i < r1.tamanho; i++) {
//             for (j = 0; j < r2.tamanho; j++) {
//                 if (r1.vetor_rrn[i] == r2.vetor_rrn[j]) {
//                     resultado.vetor_rrn[resultado.tamanho++] = r1.vetor_rrn[i];
//                     break;
//                 }
//             }
//         }
//     } else if (operador == 'O') { // OR
//         for (i = 0; i < r1.tamanho; i++) {
//             resultado.vetor_rrn[resultado.tamanho++] = r1.vetor_rrn[i];
//         }
//         for (i = 0; i < r2.tamanho; i++) {
//             int existe = 0;
//             for (j = 0; j < r1.tamanho; j++) {
//                 if (r2.vetor_rrn[i] == r1.vetor_rrn[j]) {
//                     existe = 1;
//                     break;
//                 }
//             }
//             if (!existe) {
//                 resultado.vetor_rrn[resultado.tamanho++] = r2.vetor_rrn[i];
//             }
//         }
//     } else if (operador == 'N') { // NOT
//         for (i = 0; i < r1.tamanho; i++) {
//             int existe = 0;
//             for (j = 0; j < r2.tamanho; j++) {
//                 if (r1.vetor_rrn[i] == r2.vetor_rrn[j]) {
//                     existe = 1;
//                     break;
//                 }
//             }
//             if (!existe) {
//                 resultado.vetor_rrn[resultado.tamanho++] = r1.vetor_rrn[i];
//             }
//         }
//     }

//     return resultado;
// }