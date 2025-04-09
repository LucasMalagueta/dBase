typedef struct unidade Unidade;
typedef struct dbf DBF;
typedef struct status Status;
typedef struct campo Campo;
typedef struct dados Dados;
typedef union tipo Tipo;

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

union tipo{
    float valorN;
    char valorL, valorD[10], *valorC, *valorM;
};

struct dados {
    Tipo tipo;
    Dados* prox;
};

//SET DEFAULT TO
void setDefaltTo(Unidade **unid, char dir[3]);

//CREATE
void Create(Unidade **unid, DBF **dbf, char nome[50]);
Campo *cadastraCampo(DBF **dbf, int x, int y);
void exibelinhacampo();
void printCH(char ch);
int verificach(char ch);
int contaCampos(DBF *dbf);
char leChar();
char leNum();

//DIR
void Dir(Unidade *unid, Fila *F);
int contaRecords(DBF *dbf);
int recordAtual(Status *status, Status *pos);

//QUIT
void quit();

//USE
void USE(DBF **aberto, DBF *atual);

//LIST STRUCTURE
void ListStructure(Unidade *unid, DBF *dbf, Fila *F);
char* returnCH(char ch);

//LIST
void list(DBF *dbf, Fila *F); 
void listFor(DBF *dbf, char campoA[], char alvo[], Fila *F);
char compare2(char str[], char str2[]);
char comparaDados(Dados *dado, char tipo, char alvo[]);
void extraiDado(Campo *campo, Dados *dado, char *linha);

//APPEND
void Append(DBF **dbf, Status **pos);
void insere(Campo* campo, Dados **nova, int x, int y);

//CLEAR
void clear();

//LOCATE
void locate(DBF **dbf, char *Nomecampo, char *Nomedado, Fila *F);

//GOTO
void gotodado(DBF **dbf,Status **atual,char reg[]);

//DISPLAY
void display(DBF *dbf, Status *pos, Fila *F);

//EDIT
void edit(DBF **dbf, Status **atual);
void exibeTitulosCampos(Campo *campo);

//DELETE
void deleteUni(DBF **dbf, Status **status);
void deleteAll(DBF **dbf, Status **status);

//RECALL
void reCall(Status **status, Fila *F);
void reCallAll(DBF **dbf,  Fila *F);

//SET DELETED

//PACK

//ZAP
void zap(DBF **dbf);
void freeDado(Campo *campo, Dados *ant);

//MODIFY STRUCTURE
void modifyStrucutre(DBF **dbf);
void exibelinhacampo2(int x, int y, int count, Campo *campo);
void alteraCampo(Campo **campo, int x, int y);

//SORT
void sort(DBF **dbf, char *campoAlvo);
void sortDados(DBF **dbf, Dados *dado, Campo *campo, int pos);
void trocaDados(DBF **dbf, int pos1, int pos2);

//Demais funções
DBF* buscaDBF(char str[], DBF *dbf);
char existeDados(DBF *aberto);

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
    Campo *exibir;
    char data[11], hr[9], op, ch;
    int x = 7, y = 10, count = 1;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(data, 11, "%d/%m/%Y", tm_info);
    strftime(hr, 11, "%H:%M:%S", tm_info);
    
    //Verificar se a unidade existe
    if (*unid != NULL) {
        //Criar DBF
        //Preenche o aux com as informaçoes do novo arquivo .DBF
        aux = (DBF *)malloc(sizeof(DBF));
        sprintf(nome, "%s.DBF", nome);
        strcpy(aux->nomeDBF, nome);
        strcpy(aux->Data, data);
        strcpy(aux->Hora, hr);
        aux->campos = NULL;
        aux->status = NULL;

        //Se nao existir arquivo .dbf
        if(*dbf == NULL) {
            *dbf = aux;
            aux->ant = NULL;
            aux->prox = NULL;
            (*unid)->arqs = *dbf;
        }
        //Existe arquivos .dbf
        else {
            //Aponta o ultimo arquivo .DBF para o novo arquivo(aux)
            atual = *dbf;
            while(atual->prox != NULL){
                atual = atual->prox;
            }
            aux->ant = atual;
            atual->prox = aux;
            aux->prox = NULL;
        }

        *dbf = aux;
        //Criar campos
        dica1(0, "CREATE.");
        dica2(0, "[SPACE] Criar Campo. [Esc] encerrar CREATE.");
        gotoxy(7, 19);
        op = getch();
        createCampos();
        fflush(stdin);
        //Iniciar loop campos
        textcolor(BLACK); textbackground(LIGHTGRAY);
        while (op != 27) {
            //Condição de impresão segunda parte
            if (y >= 18) {
                createCampos2();
                x = 52;
                y = 10;
            }

            //lOGICA DE CADASTRAR CAMPOS
            dica1(0, "Enter Field name.");
            dica2(0, "Field names begin with a letter and may contain letters, digits and underscores");
            baseField(count, contaCampos(aux) + 1); //GUI
            exibelinhacampo(x, y, count); //GUI
            exibir = cadastraCampo(&aux, x, y);
            exibelinhacampo2(x, y, count, exibir); //GUI
            count++;
            y++;

            //Condição de parada if positivo
            if (x == 52 && y >= 18) {
                dica1(0, "[SPACE] Encerrar CREATE.");
                dica2(0, "Campos lotados!");
                gotoxy(7, 19);
                op = getch();
                op = 27;
            } else {
                dica1(0, "CREATE.");
                dica2(0, "[SPACE] Criar Campo. [Esc] encerrar CREATE.");
                gotoxy(7, 19);
                op = getch();
                fflush(stdin);
            }
        }
        textcolor(LIGHTGRAY); textbackground(BLACK);
    }
}

