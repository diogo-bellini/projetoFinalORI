struct NO{
    char* word; // Palavra armazenada
    int num_rrn; // Quantidade de RRNs
    int* vetor_rrn; 
    int capacidade_rrn; // Tamanho do vetor de RRNs
    int* tamanho_postagem;

    int altura;
    struct NO *esq;
    struct NO *dir;
};

typedef struct NO* ArvAVL;

ArvAVL* cria_ArvAVL();
void libera_ArvAVL(ArvAVL *raiz);
int insere_ArvAVL(ArvAVL *raiz, char* data, int rrn, int tamanho);
ArvAVL consulta_ArvAVL(ArvAVL raiz, char* valor);