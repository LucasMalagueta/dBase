struct strdin {
    char letra;
    struct strdin *prox;
};

typedef struct strdin Strdin;

void inicializarSdin(Strdin **ptr);
void inserirSdin(Strdin **str, char info);
void removerSdin(Strdin **str);
void reiniciarSdin(Strdin **ptr);
void exibirSdin(Strdin *str);
int tamanhoSdin(Strdin *ptr);
void copiaSdin(Strdin **str1, Strdin **str2);
void concatSdin(Strdin **str1, Strdin **str2, Strdin **str3);
char* converteStrdin(Strdin *S);

void inicializarSdin(Strdin **ptr) {
    *ptr = NULL;
}

void inserirSdin(Strdin **str, char info) {
    if (info == 8) { //Inseriu um backspace
        removerSdin(str);
    } else {
        Strdin *aux, *atual;
        aux = (Strdin *)malloc(sizeof(Strdin));
        aux->letra = info;
        aux->prox = NULL;

        if (*str == NULL) {
            *str = aux;
        } else {
            atual = *str;
            while(atual->prox != NULL) {
                atual = atual->prox;
            }

            atual->prox = aux;
        }
    }
}

void reiniciarSdin(Strdin **ptr) {
    Strdin *aux, *atual;

    if (*ptr != NULL) {
        atual = *ptr;

        while (atual != NULL) {
            aux = atual;
            atual = atual->prox;
            free(aux);
        }

        *ptr = NULL;
    }
}

void exibirSdin(Strdin *str) {
    Strdin *aux;

    if (str != NULL) {
        aux = str;
        while(aux->prox != NULL) {
            printf("'%c' ", aux->letra);
            aux = aux->prox;
        }

        printf("'%c'\n", aux->letra);
    } else {
        printf("NULL\n");
    }
}

int tamanhoSdin(Strdin *ptr) {
    Strdin *aux = ptr;
    int i = 0;

    while (aux != NULL) {
        aux = aux->prox;
        i++;
    }

    return i;
}

void copiaSdin(Strdin **str1, Strdin **str2) {
    Strdin *aux = *str1;
    reiniciarSdin(str2);

    while(aux != NULL) {
        inserirSdin(str2, aux->letra);
        aux = aux->prox;
    }
}

void concatSdin(Strdin **str1, Strdin **str2, Strdin **str3) {
    Strdin *aux, *ant;
    
    copiaSdin(str1, str3);

    if (*str3 == NULL) {
        *str3 = *str2;
    } else {
        aux = *str3;
    
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
    
        aux->prox = *str2;
    }
}

void removerSdin(Strdin **str) {
    Strdin *atual = *str;
    Strdin *anterior = NULL;

    if (*str != NULL) {
        if (atual->prox == NULL) {
            free(atual);
            *str = NULL;
        } else {
            while (atual->prox != NULL) {
                anterior = atual;
                atual = atual->prox;
            }

            free(atual);
            anterior->prox = NULL;
        }

    }
}

char* converteStrdin(Strdin *S) {
    Strdin *aux;
    int i = 0;
    char *str;
    
    if (S != NULL) {
        aux = S;
        str = (char *)malloc((tamanhoSdin(S) + 1) * sizeof(char));
        
        while(aux != NULL) {
            //Ignora Enter
            if (aux->letra != 13) {
                str[i] = aux->letra;
                i++;
            }
            aux = aux->prox;
        }
        str[i] = '\0';
    }

    return str;
}