int contaCampos(DBF *dbf) {
    Campo *aux;
    int i = 0;

    if(dbf != NULL){
        aux = dbf->campos;

        while(aux != NULL) {
            i++;
            aux = aux->prox;
        }
    }
    
    return i;
}

//Retorna numeros, alfabeto e Enter
char leChar() {
    char ch;

    do {
        ch = getche();
        fflush(stdin);
    } while ((ch < 48 && ch > 57) || (ch < 65 && ch > 90) ||
             (ch < 97 && ch > 122) || (ch > 13 && ch < 13) ||
             (ch > 95 && ch < 95));

    return ch;
}

char leNum() {
    char ch;

    do {
        ch = getche();
        fflush(stdin);
    } while ((ch < 48 && ch > 57) || (ch > 13 && ch < 13));

    return ch;
}

Campo *cadastraCampo(DBF **dbf, int x, int y) {
    Campo *atual = NULL, *novo = NULL; // Cria um ponteiro auxiliar para percorrer a lista
    char ch, tipo;
    int autentico;
    Strdin *S;
    inicializarSdin(&S);

    //Checar se o DBF existe
    if (*dbf != NULL) {
        novo = (Campo *)malloc(sizeof(Campo));
        gotoxy(x + 3, y);

        // scanf("%s", novo->FieldName);
        // fflush(stdin);
        do {
            ch = leChar();
            inserirSdin(&S, ch);
        } while (tamanhoSdin(S) < 10 && ch != 13);
        strcpy(novo->FieldName, converteStrdin(S));
        reiniciarSdin(&S);

        gotoxy(x + 15, y);
        do {
            tipo = toupper(getch());
        } while(verificach(tipo));
        novo->Type = tipo;
        printCH(tipo);

        //Tipo logico é char, só 1 de tamanho
        if (tipo == 'L') {
            gotoxy(x + 26, y);
            printf("1");
            novo->Width = 1;
        } 
        else {
            //Tipo memo é 50 de tamanho
            if (tipo == 'M') {
                gotoxy(x + 26, y);
                printf("50");
                novo->Width = 50;
            }
            else {
                //Tipo date é 10 de tamanho
                if (tipo == 'D') {
                    gotoxy(x + 26, y);
                    printf("10");
                    novo->Width = 10;
                } 
                else {
                    // scanf("%d", &novo->Width);
                    do {
                        reiniciarSdin(&S);
                        print2(x + 26, y, "   ");
                        gotoxy(x + 26, y);
                        do {
                            ch = leNum();
                            inserirSdin(&S, ch);
                        } while (tamanhoSdin(S) < 2 && ch != 13);
                    } while (atoi(converteStrdin(S)) > 50);
                    novo->Width = atoi(converteStrdin(S));
                    reiniciarSdin(&S);
                }
            }
        }
        
        if (tipo == 'N') {
            //scanf("%d", &novo->Dec);
            do {
                reiniciarSdin(&S);
                print2(x + 31, y, "   ");
                gotoxy(x + 31, y);
                do {
                    ch = leNum();
                    inserirSdin(&S, ch);
                } while (tamanhoSdin(S) < 1);
            } while (atoi(converteStrdin(S)) > 7);
            novo->Dec = atoi(converteStrdin(S));
            reiniciarSdin(&S);
        } else {
            gotoxy(x + 31, y);
            printf("0");
            novo->Dec = 0;
        }

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

    return novo;
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

void exibelinhacampo(int x, int y, int count) {
    gotoxy(x, y);
    //Exibir numero do campo
    textcolor(LIGHTGRAY); textbackground(BLACK); 
    if (count < 9) {
        printf("%d  ", count);
    } else {
        printf("%d ", count);
    }
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Exibir barras de preenchimento
    printf("          ");
    print2(x+15, y, "         ");
    print2(x+26, y, "   ");
    print2(x+31, y, "   ");
    gotoxy(x+3, y);
}

//3
void Dir(Unidade *unid, Fila *F) {
    DBF *aux;
    char linha[100];
    int soma = 0, i = 0;
    
    //Verificar se a unidade existe
    if (unid->arqs != NULL) {
        inserir(F, ". DIR");
        inserir(F, "Database Files\t#Records\tLast Update\tSize");
        aux = unid->arqs;

        while(aux != NULL) {
            sprintf(linha, "%s\t\t%d\t\t%s\t%d", aux->nomeDBF, contaRecords(aux), aux->Data, sizeof(*aux));
            inserir(F, linha);
            soma += sizeof(*aux);
            i++;
            aux = aux->prox;
        }

        inserir(F, " ");
        sprintf(linha, "\t%d Bytes in\t%d files.", soma, i);
        inserir(F, linha);
    }
}

int contaRecords(DBF *dbf) {
    Status *aux;
    int i = 0;

    if(dbf != NULL){

        aux = dbf->status;
        while(aux != NULL) {

            if(aux->boolean) {
                i++;
            }
            aux = aux->prox;
        }

    }
    
    return i;
}

char existeDados(DBF *aberto) {
    Campo *campo = NULL;
    Dados *dado = NULL;

    if (aberto != NULL) {
        campo = aberto->campos;
        if (campo != NULL) {
            dado = campo->Pdados;
            if (dado != NULL) {
                return 1;
            }
        }
    }

    return 0;
}

int recordAtual(Status *status, Status *pos){
    int i = 1;
    
    while (status != NULL) {
        if(status->boolean){
            if (status == pos){
                return i;
            }
            // Só altera a pos se nao estiver excluido
            i++;
        }
        // Avança na lista
        status = status->prox;
    }

    return -1;
}

//4
void quit() {
    gotoxy(1, 24);
}

//5
void USE(DBF **aberto, DBF *atual) {
    *aberto = atual;
}

//6
void ListStructure(Unidade *unid, DBF *dbf, Fila *F) {
    Campo *campos = NULL;
    int i = 1, soma = 0;
    char linha[100];

    if (dbf != NULL && unid != NULL) {
        inserir(F, ". LIST STRUCTURE");
        sprintf(linha, "Structure for database: %s%s", unid->und, dbf->nomeDBF);
        inserir(F, linha);
        sprintf(linha, "Number of the data records: %d", contaRecords(dbf));
        inserir(F, linha);
        sprintf(linha, "Date of the last update: %s", dbf->Data);
        inserir(F, linha);

        campos = dbf->campos;
        if (campos != NULL) {
            sprintf(linha, "%-5s  %-15s %-12s %-5s  %-3s", "Field", "Field Name", "Type", "Width", "Dec");
            inserir(F, linha);

            while(campos != NULL) {
                sprintf(linha, "%5d  %-15s %-12s %5d  %3d", i++, campos->FieldName, returnCH(campos->Type), campos->Width, campos->Dec);
                inserir(F, linha);
                soma += campos->Width;
                campos = campos->prox;
            }
            sprintf(linha, "** Total **%30d", soma);
            inserir(F, linha);
        }
    } 
}

char* returnCH(char ch) {
    if (ch == 'N') {
        return "Numeric";
    } else if (ch == 'D') {
        return "Date";
    } else if (ch == 'L') {
        return "Logical";
    } else if (ch == 'C') {
        return "Character";
    } else if (ch == 'M') {
        return "Memo";
    }
}

//7
void Append(DBF **dbf, Status **pos) {
    Campo *campo = NULL;
    Status *status = NULL, *novastatus = NULL;
    Dados *dados = NULL, *nova = NULL;
    int i = 8;
    
    if(*dbf!=NULL){
        campo = (*dbf)->campos;
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
                //Popular dados
                nova->prox = NULL;
                sizeToSpace(20, i, campo->Width);
                textcolor(BLACK); textbackground(LIGHTGRAY);
                insere(campo, &nova, 20, i); //Chama uma captura de input
                fflush(stdin);
                textcolor(LIGHTGRAY); textbackground(BLACK);

                //Verificar se o campo tem dados ou se é o primeiro
                if (campo->Pdados != NULL) {
                    dados = campo->Pdados;
                    status = (*dbf)->status;
                    while (dados->prox != NULL && status->prox != NULL) {
                        dados = dados->prox;
                        status = status->prox;
                    }
                    dados->prox = nova;
                } else {
                    campo->Patual = nova;
                    campo->Pdados = nova;
                }

                campo = campo->prox;
                i++;
            }
            
            novastatus = (Status *)malloc(sizeof(Status));
            //Verificar se o status é o primeiro
            if((*dbf)->status != NULL) {
                while(status->prox != NULL) {
                    status = status->prox;
                }
                status->prox = novastatus;
            } else {
                (*dbf)->status = novastatus;
                (*pos) = novastatus;
            }
            //preenche e liga a nova caixa status
            novastatus->prox = NULL; 
            novastatus->boolean = 1; 
        }
        baseRec(1, contaRecords((*dbf)));
    }
}

