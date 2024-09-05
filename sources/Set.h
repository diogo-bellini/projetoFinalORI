//Arquivo Set.h

typedef struct set Set;

Set* criaSet();
void liberaSet(Set* s);
int insereSet(Set* s, int num);
int tamanhoSet(Set* s);
int consultaSet(Set* s, int num);

Set* uniaoSet(Set* A, Set* B);
Set* interseccaoSet(Set* A, Set* B);
Set* interseccaoSetNot(Set* A, Set* B, int k);

void beginSet(Set *s);
int endSet(Set *s);
void nextSet(Set *s);
void getItemSet(Set *s, int *num);
