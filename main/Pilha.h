struct pilha {
    char linha[100];
    int qtdLinha;
    struct pilha *prox;
};

typedef struct pilha Pilha;

char vazia(Pilha *P) {
    return (P == NULL);
}

char cheia(Pilha *P) {
    return (P->qtdLinha <= 11);
}

void push(Pilha **P, char str[100]) {
    Pilha *aux = (Pilha*)malloc(sizeof(Pilha));
    strcpy(aux->linha, str);
    aux->prox = NULL;

    if (vazia(*P)) {
        *P = aux;
        aux->qtdLinha = 1;
    } else {
        (*P)->prox = aux;
        aux->qtdLinha = (*P)->qtdLinha + 1;
    }
}

void pop(Pilha **P) {
    Pilha *aux = *P;

    if (!vazia(*P)) {
        while(aux->prox->prox != NULL) {
            aux = aux->prox;
        }

        free(aux->prox);
    }

}

void print(Pilha *P) {
    Pilha *aux = P;

    if (!vazia(P)) {
        while(aux->prox != NULL) {
            printf("%s\n", P->linha);
            aux = aux->prox;
        }
    }
}