void insere(Campo* campo, Dados **nova, int x, int y) {
    char str[80];
    char T = campo->Type, ch;
    Strdin *S;
    inicializarSdin(&S);

    switch (T) {
        case 'N':
            do {
                ch = leChar();
                inserirSdin(&S, ch);
            } while (tamanhoSdin(S) < campo->Width && ch != 13);

            (*nova)->tipo.valorN = (float)atof(converteStrdin(S));
            reiniciarSdin(&S);
        break;
        
        case 'D':
            //Loop para respeitar o tamanho do campo
            sizeToSpace(x, y, campo->Width);
            do {
                textcolor(BLACK); textbackground(LIGHTGRAY);
                ch = leChar();
                inserirSdin(&S, ch);
                textcolor(LIGHTGRAY); textbackground(BLACK);
            } while (tamanhoSdin(S) < campo->Width && ch != 13);

            strcpy((*nova)->tipo.valorD, converteStrdin(S));
            reiniciarSdin(&S);
        break;
        
        case 'L':
            //Loop para garantir que é uma das opçoes corretas
            sizeToSpace(x, y, campo->Width);
            do {
                textcolor(BLACK); textbackground(LIGHTGRAY);
                ch = toupper(getch());
                fflush(stdin);
            } while(verificach(ch));
            (*nova)->tipo.valorL = ch;
            printCH(ch);
            textcolor(LIGHTGRAY); textbackground(BLACK);
        break;
        
        case 'C':
            //Loop para respeitar o tamanho do campo
            sizeToSpace(x, y, campo->Width);
            do {
                textcolor(BLACK); textbackground(LIGHTGRAY);
                ch = leChar();
                inserirSdin(&S, ch);
                textcolor(LIGHTGRAY); textbackground(BLACK);
            } while (tamanhoSdin(S) < campo->Width && ch != 13);

            (*nova)->tipo.valorC = (char *)malloc((tamanhoSdin(S) + 1) * sizeof(char));
            strcpy((*nova)->tipo.valorC, converteStrdin(S));
            reiniciarSdin(&S);
        break;
        
        case 'M':
            //Loop para respeitar o tamanho do campo
            sizeToSpace(x, y, campo->Width);
            do {
                textcolor(BLACK); textbackground(LIGHTGRAY);
                ch = leChar();
                inserirSdin(&S, ch);
                textcolor(LIGHTGRAY); textbackground(BLACK);
            } while (tamanhoSdin(S) < campo->Width && ch != 13);

            (*nova)->tipo.valorM = (char *)malloc((tamanhoSdin(S) + 1) * sizeof(char));
            strcpy((*nova)->tipo.valorM, converteStrdin(S));
            reiniciarSdin(&S);
        break;
    }
}

