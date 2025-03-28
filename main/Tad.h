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

void cadastraCampo(DBF **dbf, int *x, int *y);
void exibelinhacampo();
void printCH(char ch);

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
void Create(Unidade **unid, DBF **dbf, char nome[50]) {
    DBF *aux, *atual = NULL;
    char data[11], hr[9], op, ch;
    int x = 7, y = 10, count = 0;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    //Verificar se a unidade existe
    if (*unid != NULL) {
        //Criar DBF
        strftime(data, 11, "%d/%m/%Y", tm_info);
        strftime(hr, 11, "%H:%M:%S", tm_info);
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

        //Criar campos
        dica1(25, "[ENTER] Criar Campo. [Esc] encerrar CREATE");
        gotoxy(7, 19);
        op = getch();
        createCampos();
        fflush(stdin);
        //Iniciar loop campos
        textcolor(BLACK); textbackground(LIGHTGRAY);
        while (op != 27) {
            dica1(0, " ");
            //lOGICA DE CADASTRAR CAMPOS
            exibelinhacampo(&x, &y, &count);
            cadastraCampo(&aux, &x, &y);
            y++;

            dica1(25, "[ENTER] Criar Campo. [Esc] encerrar CREATE");
            gotoxy(7, 19);
            op = getch();
            fflush(stdin);
        }
        textcolor(LIGHTGRAY); textbackground(BLACK);
    }
}

void cadastraCampo(DBF **dbf, int *x, int *y) {
    Campo *atual = NULL, *novo = NULL; // Cria um ponteiro auxiliar para percorrer a lista
    char ch;

    //Checar se o DBF existe
    if (dbf != NULL) {
        novo = (Campo *)malloc(sizeof(Campo));
        gotoxy(*x + 3, *y);
        scanf("%s", novo->FieldName);

        gotoxy(*x + 15, *y);
        ch = toupper(getch());
        novo->Type = ch;
        printCH(ch);

        gotoxy(*x + 26, *y);
        scanf("%d", &novo->Width);

        gotoxy(*x + 31, *y);
        scanf("%d", &novo->Dec);

        novo->prox = NULL;
        novo->Patual = NULL;
        novo->Pdados = NULL;


        atual = (*dbf)->campos;
        //Checar se o DBF tem campos cadastrados. Caso sim atual != NULL
        if (atual != NULL) { 
            //Caso exista campos ir até o ultimo
            while (atual->prox != NULL) {
                atual = atual->prox;
            }
            atual->prox = novo;
        } 
        //Caso DBF não ter nenhum campo, novo será o primeiro
        else {
            (*dbf)->campos = novo;
        }
    }

}

void printCH(char ch) {
    if (ch == 'N') {
        printf("Numeric");
    } else if (ch == 'D') {
        printf("Date");
    } else if (ch == 'L') {
        printf("Logical");
    } else if (ch == 'C') {
        printf("Character");
    } else if (ch == 'M') {
        printf("Memo");
    }
}

void exibelinhacampo(int *x, int *y, int *count) {
    gotoxy(*x, *y);
    //Exibir numero do campo
    textcolor(LIGHTGRAY); textbackground(BLACK); 
    printf("%d  ", ++(*count));
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Exibir barras de preenchimento
    printf("          ");
    print2(*x+15, *y, "         ");
    print2(*x+26, *y, "   ");
    print2(*x+31, *y, "   ");
    gotoxy(*x+3, *y);
}

//3
void Dir(Unidade **unid){
    DBF *aux;

    //Verificar se a unidade existe
    if (*unid != NULL) {
        aux = (*unid)->arqs;

        while(aux != NULL){
            printf("%s",(*unid)->und);
            printf("%s\n",aux->nomeDBF);
            aux = aux->prox;
        }
    }
}

//4
void quit(){
    gotoxy(1, 20);
}

//5
void USE(DBF **aberto, DBF *atual) {
    *aberto = atual;
}

//6
void ListStructure(Unidade *unid, DBF *dbf){
    Campo *campos = dbf->campos;
    int i = 1;
    if(unid !=NULL && unid->arqs !=NULL){
        printf("Structure for database: %s%s\n",unid->und,dbf->nomeDBF);
        printf("Number of the data records: \n");
        printf("Date of the last update: %s\n",dbf->Data);
        printf("Field      Field Name      Type      Width      Dec\n",dbf->Data);

        while(campos != NULL){
            printf("%d      %s      %s      %d      %d\n",i++,campos->FieldName,campos->Type,campos->Width,campos->Dec);
            campos = campos->prox;
        }
    }else{
        printf("Caminho inexistente!\n");
    }
    
}

void insere(char T,Dados **nova);

//7
void Append(DBF **dbf){
    
    Campo *aux = (*dbf)->campos;
    Status *status = (*dbf)->status,*novastatus = NULL;
    Dados *dados = NULL, *nova = NULL;

    nova = (Dados *)malloc(sizeof(Dados));
    novastatus = (Status *)malloc(sizeof(Status));

    //Printa os Campos que existem no arq
    while(aux != NULL){
        printf("%s:\n",aux->FieldName);
        aux = aux->prox;
    }
    aux = (*dbf)->campos;

    while(aux != NULL){

        dados = aux->Pdados;
        while(dados->prox != NULL){
            dados = dados->prox;
            status = status->prox;
        }
        dados->prox = nova;
        nova->prox = NULL;
        insere(aux->Type,&nova);

        aux = aux->prox;
    }
    //preenche e liga a nova caixa status
    status->prox = novastatus;
    novastatus->prox = NULL;
    novastatus->boolean = 1;

}

//MUDAR, POIS O TYPE DA STRUCT CAMPO NÃO É MAIS CHAR E SIM STRING!
void insere(char T,Dados **nova){
    switch (T){
    case 'N':
        scanf("%d", &(*nova)->valorN);
        break;
    
    case 'D':
        gets((*nova)->valorD);
        break;
    
    case 'L':
        (*nova)->valorL = getch();
        break;
    
    case 'C':
        gets((*nova)->valorC);
        break;
    
    case 'M':
        gets((*nova)->valorM);
        break;
    
    default:
        break;
    }
}