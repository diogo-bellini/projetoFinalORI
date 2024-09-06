/*
    Diogo Conforti Vaz Bellini 823829
    João Paulo Morais Rangel 820827
    Enzo Yasumasa Hirotani 823839
*/

// Bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ArvoreAVL.h"
#include "Set.h"

// Definições
#define SLOTS 1009
#define MAX_LINE_LENGHT 280
#define TAM_STRING_BUSCA 300
#define DELIMITADOR " ,.!?"
#define MAX_PILHA_SIZE 100
#define MAX_TOKENS 100



typedef struct
{
    int m;
    ArvAVL** vetor;
}hash_table;

//Protótipos
void init_hash(hash_table*); // Função para iniciar tabela hash
void libera_hash(hash_table*); // Função para desalocar tabela hash
int funcao_hash(hash_table*, char*); // Função que retorna o index da palavra na hash
void processa_arquivo(FILE*, hash_table*); // Função de processamento das postagens na hash
void insere_tabela(hash_table*, char*, int); // Função de inserção na hash

Set* buscar_palavra(hash_table*, char*); // Retorna o conjunto de postagens com a palavra especificada
char** tokenize(const char*, int*); // Separa a string de busca em sub-strings
int precedencia(const char*); // Função auxiliar que define a prioridade de operações
int is_operator(const char* ); // Função auxiliar que verifica se a string trata-se de um operador
char** infix_para_postfix(char**, int, int*); // Ordena o conjunto de sub-strings da string de busca para Ordenação Pós-Fixa
Set* avaliar_postfix(hash_table*, char**, int); // Realiza as operações e retorna um conjunto de postagens(seus rrns)
Set* avaliar_expressao(hash_table*, const char*); // Gerencia o tratamento da string de busca, desde sua entrada até o retorno do conjunto especificado
void realizaBusca(hash_table*, char*,FILE*); // Procura pelas postagens na tabela hash e printa-as

//Função principal
int main(){
    hash_table minhaTabela;
    init_hash(&minhaTabela);

    FILE* f = fopen("corpus.csv","r"); // Abertura do arquivo
    if (!f)
    {
        printf("Erro ao abrir o arquivo!!");
        libera_hash(&minhaTabela);
        return -1;
    }

    processa_arquivo(f, &minhaTabela); // Preenchimento da hash

    char opcao = -1;

    while (opcao != '0') // Menu do usuário
    {
        printf("\nEscolha o que deseja fazer:\n0. Sair\n1. Pesquisar\n>> ");
        scanf("%c", &opcao);

        char stringBusca[TAM_STRING_BUSCA];

        switch (opcao)
        {
        case '1':
            printf("\nBuscar: ");
            while ( getchar() != '\n' ); //Limpar o buffer
            if (fgets(stringBusca, TAM_STRING_BUSCA, stdin) != NULL)
            {
                printf("\n");
                stringBusca[strcspn(stringBusca, "\n")] = '\0';
                realizaBusca(&minhaTabela, stringBusca, f);
            }
            else{
                printf("\nFalha na busca!!!\n");
            }

            break;
        case '0':
            printf("\nSaindo...\n");
            break;
        default:
            printf("\nOpção Inválida\n");
            break;
        }
    }
    
    fclose(f); // Fechamento do arquivo

    libera_hash(&minhaTabela); // Liberação da memória

    return 0;
}