//8
void list(DBF *dbf, Fila *F) {
    Campo *campo = NULL;
    Dados *dado = NULL, *nivelDado = NULL;
    Status *status = NULL;
    char linha[100], ch;
    int i, size, flag, rec = 1;

    if (dbf != NULL) {
        campo = dbf->campos;

        if (campo != NULL) {
            dado = campo->Pdados;

            if (dado != NULL) {
                //Guarda comando
                inserir(F, ". LIST");
                //Guardar titulos
                sprintf(linha, "%s", "Record#");
                while(campo != NULL) {
                    //Size serve exclusivamente para logical
                    size = (campo->Width == 1) ? 10 : campo->Width;
                    sprintf(linha, "%s %-*s", linha, size, campo->FieldName);
                    campo = campo->prox;
                }
                inserir(F, linha);

                //Guardar conteudos
                i = 1;
                nivelDado = dbf->campos->Pdados;
                while (nivelDado != NULL) {
                    campo = dbf->campos;
                    sprintf(linha, "%7d", rec);
                    flag = 0;
                    while(campo != NULL) {
                        dado = campo->Pdados;
                        status = dbf->status;

                        for(int x = i; x > 1; x--) {
                            dado = dado->prox;
                            status = status->prox;
                        }
    
                        if (dado != NULL) {
                            if (status->boolean) {
                                extraiDado(campo, dado, linha);
                                flag = 1;
                            }
                        }
                        campo = campo->prox;
                    }
                    if (flag == 1) {
                        inserir(F, linha);
                        rec++;
                    }
                    i++;
                    nivelDado = nivelDado->prox; //Nivel dado e i++ ajudam a definir quantos niveis pra baixo 
                    //é necessario ir para recuperar o reg atual
                }
            }
        }
    }
}

void extraiDado(Campo *campo, Dados *dado, char *linha) {
    switch (campo->Type) {
        case 'N':
            sprintf(linha, "%s %-*.*f", linha, campo->Width, campo->Dec, dado->tipo.valorN);
        break;
        
        case 'D':
            sprintf(linha, "%s %-*s", linha, campo->Width, dado->tipo.valorD);
        break;
        
        case 'L':
            sprintf(linha, "%s %-*s", linha, 10, returnCH(dado->tipo.valorL));
        break;
        
        case 'C':
            sprintf(linha, "%s %-*s", linha, campo->Width, dado->tipo.valorC);
        break;
        
        case 'M':
            sprintf(linha, "%s %-*s", linha, campo->Width, dado->tipo.valorM);
        break;
    }
}

void listFor(DBF *dbf, char campoA[], char alvo[], Fila *F) {
    Campo *campo = NULL;
    Dados *dado = NULL, *nivelDado = NULL;
    char linha[100], ch, flag;
    int i, size;

    if (dbf != NULL) {
        campo = dbf->campos;

        if (campo != NULL) {
            dado = campo->Pdados;

            if (dado != NULL) {
                //Guarda comando
                sprintf(linha, "LIST FOR %s = \"%s\"", campoA, alvo);
                inserir(F, linha);
                //Guardar titulos
                sprintf(linha, "%s", "Record#");
                while(campo != NULL) {
                    size = (campo->Width == 1) ? 10 : campo->Width;
                    sprintf(linha, "%s %-*s", linha, size, campo->FieldName);
                    campo = campo->prox;
                }
                inserir(F, linha);

                //Guardar conteudos
                i = 1;
                nivelDado = dbf->campos->Pdados;
                while (nivelDado != NULL) {
                    //Verificar se o reg atual da match com alvo 
                    flag = 0;
                    campo = dbf->campos;
                    if (campo != NULL) {
                        //Ir até o campo
                        while (campo != NULL && !compare(campo->FieldName, campoA)) {
                            campo = campo->prox;
                        }

                        if (campo != NULL) {
                            dado = campo->Pdados;
                            //Ir até o nivel correto
                            for(int x = i; x > 1; x--) {
                                dado = dado->prox;
                            }

                            //Checar o dado daquela linha da match
                            if (dado != NULL) {
                                flag = comparaDados(dado, campo->Type, alvo);
                            }
                        }
                    }

                    if (flag == 1) {
                        //Resto da logica igual LIST
                        campo = dbf->campos;
                        sprintf(linha, "%7d", i);
                        while(campo != NULL) {
                            dado = campo->Pdados;

                            for(int x = i; x > 1; x--) {
                                dado = dado->prox;
                            }
        
                            if (dado != NULL) {
                                extraiDado(campo, dado, linha);        
                            }
                            campo = campo->prox;
                        }
                        inserir(F, linha);
                    }

                    i++;
                    nivelDado = nivelDado->prox; //Nivel dado e i++ ajudam a definir quantos niveis pra baixo 
                    //é necessario ir para recuperar o reg atual
                }
            }
        }
    }
}

