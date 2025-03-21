struct unidade {
    struct unidade *top;
    char und[2];
    struct dbf *arqs;
    struct unidade *bottom;
};
typedef struct unidade Unidade;

struct dbf {
    struct dbf *ant;
    char nomeDBF[50];
    char Data[11]; // "DD/MM/YYYY"
    char Hora[6];  // "HH:MM"
    struct status* status; // 'A' = Aberto, 'F' = Fechado
    struct campo *campos; // Lista encadeada de campos
    struct dbf *prox;

    //NAO TEM ESSE CAMPO NA REPRESENTAÇÃO
    union Registro *registros; // Lista encadeada de registros
};
typedef struct dbf DBF;

struct status {
    char boolean;
    struct status* prox;
};

struct campo {
    union dados* Patual;
    char FieldName[50];
    char Type; // 'N' = Número, 'D' = Data, 'L' = Lógico, 'C' = Caracter, 'M' = Memo
    int Width;
    int Dec;
    union dados* Pdados;
    struct campo *prox;
};
typedef struct campo Campo;

union dados {
    float valorN;
    char valorD[10]; // "DD/MM/YYYY"
    char valorL; // 'T' ou 'F'
    char valorC[50];
    char valorM[50];
    union dados* prox;
};
typedef union dados Dados;

void setDefaltTo(Unidade **unid, char dir[2]) {
    Unidade *C = NULL, *D = NULL;
    
    //Diretorios C e D nao existem
    if (*unid == NULL) {
        D = (Unidade *)malloc(sizeof(Unidade));
        C = (Unidade *)malloc(sizeof(Unidade));
        //D:
        D->top = NULL;
        D->arqs = NULL;
        strcpy(D->und, "D:");
        D->bottom = C;
        //C:
        C->top = D;
        C->arqs = NULL;
        strcpy(C->und, "C:");
        C->bottom = NULL;
        
        if (strcmp(dir, "C:") == 0) {
            *unid = C;
        } else {
            *unid = D;
        }
    }
    //Diretorios C e D existem
    else {
        //Unidade atual diferente da solicitada
        if (strcmp((*unid)->und, dir) != 0) {
            //Aponta para a outra unidade
            *unid = ((*unid)->bottom != NULL) ? (*unid)->bottom : (*unid)->top;
        }
    }
} 

void criarDBF(DBF **dbf, char nome[50],int data,int hr) {
    DBF *aux, *atual;
    FILE *ptr = fopen("Dados.dbf", "wb+");

    aux = (DBF *)malloc(sizeof(DBF));
    strcpy(aux->nomeDBF,nome);
    *aux->Hora = hr;
    *aux->Data = data;

    if(ptr != NULL){
        fprintf(ptr,"%s",aux->nomeDBF);
        fprintf(ptr,"|");
        fprintf(ptr,"%d|",aux->Data);
        fprintf(ptr,"%d|",aux->Hora);

    }

    if(*dbf == NULL){
        *dbf = aux;
    }else{
        atual = *dbf;
        while(atual ->prox != NULL){
            atual = atual->prox;
        }
        aux->ant = atual;
        atual->prox = aux;
        aux->prox = NULL;
        
    }
}

void quit(){
    gotoxy(30, 15);
    textcolor(LIGHTRED);
    printf("Encerrando o programa...\n");
    textcolor(WHITE);
    exit;
}

int Menu1(unsigned int op){
    clrscr();

    textcolor(LIGHTBLUE);
    gotoxy(30, 5);  printf("=====================");
    gotoxy(30, 6);  printf("       MENU         ");
    gotoxy(30, 7);  printf("=====================");
    
    textcolor(YELLOW);
    gotoxy(30, 9);  printf("1 - Opcao 1");
    gotoxy(30, 10); printf("2 - Opcao 2");
    gotoxy(30, 11); printf("0 - QUIT (Sair)");
    
    textcolor(WHITE);
    gotoxy(30, 13);
    printf("Escolha uma opcao: ");
    scanf("%d",&op);

    return op;
}