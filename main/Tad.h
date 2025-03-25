typedef struct unidade Unidade;
typedef struct dbf DBF;
typedef struct status Status;
typedef struct campo Campo;
typedef union dados Dados;

struct unidade {
    struct unidade *top;
    char und[2];
    DBF *arqs;
    struct unidade *bottom;
};

struct dbf {
    DBF *ant;
    char nomeDBF[50];
    char Data[11]; // "DD/MM/YYYY"
    char Hora[8];  // "HH:MM"
    Status* status; // 'A' = Aberto, 'F' = Fechado
    Campo *campos; // Lista encadeada de campos
    DBF *prox;

    //NAO TEM ESSE CAMPO NA REPRESENTAÇÃO
    union Registro *registros; // Lista encadeada de registros
};


struct status {
    char boolean;
    Status* prox;
};


struct campo {
    Dados* Patual;
    char FieldName[50];
    char Type; // 'N' = Número, 'D' = Data, 'L' = Lógico, 'C' = Caracter, 'M' = Memo
    int Width;
    int Dec;
    Dados* Pdados;
    Campo *prox;
};


union dados {
    float valorN;
    char valorD[10]; // "DD/MM/YYYY"
    char valorL; // 'T' ou 'F'
    char valorC[50];
    char valorM[50];
    Dados* prox;
};

//1
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

//2
void Create(Unidade **unid, DBF **dbf, char nome[50],char data[11],char hr[8]) {
    DBF *aux, *atual = NULL;

    //Preenche o aux com as informaçoes do novo arquivo .DBF
    aux = (DBF *)malloc(sizeof(DBF));
    strcpy(aux->nomeDBF,nome);
    strcpy(aux->Data,data);
    strcpy(aux->Hora,hr);
    aux->campos = NULL;
    aux->status = NULL;

    //Se nao existir arquivo .dbf
    if(*dbf == NULL){
        *dbf = aux;
        aux->ant = NULL;

        (*unid)->arqs = *dbf;
    }
    //Existe arquivos .dbf
    else{
        //Aponta o ultimo arquivo .DBF para o novo arquivo(aux)
        atual = *dbf;
        while(atual ->prox != NULL){
            atual = atual->prox;
        }
        aux->ant = atual;
        atual->prox = aux;
        
    }
    aux->prox = NULL;
}

//3
void Dir(Unidade **unid){
    DBF *aux;

    aux = (*unid)->arqs;
    printf("%s",(*unid)->und);
    while(aux != NULL){
        printf("\\%s\n",aux->nomeDBF);
        aux = aux->prox;
    }
}

//4
void quit(){
    gotoxy(30, 15);
    textcolor(LIGHTRED);
    printf("Encerrando o programa...\n");
    textcolor(WHITE);
    exit;
}

//6
ListStructure(Unidade *unid, DBF *dbf){
    Campo *campos = dbf->campos;
    if(unid !=NULL && unid->arqs !=NULL){
        printf("Structure for database: %s%s\n",unid->und,dbf->nomeDBF);
        printf("Number of the data records: \n");
        printf("Date of the last update: %s\n",dbf->Data);

        while(campos != NULL){
            printf()
        }
    }
    
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