char comparaDados(Dados *dado, char tipo, char alvo[]) {
    char flag = 0;

    switch (tipo) {
        case 'N':
            //Converter string alvo para num e comparar
        break;

        case 'C':
            if (compare2(alvo, dado->tipo.valorC)) {
                flag = 1;
            }
        break;
        
        default:
            break;
    }

    return flag;
}

char compare2(char str[], char str2[]) {
    char flag = 1;
    for(int i = 0; str[i] != '\0'; i++) {
        if (str[i] != str2[i]) {
            flag = 0;
        }
    }
    return flag;
}

//9
void clear(Fila *F) {
    limparArea(5, 8, 87, 19);
    inicializar(F);
}

DBF* buscaDBF(char str[], DBF *dbf) {
    if (dbf == NULL){
        return NULL;
    }
    
    DBF *aux = dbf;

    while (aux->ant != NULL) {
        aux = aux->ant;
    }

    while (aux != NULL) {
        if (compare(aux->nomeDBF, str)) {
            return aux;  
        }
        aux = aux->prox;
    }

    return NULL;
}

//10
void locate(DBF **dbf, char *Nomecampo, char *Nomedado, Fila *F) {
    Status *status = NULL;
    Dados *dado = NULL;
    Campo *campo = NULL;
    int flag = 1, i = 0;
    float val;
    char linha[100];

    //Verifica se existe campo no .dbf
    if ((*dbf) != NULL) {
        campo = (*dbf)->campos;

        if (campo != NULL) {
            //Vai até o campo procurado
            while(campo != NULL && !compare(Nomecampo, campo->FieldName)) {
                campo = campo->prox;
            }
            
            //Verifica se chegou no campo
            if (compare(Nomecampo, campo->FieldName)) {
                sprintf(linha, ". LOCATE FOR %s = \"%s\"", Nomecampo, Nomedado);
                inserir(F, linha);

                if(campo->Pdados != NULL) {
                    dado = campo->Pdados;
                    status = (*dbf)->status;

                    while(dado != NULL && flag) {
                        if (status->boolean) {
                            switch(campo->Type) {
                                case 'L' :
                                    if (dado->tipo.valorL == Nomedado[0]) {
                                        flag = 0;
                                    }
                                break;
    
                                case 'N':
                                    //Converter valor respeitando Dec e casting
                                    sprintf(linha, "%*f", campo->Dec, dado->tipo.valorN);
                                    val = (float) atof(linha);
                                    if (val - (float) atof(Nomedado) == 0) {
                                        flag = 0;
                                    }
                                break;
    
                                case 'D':
                                    if (compare(dado->tipo.valorD, Nomedado)) {
                                        flag = 0;
                                    }
                                break;
    
                                case 'C':
                                    if (compare(dado->tipo.valorC, Nomedado)) {
                                        flag = 0;
                                    }
                                break;
    
                                case 'M':
                                    if (compare(dado->tipo.valorM, Nomedado)) {
                                        flag = 0;
                                    }
                                break;
                            }
                        }

                        dado = dado->prox;
                        status = status->prox;
                        i++;
                    }
    
                    if(flag == 0) {
                        sprintf(linha, "Record = %d", i);
                    } else {
                        sprintf(linha, "Registro nao encontrado!");
                    }
                    inserir(F, linha);
                }
    
            } else {
                sprintf(linha, "Campo nao encontrado!");
                inserir(F, linha);
            }
        }
    }
}

//11
void gotodado(DBF **dbf,Status **atual,char reg[]){

    Campo *campo = NULL;
    Dados *dado = NULL;
    Status *status = NULL;
    int i;

    //verifica se existe campo no .dbf
    if((*dbf) != NULL){

        campo = (*dbf)->campos;
        status = (*dbf)->status;
        //verifica se o campo é NULL e percorre todos
        while(campo != NULL && status != NULL){

            dado = campo->Pdados;
            status = (*dbf)->status;

            //percorre a qntd de vezes que o usuario digitou no campo dados
            for(i = atoi(reg); i - 1 > 0 && dado != NULL; i--){
                if(status->boolean){
                    dado = dado->prox;
                    status = status->prox;
                }
                else
                    i++;
                
            }
            if(dado != NULL ){
                campo->Patual = dado;
                *atual = status;
            }
            campo = campo->prox;
        }
    }

    baseRec(recordAtual((*dbf)->status,status), contaRecords(*dbf));
}