//Implementação das funções
void init_hash(hash_table* t){
    t->m = SLOTS;
    t->vetor = (ArvAVL**)malloc(sizeof(ArvAVL*) * t->m);
    if (t->vetor == NULL) {
        exit(1);
    }

    // Inicializa cada ponteiro no array
    for (int i = 0; i < t->m; i++) {
        t->vetor[i] = cria_ArvAVL(); // cria_ArvAVL retorna um ponteiro para ArvAVL
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

int funcao_hash(hash_table* t, char* word){
    unsigned long numero = 0;
    size_t len = strlen(word);

    for (size_t i = 0; i < len; i++) {
        // Multiplicando o valor do caractere pelo seu índice + 1 para dar peso à posição
        numero += (unsigned char)word[i] * (i + 1);
    }

    // Garantir que o número gerado esteja dentro dos limites da tabela
    return numero % t->m;
}

void insere_tabela(hash_table* t, char* word, int rrn){
    int index = funcao_hash(t, word);
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
    int tamanho_postagem = 0;

    while (fgets(linha, sizeof(linha), f)) // Processa linha por linha
    {
        linha[strcspn(linha, "\n")] = '\0'; // Remove o "\n"
        
        linha_inicial = ftell(f) - strlen(linha) - 1; // Posição começo da linha

        char postagem[MAX_LINE_LENGHT -4];

        sscanf(linha, "%*d,%*d,%[^\n]", postagem);
        
        tamanho_postagem = strlen(postagem); // Definição do tamanho da postagem
        rrn = linha_inicial + (strlen(linha) - tamanho_postagem); // Definição do RRN da postagem
        
        // Separando por palavra
        char* token = strtok(postagem, DELIMITADOR);
        while (token != NULL)
        {
            // Remove espaços
            while (*token && isspace(*token)) token++;
            if (*token == '\0') { // Se a string estiver vazia após remover espaços
                token = strtok(NULL, DELIMITADOR);
                continue;
            }
            char* end = token + strlen(token) - 1;
            while (end > token && isspace(*end)) end--;
            *(end + 1) = '\0';

            // Insere palavra na tabela
            if (strlen(token) > 0)
            {
                insere_tabela(t, token, rrn);
            }
            token = strtok(NULL, DELIMITADOR);
        }
    }
}

Set* buscar_palavra(hash_table* t, char* word){
    Set* conjunto;
    ArvAVL no;

    conjunto = criaSet(); // Criação de um novo conjunto vazio
    int indice = funcao_hash(t, word); // Cálculo do índice da tabela hash
    // Verificação se o ponteiro para a lista ligada no índice calculado é nulo
    if (&(t->vetor[indice]) == NULL) {
        printf("Erro: Ponteiro nulo em t->vetor[%d] para a palavra '%s'\n", indice, word);
        return NULL;  // ou o valor apropriado para indicar um erro
    }
    // Consulta na árvore AVL para encontrar a palavra
    no = consulta_ArvAVL(*(t->vetor[indice]), word);
    
    // Se a palavra for encontrada, armazena os RRN no conjunto
    if (no != NULL)
    {
        for(int i = 0; i < no->num_rrn; i++)
            insereSet(conjunto, no->vetor_rrn[i]);
    }

    return conjunto; // Retorna o conjunto com os RRN encontrados ou vazio
}

// Retorna vetor com strings das palavras da busca
char** tokenize(const char* expressao, int* count) {
    char** tokens = malloc(MAX_TOKENS * sizeof(char*));
    if (tokens == NULL) {
        return NULL;
    }
    
    *count = 0;
    int i = 0;

    while (*expressao != '\0') {
        if (*expressao == '(' || *expressao == ')') {// Tratar parênteses
            tokens[i] = malloc(2 * sizeof(char));
            if (tokens[i] == NULL) {// Liberar memória em caso de falha
                for (int j = 0; j < i; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                return NULL;
            }
            tokens[i][0] = *expressao;
            tokens[i][1] = '\0';
            i++;
            expressao++;
        } else if (isalpha(*expressao)) {// Tratar operandos e operadores
            const char* inicio = expressao;
            while (isalpha(*expressao)) {
                expressao++;
            }

            int tamanho = expressao - inicio;
            
            tokens[i] = malloc((tamanho + 1) * sizeof(char));
            if (tokens[i] == NULL) {// Liberar memória em caso de falha
                for (int j = 0; j < i; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                return NULL;
            }

            strncpy(tokens[i], inicio, tamanho);
            tokens[i][tamanho] = '\0';
            i++;
        } else {// Ignorar espaços
            expressao++;
        }
    }

    *count = i;

    return tokens;
}

// Precedência: NOT > AND > OR
int precedencia(const char* operador) {
    if (strcmp(operador, "NOT") == 0) return 3;
    if (strcmp(operador, "AND") == 0) return 2;
    if (strcmp(operador, "OR") == 0) return 1;
    return 0;
}

int is_operator(const char* token) {
    return strcmp(token, "NOT") == 0 || strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0;
}

char** infix_para_postfix(char** tokens, int count, int* postfix_count) {
    char** postfix = (char**) malloc(MAX_PILHA_SIZE * sizeof(char));
    if (postfix == NULL){
        return NULL;
    }
    char* pilha[MAX_PILHA_SIZE];
    int pilha_topo = -1;
    int j = 0;

    for (int i = 0; i < count; i++) {
        if (!is_operator(tokens[i]) && strcmp(tokens[i], "(") != 0 && strcmp(tokens[i], ")") != 0) {  // É um operando
            postfix[j] = tokens[i];
            j++;
        } else if (is_operator(tokens[i])){ // É um operador
            while (pilha_topo != -1 && precedencia(pilha[pilha_topo]) >= precedencia(tokens[i])) {
                postfix[j] = pilha[pilha_topo];
                j++;
                pilha_topo--;
            }

            pilha_topo++;
            pilha[pilha_topo] = tokens[i];  // Empilha o operador
        } else if (strcmp(tokens[i], "(") == 0) {
            pilha_topo++;
            pilha[pilha_topo] = tokens[i];  // Empilha o '('
        } else if (strcmp(tokens[i], ")") == 0) {
            while (pilha_topo != -1 && strcmp(pilha[pilha_topo], "(") != 0) {
                postfix[j] = pilha[pilha_topo];
                j++;
                pilha_topo--;
            }

            pilha_topo--; // Remove '(' da pilha
        } 
    }

    while (pilha_topo != -1) {
        postfix[j] = pilha[pilha_topo]; // Adiciona os operadores a expressão
        j++;
        pilha_topo--;
    }

    *postfix_count = j; // Quantidade de strings da expressão
    
    return postfix; // Retorna expressão pós-fixa
}

Set* avaliar_postfix(hash_table* t, char** postfix, int count) {
    Set* pilha[100]; // Pilha para conjunto das palavras buscadas
    int pilha_topo = -1;
    int not_key = 0; // Booleano para aplicar o NOT na busca
    int set_negado = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(postfix[i], "NOT") == 0) {
            if (pilha_topo < 0) { // Verifica se há pelo menos dois conjuntos na pilha para a operação
                printf("Erro: Expressão inválida para operação AND.\n");
                return NULL;
            }

            not_key = 1; // Altera para verdadeiro

            if (pilha_topo == 0)
                set_negado = 0; // primeiro negado
            else if (pilha_topo == 1){
                if (set_negado == 0){ // os dois negados
                    printf("Erro: Expressão inválida.\n");
                    return NULL;
                } else if (set_negado == -1)
                    set_negado = 1; // o segundo negado
            }
        } else if (strcmp(postfix[i], "AND") == 0) {
            if (pilha_topo < 1) { // Verifica se há pelo menos dois conjuntos na pilha para a operação
                printf("Erro: Expressão inválida para operação AND.\n");
                return NULL;
            }
            // Remove os dois conjuntos do topo da pilha
            Set* set1 = pilha[pilha_topo];
            pilha_topo--; 
            Set* set2 = pilha[pilha_topo];
            pilha_topo--;

            // Realiza a intersecção
            if(not_key){
                pilha[++pilha_topo] = interseccaoSetNot(set1, set2, set_negado);
            } else {
                pilha[++pilha_topo] = interseccaoSet(set1, set2);
            }
            not_key = 0; // Reseta as chaves
            set_negado = -1;
        } else if (strcmp(postfix[i], "OR") == 0) {
            if (pilha_topo < 1) { // Verifica se há pelo menos dois conjuntos na pilha para a operação
                printf("Erro: Expressão inválida para operação OR.\n");
                return NULL;
            }
            // Remove os dois conjuntos do topo da pilha
            Set* set1 = pilha[pilha_topo];
            pilha_topo--;
            Set* set2 = pilha[pilha_topo];
            pilha_topo--;

            // Realiza a união
            if(not_key){ // Operador OR não pode ser combinado com NOT neste contexto
                printf("Erro: Expressão mal formada.\n");
                return NULL;
            } else { 
                pilha[++pilha_topo] = uniaoSet(set1, set2);
            }
        } else { // Busca a palavra na tabela hash e empilha o conjunto resultante
            Set* conjunto = buscar_palavra(t, postfix[i]);
            if (conjunto != NULL)
            {
                pilha[++pilha_topo] = conjunto;
            } else
            {
                pilha[++pilha_topo] = criaSet();
            }
        } 
    }

    // Verifica se a expressão foi totalmente avaliada com um único conjunto final na pilha
    if (pilha_topo != 0) { 
        printf("Erro: Expressão mal formada.\n");
        return NULL;
    }

    return pilha[pilha_topo]; // Retorna o conjunto resultante da avaliação
}

Set* avaliar_expressao(hash_table* t, const char* expressao) {
    int token_count; // Número de tokens encontrados na expressão
    char** tokens = tokenize(expressao, &token_count); // Divide a expressão em tokens e conta quantos foram encontrados

    int postfix_count; // Armazena o número de tokens na expressão postfix
    char** postfix = infix_para_postfix(tokens, token_count, &postfix_count); // Converte os tokens da expressão infixa para postfix

    // Avalia a expressão postfix usando a tabela hash e retorna o conjunto resultante
    Set* resultado = avaliar_postfix(t, postfix, postfix_count);

    // Libera a memória
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);

    return resultado; // Retorna conjunto especificado
}

void realizaBusca(hash_table* t, char* expressao, FILE* f){
    Set* conjunto = criaSet();
    int rrn;

    conjunto = avaliar_expressao(t, expressao); // Realiza o tratamento da expressão

    // Busca na hash pela postagem com esse rrn e printa
    for(beginSet(conjunto); !endSet(conjunto); nextSet(conjunto)){
        char saida[MAX_LINE_LENGHT];

        getItemSet(conjunto, &rrn);
        fseek(f, rrn, SEEK_SET);
        fgets(saida, MAX_LINE_LENGHT * sizeof(char), f);
        printf("%s\n",saida);
    }
}