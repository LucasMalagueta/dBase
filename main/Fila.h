#define MAX 11

typedef struct {
    char fila[MAX][100];
    int inicio, fim;
} Fila;

void inicializar(Fila *F);
char filaVazia(Fila *F);
char filaCheia(Fila *F);
void retirar(Fila *F);
void inserir(Fila *F, char linha[100]);
void exibir(Fila *F);

void inicializar(Fila *F) {
    F->inicio = -1;
    F->fim = -1;

    for (int i = MAX; i > 0; i--) {
        inserir(F, "");
    }
}

char filaVazia(Fila *F) {
    return F->fim == -1 && F->inicio == -1;
}

char filaCheia(Fila *F) {
    return (F->fim + 1) % MAX == F->inicio;
}

void retirar(Fila *F) {
    if (!filaVazia(F)) {
        if (F->inicio == F->fim) {
            F->inicio = F->fim = -1;
        } else {
            F->inicio = (F->inicio + 1) % MAX; 
        }
    }
}

void inserir(Fila *F, char linha[100]) {
    if (filaCheia(F)) {
        retirar(F);
    }

    if (filaVazia(F)) {
        F->fim = F->inicio = 0;
    } else {
        F->fim = (F->fim + 1) % MAX; 
    }

    strcpy(F->fila[F->fim], linha);
}

void exibir(Fila *F) {
    limparArea(5, 8, 87, 19);
    int y = 8;
    if (!filaVazia(F)) {
        int i = F->inicio;
        do {
            gotoxy(5, y++); printf("%s", F->fila[i]);
            i = (i + 1) % MAX;
        } while (i != (F->fim + 1) % MAX);
    }
}
