struct NO{
    char* word;
    int num_rrn;
    int* vetor_rrn;
    int capacidade_rrn;
    int* tamanho_postagem;

    int altura;
    struct NO *esq;
    struct NO *dir;
};

typedef struct NO* ArvAVL;

ArvAVL* cria_ArvAVL();
void libera_ArvAVL(ArvAVL *raiz);
int insere_ArvAVL(ArvAVL *raiz, char* data, int rrn, int tamanho);
int* consulta_ArvAVL(ArvAVL raiz, char* valor);