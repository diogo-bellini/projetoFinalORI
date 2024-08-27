//Defini��o do tipo do iterator
struct iterator{
    int valor;
    struct iterator *prox;
};

typedef struct NO* ArvAVL;

ArvAVL* cria_ArvAVL_set();
void libera_ArvAVL_set(ArvAVL *raiz);
int insere_ArvAVL_set(ArvAVL *raiz, int data);
int remove_ArvAVL_set(ArvAVL *raiz, int valor);
int estaVazia_ArvAVL_set(ArvAVL *raiz);
int altura_ArvAVL_set(ArvAVL *raiz);
int totalNO_ArvAVL_set(ArvAVL *raiz);
int consulta_ArvAVL_set(ArvAVL *raiz, int valor);
void preOrdem_ArvAVL_set(ArvAVL *raiz);
void emOrdem_ArvAVL_set(ArvAVL *raiz);
void posOrdem_ArvAVL_set(ArvAVL *raiz);

void iterator_ArvAVL_set(ArvAVL *raiz, struct iterator **iter);
