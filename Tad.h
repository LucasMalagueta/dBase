
struct Campo {
    char FieldName[50];
    char Type; // 'N' = Número, 'D' = Data, 'L' = Lógico, 'C' = Caracter, 'M' = Memo
    int Width;
    int Dec;
    
    struct Campo *prox;
};

struct Unidade{
    struct Unidade *top;
    char unid[2];
    struct DBF *arqs;
    struct Unidade *bottom;
};

union Registro {
    float valorN;
    char valorD[10]; // "DD/MM/YYYY"
    char valorL; // 'T' ou 'F'
    char valorC[50];
    char valorM[50];
};

struct DBF{
    char nomeDBF[50];
    char Data[11]; // "DD/MM/YYYY"
    char Hora[6];  // "HH:MM"
    char status; // 'A' = Aberto, 'F' = Fechado
    struct Campo *campos; // Lista encadeada de campos
    union Registro *registros; // Lista encadeada de registros
    struct DBF *prox;
    struct DBF *ant;
};

typedef struct DBF DBF;
typedef struct Campo Campo;
typedef struct Unidade Unidade;

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

void DefaltTo(Unidade **Uni){
    Unidade *C = (Unidade *)malloc(sizeof(Unidade));
    Unidade *D = (Unidade *)malloc(sizeof(Unidade));

    //C:
    C->top = D;
    C->bottom = NULL;
    strcpy(C->unid,"C:");
    C->arqs = NULL;

    //D:
    D->top = C;
    D->bottom = NULL;
    strcpy(D->unid,"D:");
    D->arqs = NULL;

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