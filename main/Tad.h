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

//SET DEFAULT TO
void setDefaltTo(Unidade **unid, char dir[3]);

//CREATE
void Create(Unidade **unid, DBF **dbf, char nome[50]);
void cadastraCampo(DBF **dbf, int *x, int *y);
void exibelinhacampo();
void printCH(char ch);
int verificach(char ch);

//DIR
void Dir(Unidade *unid, Fila *F);
int contaRecords(Status *status);

//QUIT
void quit();

//USE
void USE(DBF **aberto, DBF *atual);

//LIST STRUCTURE
void ListStructure(Unidade *unid, DBF *dbf);

//APPEND
void Append(DBF **dbf);
void insere(char T,Dados **nova);

//CLEAR
void clear();

//Demais funções
DBF* buscaDBF(char str[], DBF *dbf);

//1
void setDefaltTo(Unidade **unid, char dir[3]) {
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
            if((*unid)->bottom != NULL){
                (*unid) = (*unid)->bottom;
            }else{
                (*unid) = (*unid)->top;
            }
            //*unid = ((*unid)->bottom != NULL) ? (*unid)->bottom : (*unid)->top;
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
        dica1(0, " ");
        textcolor(LIGHTGRAY); textbackground(BLACK);
    }
}

void cadastraCampo(DBF **dbf, int *x, int *y) {
    Campo *atual = NULL, *novo = NULL; // Cria um ponteiro auxiliar para percorrer a lista
    char ch;
    int autentico;

    //Checar se o DBF existe
    if (*dbf != NULL) {
        novo = (Campo *)malloc(sizeof(Campo));
        gotoxy(*x + 3, *y);

        scanf("%s", novo->FieldName);
        fflush(stdin);

        gotoxy(*x + 15, *y);
        do{
            ch = toupper(getch());
            autentico = verificach(ch);
        }while(autentico);
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
        } //Caso DBF não ter nenhum campo, novo será o primeiro
        else {
            (*dbf)->campos = novo;
        }
    }

}