//12
void display(DBF *dbf, Status *pos, Fila *F){
    int i, size;
    Campo *campo = NULL;
    Dados *nivelDado = NULL;
    char linha[100];

    if (dbf != NULL) {
        campo = dbf->campos;

        if (campo != NULL) {
            nivelDado = campo->Patual;

            if (nivelDado != NULL) {
                //Guarda comando
                inserir(F, ". DISPLAY");
                //Guardar titulos
                sprintf(linha, "%s", "Record#");
                while(campo != NULL) {
                    size = (campo->Width == 1) ? 10 : campo->Width;
                    sprintf(linha, "%s %-*s", linha, size, campo->FieldName);
                    campo = campo->prox;
                }
                inserir(F, linha);
                
                campo = dbf->campos;
                i = recordAtual(dbf->status, pos);
                sprintf(linha, "%7d", i);
                
                while(campo != NULL && pos != NULL) {
                    nivelDado = campo->Patual;

                    if (nivelDado != NULL && pos->boolean) {
                        extraiDado(campo, nivelDado, linha);
                    }
                    
                    campo = campo->prox;
                    
                }
                inserir(F, linha);
            }
        }
    }
    
}

//13
void edit(DBF **dbf, Status **atual) {
    Campo *campo = NULL;
    Dados *dado = NULL;
    int y = 8;
    char linha[100];

    if (*dbf != NULL) {
        campo = (*dbf)->campos;

        if (campo != NULL) {

            //Printa os Campos que existem no arq
            exibeTitulosCampos(campo);
            while (campo != NULL) {
                dado = campo->Patual;

                sizeToSpace(20, y, campo->Width);
                textcolor(BLACK); textbackground(LIGHTGRAY);
                insere(campo, &dado, 20, y);
                fflush(stdin);

                campo = campo->prox;
                y++;
            }

        }
    }
}

void exibeTitulosCampos(Campo *campo) {
    int y = 8;

    while(campo->prox != NULL){
        gotoxy(7, y);
        printf("%s:\n",campo->FieldName);
        campo = campo->prox;
        y++;
    }

    gotoxy(7, y);
    printf("%s:\n", campo->FieldName);
}

//14
void deleteUni(DBF **dbf, Status **status){
    Campo *campos = (*dbf)->campos;
    if((*dbf) != NULL){

        if(*status != NULL && (*status)->boolean){

            (*status)->boolean = 0;
            // baseRec(1, contaRecords(*dbf));
            // Status atual recebe a cabeça da lista de status
            // (*status) = (*dbf)->status;
        }
        // Patual de todos os campos recebe a cabeça da lista de dados
        // while(campos != NULL){
        //     campos->Patual = campos->Pdados;
        //     campos = campos->prox;
        // }
    }
}

void deleteAll(DBF **dbf, Status **status){

    Status *auxSt = NULL;
    Campo *campos = NULL;
    
    if((*dbf)->status != NULL){
        auxSt = (*dbf)->status;
        campos = (*dbf)->campos;

        while(auxSt != NULL){

            if(auxSt->boolean){
                auxSt->boolean = 0;
            }
            auxSt = auxSt->prox;
        }
        // baseRec(1, contaRecords(*dbf));

        // Status atual recebe a cabeça da lista de status
        // (*status) = (*dbf)->status;

        // Patual de todos os campos recebe a cabeça da lista de dados
        // while(campos != NULL){
        //     campos->Patual = campos->Pdados;
        //     campos = campos->prox;
        // }
    }
}

//15
void reCall(Status **status, Fila *F){
    char linha[100];

    if(*status != NULL && !(*status)->boolean){
        (*status)->boolean = 1;
        sprintf(linha, "1 record recalled");
    }else{
        sprintf(linha, "no record recalled");
    }
    inserir(F, linha);
}
void reCallAll(DBF **dbf,  Fila *F){
    char linha[100];
    Status *status = (*dbf)->status;
    char flag = 0;

    if(*dbf != NULL){
        while(status != NULL){
            if(!status->boolean){
                status->boolean = 1;
                flag = 1;
            }
            status = status->prox;
        }
        
        if(flag){
            sprintf(linha, "all record recalled");
        }
        else{
            sprintf(linha, "no record recalled");
        }
    }
}

//18
void zap(DBF **dbf) {
    Campo *campo, *antCampo;
    Dados *dado, *antDado;
    Status *status, *antStatus;

    if ((*dbf) != NULL) {
        campo = (*dbf)->campos;
        status = (*dbf)->status;

        //Exclui dados
        if(campo != NULL) {
            while(campo != NULL) {
                dado = campo->Pdados;
                campo->Patual = NULL;
                campo->Pdados = NULL;

                while (dado != NULL) {
                    antDado = dado;
                    dado = dado->prox;
                    freeDado(campo, antDado);
                }

                campo = campo->prox;
            }
        }
        //Exclui status
        if (status != NULL) {
            while (status != NULL) {
                antStatus = status;
                status = status->prox;
                free(antStatus);
            }
            (*dbf)->status = NULL;
        }
    }
}

void freeDado(Campo *campo, Dados *ant) {
    switch (campo->Type) {
        case 'M':
            free(ant->tipo.valorM);
            ant->tipo.valorM = NULL;
        break;

        case 'C':
            free(ant->tipo.valorC);
            ant->tipo.valorC = NULL;
        break;
    }

    free(ant);    
    ant = NULL;
}