int verificach(char ch){
    return ch != 'N' && ch != 'D' && ch != 'L' && ch != 'C' && ch != 'M';
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
void Dir(Unidade *unid, Fila *F) {
    DBF *aux;
    char linha[100];
    
    //Verificar se a unidade existe
    if (unid->arqs != NULL) {
        inserir(F, ". DIR");
        inserir(F, "Database Files\t#Records\tLast Update\tSize");
        aux = unid->arqs;

        while(aux != NULL) {
            sprintf(linha, "%s\t\t%d\t\t%s\t%d", aux->nomeDBF, contaRecords(aux->status), aux->Data, 0);
            inserir(F, linha);
            aux = aux->prox;
        }
    }
}

int contaRecords(Status *status) {
    Status *aux = status;
    int i = 0;
    while(aux != NULL) {
        if(aux->boolean) {
            i++;
        }
        aux = aux->prox;
    }
    return i;
}

//4
void quit() {
    gotoxy(1, 20);
}

//5
void USE(DBF **aberto, DBF *atual) {
    *aberto = atual;
}

//6
void ListStructure(Unidade *unid, DBF *dbf) {
    Campo *campos = dbf->campos;
    int i = 1;
    if(unid !=NULL && unid->arqs !=NULL){
        printf("Structure for database: %s%s\n",unid->und,dbf->nomeDBF);
        printf("Number of the data records: \n");
        printf("Date of the last update: %s\n",dbf->Data);
        printf("Field      Field Name      Type      Width      Dec\n",dbf->Data);

        while(campos != NULL){
            printf("%d      %s      %c      %d      %d\n",i++,campos->FieldName,campos->Type,campos->Width,campos->Dec);
            campos = campos->prox;
        }
    }else{
        printf("Caminho inexistente!\n");
    }
    
}

//7
void Append(DBF **dbf) {
    Campo *campo = (*dbf)->campos;
    Status *status = (*dbf)->status, *novastatus = NULL;
    Dados *dados = NULL, *nova = NULL;
    int i = 8;

    //Se os campos existem
    if(campo != NULL) {
        //Printa os Campos que existem no arq
        while(campo->prox != NULL){
            gotoxy(7, i);
            printf("%s:\n",campo->FieldName);
            campo = campo->prox;
            i++;
        }
        gotoxy(7, i);
        printf("%s:\n", campo->FieldName);

        campo = (*dbf)->campos;
        i = 8;
        //Enquanto existirem campos cadastra um dado para cada
        while(campo != NULL) {
            nova = (Dados *)malloc(sizeof(Dados));
            gotoxy(20, i);
            dados = campo->Pdados;

            //Verificar se o campo tem dados ou se é o primeiro
            if(dados != NULL) {
                while(dados->prox != NULL) {
                    dados = dados->prox;
                }
                dados->prox = nova;
            } else {
                dados = nova;
            }

            //Popular dados
            nova->prox = NULL;
            insere(campo->Type, &nova); //Chama uma captura de input
            fflush(stdin);
            campo = campo->prox;
            i++;
        }

        
        novastatus = (Status *)malloc(sizeof(Status));
        //Verificar se o status é o primeiro
        if(status != NULL) {
            while(status->prox != NULL) {
                status = status->prox;
            }
            status->prox = novastatus;
        } else {
            status = novastatus;
        }
        //preenche e liga a nova caixa status
        status->prox = novastatus;
        novastatus->prox = NULL; 
        novastatus->boolean = 1; 
    }
}

void insere(char T, Dados **nova) {
    switch (T){
    case 'N':
        scanf("%f", &(*nova)->valorN);
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

//8
void list(DBF *dbf, Fila *F) {
    Campo *campo = NULL;
    char linha[100];
    int i;

    if (dbf != NULL) {
        campo = dbf->campos;

        if (campo != NULL) {
            inserir(F, ". LIST");
            //Guardar titulos
            sprintf(linha, "%s", "Record#");
            while(campo != NULL) {
                sprintf(linha, "%s\t%s", linha, campo->FieldName);
                campo = campo->prox;
            }
            inserir(F, linha);

            //Guardar conteudos
            i = 1;
            campo = dbf->campos;
            while(campo != NULL) {
                sprintf(linha, "\t%d", i);
                
                switch (campo->Type) {
                    case 'N':
                        sprintf(linha, "%s\t%d", linha, campo->Pdados->valorN);
                    break;
                    
                    case 'D':
                        sprintf(linha, "%s\t%s", linha, campo->Pdados->valorD);
                    break;
                    
                    case 'L':
                        sprintf(linha, "%s\t%c", linha, campo->Pdados->valorL);
                    break;
                    
                    case 'C':
                        sprintf(linha, "%s\t%s", linha, campo->Pdados->valorC);
                    break;
                    
                    case 'M':
                        sprintf(linha, "%s\t%s", linha, campo->Pdados->valorM);
                    break;
                }
                campo = campo->prox;
            }
            inserir(F, linha);
        }
    }
}

//9
void clear(Fila *F) {
    limparArea(5, 8, 87, 19);
    inicializar(F);
}

DBF* buscaDBF(char str[], DBF *dbf) {
    DBF *aux = dbf, *ret = NULL;
    char flag = 0;

    //Ir para o começo dos DBF
    while(aux->ant != NULL) {
        aux = aux->ant;
    }

    //Procurar o DBF passado
    if (compare(aux->nomeDBF, str)) {
        ret = aux;
    }
    while(aux->prox != NULL) {
        if (compare(aux->nomeDBF, str)) {
            ret = aux;
        }
        aux = aux->prox;
    }

    return ret;
}

//10
void locate(DBF **dbf,char campo[],char dado[]){

    int i = 1;
    
    if((*dbf)->campos != NULL){

        Campo *aux = (*dbf)->campos;

        while(aux !=NULL && !compare(campo,aux->FieldName)){

            aux = aux->prox;
        }
        
        if(compare(campo,aux->FieldName)){

            if(aux->Pdados != NULL){

                Status *status = (*dbf)->status;
                Dados *auxdado = aux->Pdados;
                while(auxdado !=NULL && !compare(campo,auxdado->valorC) && status->boolean){

                    auxdado = auxdado->prox;
                    status = status->prox;
                    i++;
                }

                if(compare(campo,auxdado->valorC))
                    printf("Record = %d",i);
                
                else
                    printf("Registro nao encontrado!");
                
            }

        }

    }
    
}

//11
void gotodado(DBF **dbf,char reg[0]){

    int i = reg[0] - 48;
    //verifica se existe campo no .dbf
    if((*dbf)->campos != NULL){

        Campo *campo = (*dbf)->campos;
        Dados *dado;
        //verifica se o campo e NULL e percorre todos
        while(campo != NULL){
            dado = campo->Pdados;
            //percorre a qntd de vezes que o usuario digitou no campo dados
            for(; i <= 0 && dado != NULL; i--){
                dado = dado->prox;
            }
            if(dado != NULL){
                campo->Patual = dado;
            }
            campo = campo->prox;
        }
    }

}