//19
void modifyStrucutre(DBF **dbf) {
    Campo *campo = NULL, *exibir;
    char op;
    int x = 7, y = 10, count = 1;
    char data[11], hr[9];

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(data, 11, "%d/%m/%Y", tm_info);
    strftime(hr, 11, "%H:%M:%S", tm_info);

    if (*dbf != NULL) {
        campo = (*dbf)->campos;
        if(campo != NULL) {

            createCampos();
            //Pergunta se quer alterar ou nao
            do {
                exibelinhacampo2(x, y, count, campo);
                dica1(0, "ALTERAR CAMPOS");
                dica2(0, "[1] Alterar Campo. [SPACE] Proximo campo.");
                baseField(count, contaCampos(*dbf) + 1);
                gotoxy(7, 19);
                op = getch();
                fflush(stdin);
                if (op == '1') {
                    exibelinhacampo(x, y, count);
                    alteraCampo(&campo, x, y);  
                    exibelinhacampo2(x, y, count, campo);
                }
                count++;
                y++;
                campo = campo->prox;    
            } while (campo != NULL);

            //Pergunta se quer adicionar ou nao
            //Criar campos
            dica1(0, "ADICIONAR CAMPOS");
            dica2(0, "[1] Criar Campo. [Esc] encerrar MODIFY STRUCTURE.");
            gotoxy(7, 19);
            op = getch();
            createCampos();
            fflush(stdin);
            //Iniciar loop campos
            textcolor(BLACK); textbackground(LIGHTGRAY);
            while (op != 27) {
                //Condição de impresão segunda parte
                if (y >= 18) {
                    createCampos2();
                    x = 52;
                    y = 10;
                }

                //lOGICA DE CADASTRAR CAMPOS
                baseField(count, contaCampos(*dbf) + 1);
                exibelinhacampo(x, y, count);
                exibir = cadastraCampo(&(*dbf), x, y);
                exibelinhacampo2(x, y, count, exibir);
                count++;
                y++;

                //Condição de parada if positivo
                if (x == 52 && y >= 18) {
                    dica1(0, "[SPACE]Encerrar CREATE.");
                    dica2(0, "Campos lotados!");
                    gotoxy(7, 19);
                    op = getch();
                    fflush(stdin);
                    op = 27;
                } else {
                    dica1(0, "ADICIONAR CAMPOS");
                    dica2(0, "[1] Criar Campo. [Esc] encerrar MODIFY STRUCTURE.");
                    gotoxy(7, 19);
                    op = getch();
                    fflush(stdin);
                }
            }
            textcolor(LIGHTGRAY); textbackground(BLACK);
        }
        //Atualiza o last update do Arq ref.
        strcpy((*dbf)->Hora,hr);
        strcpy((*dbf)->Data,data);

    }
}

void exibelinhacampo2(int x, int y, int count, Campo *campo) {
    textcolor(LIGHTGRAY); textbackground(BLACK);
    limparLinha(x, x + 34, y);
    gotoxy(x, y);
    //Exibir numero do campo
    if (count < 9) {
        printf("%d  ", count);
    } else {
        printf("%d ", count);
    }

    //Exibir conteudo
    printf("%s", campo->FieldName);
    gotoxy(x + 15, y); printf("%s", returnCH(campo->Type));
    gotoxy(x + 26, y); printf("%d", campo->Width);
    gotoxy(x + 31, y); printf("%d", campo->Dec);
    gotoxy(x + 3, y);
}

void alteraCampo(Campo **campo, int x, int y) {
    char ch, tipo;
    int autentico;
    Strdin *S;
    inicializarSdin(&S);

    if (*campo != NULL) {
        gotoxy(x + 3, y);

        do {
            ch = leChar();
            inserirSdin(&S, ch);
        } while (tamanhoSdin(S) < 10 && ch != 13);
        strcpy((*campo)->FieldName, converteStrdin(S));
        reiniciarSdin(&S);

        gotoxy(x + 15, y);
        do {
            tipo = toupper(getch());
        } while(verificach(tipo));
        (*campo)->Type = tipo;
        printCH(tipo);

        //Tipo logico é char, só 1 de tamanho
        if (tipo == 'L') {
            gotoxy(x + 26, y);
            printf("1");
            (*campo)->Width = 1;
        } 
        else {
            //Tipo memo é 50 de tamanho
            if (tipo == 'M') {
                gotoxy(x + 26, y);
                printf("50");
                (*campo)->Width = 50;
            }
            else {
                //Tipo date é 10 de tamanho
                if (tipo == 'D') {
                    gotoxy(x + 26, y);
                    printf("10");
                    (*campo)->Width = 10;
                } 
                else {
                    do {
                        reiniciarSdin(&S);
                        print2(x + 26, y, "   ");
                        gotoxy(x + 26, y);
                        do {
                            ch = leNum();
                            inserirSdin(&S, ch);
                        } while (tamanhoSdin(S) < 2 && ch != 13);
                    } while (atoi(converteStrdin(S)) > 50);
                    (*campo)->Width = atoi(converteStrdin(S));
                    reiniciarSdin(&S);
                }
            }
        }
        
        if (tipo == 'N') {
            //scanf("%d", &novo->Dec);
            do {
                reiniciarSdin(&S);
                print2(x + 31, y, "   ");
                gotoxy(x + 31, y);
                do {
                    ch = leNum();
                    inserirSdin(&S, ch);
                } while (tamanhoSdin(S) < 1);
            } while (atoi(converteStrdin(S)) > 7);
            (*campo)->Dec = atoi(converteStrdin(S));
            reiniciarSdin(&S);
        } else {
            gotoxy(x + 31, y);
            printf("0");
            (*campo)->Dec = 0;
        }
    }
}

//20
void sort(DBF **dbf, char *campoAlvo) {
    Campo *campo = NULL;
    Dados *dado = NULL;
    int pos = 0;

    if (*dbf != NULL) {
        campo = (*dbf)->campos;
        if(campo != NULL) {
            while (campo != NULL && !compare(campoAlvo, campo->FieldName)) {
                campo = campo->prox;
            }

            if (compare(campoAlvo, campo->FieldName)) {
                dado = campo->Pdados;
                if (dado != NULL) {
                    while (dado != NULL) {
                        sortDados(dbf, dado, campo, pos);
                        
                        dado = dado->prox;
                        pos++;
                    }
                }
            }
        }
    }
}

void sortDados(DBF **dbf, Dados *dado, Campo *campo, int pos) {
    Dados *auxDado = dado;
    int i = pos, troca = 0;
    float val, menorF;
    char floatchar[10], menorS[60], menorL;

    switch (campo->Type) {
        case 'N':
            sprintf(floatchar, "%*f", campo->Dec, auxDado->tipo.valorN);
            val = (float) atof(floatchar);
            menorF = val;
            while(auxDado != NULL) {
                sprintf(floatchar, "%*f", campo->Dec, auxDado->tipo.valorN);
                val = (float) atof(floatchar);

                if (val < menorF) {
                    menorF = val;
                    troca = i;
                }

                auxDado = auxDado->prox;
                i++;
            }
        break;

        case 'L':
            menorL = auxDado->tipo.valorL;
            while(auxDado != NULL) {
                if (auxDado->tipo.valorL < menorL) {
                    menorL = auxDado->tipo.valorL;
                    troca = i;
                }

                auxDado = auxDado->prox;
                i++;
            }
        break;

        case 'D':
            strcpy(menorS, auxDado->tipo.valorD);

            while(auxDado != NULL) {
                if (stricmp(menorS, auxDado->tipo.valorD) > 0) {
                    strcpy(menorS, auxDado->tipo.valorD);
                    troca = i;
                }

                auxDado = auxDado->prox;
                i++;
            }
        break;

        case 'C':
            strcpy(menorS, auxDado->tipo.valorC);
            while(auxDado != NULL) {
                if (stricmp(menorS, auxDado->tipo.valorC) > 0) {
                    strcpy(menorS, auxDado->tipo.valorC);
                    troca = i;
                }

                auxDado = auxDado->prox;
                i++;
            }
        break;

        case 'M':
            strcpy(menorS, auxDado->tipo.valorM);

            while(auxDado != NULL) {
                if (stricmp(menorS, auxDado->tipo.valorM) > 0) {
                    strcpy(menorS, auxDado->tipo.valorM);
                    troca = i;
                }

                auxDado = auxDado->prox;
                i++;
            }
        break;
    }

    if (troca > 0) {
        trocaDados(dbf, pos, troca);
    }
}

void trocaDados(DBF **dbf, int pos1, int pos2) {
    Campo *campo;
    Dados *dado, *dado1, *dado2;
    Tipo temp;
    char charAux[60];
    float floatAux;
    
    if (dbf != NULL) {
        campo = (*dbf)->campos;
        if(campo != NULL) {
            while (campo != NULL) {
                dado = campo->Pdados;

                if (dado != NULL) {
                    // print2(5, 8, "");
                    // printf("%d %d", pos1, pos2); system("pause");
                    for (int i = 0; i < pos1; i++) {
                        dado = dado->prox;
                    } 
                    dado1 = dado;
                    for (int i = 0; i < pos2; i++) {
                        dado = dado->prox;
                    } 
                    dado2 = dado;
                    
                    switch (campo->Type) {
                        case 'N':
                            temp.valorN = dado1->tipo.valorN;
                            dado1->tipo.valorN = dado2->tipo.valorN;
                            dado2->tipo.valorN = temp.valorN;
                        break;
            
                        case 'L':
                            temp.valorL = dado1->tipo.valorL;
                            dado1->tipo.valorL = dado2->tipo.valorL;
                            dado2->tipo.valorL = temp.valorL;
                        break;
                
                        case 'D':
                            strcpy(charAux, dado1->tipo.valorD);
                            strcpy(dado1->tipo.valorD, dado2->tipo.valorD);
                            strcpy(dado2->tipo.valorD, charAux);
                        break;
                
                        case 'C':
                            temp.valorC = dado1->tipo.valorC;
                            dado1->tipo.valorC = dado2->tipo.valorC;
                            dado2->tipo.valorC = temp.valorC;
                        break;
                
                        case 'M':
                            temp.valorM = dado1->tipo.valorM;
                            dado1->tipo.valorM = dado2->tipo.valorM;
                            dado2->tipo.valorM = temp.valorM;
                        break;
                    }
                }
                
                campo = campo->prox;
            }
        }
    }
}