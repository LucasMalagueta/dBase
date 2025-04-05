#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio2.h>

//Tratar comando
char validaComando(char str[]);
void lerComando(char str[]);
int chaveComando(char str[]);
void extrairParametro(char str[], char str2[]) ;
void extrairParametros(char mat[5][15], char comando[]);
void extraiAspas(char str[],char str2[]);

//Funções gerais
char compare(char str[], char str2[]);
void strSplit(char str[], char str2[], char del);
char validaCreate(char str[]);

//Interface Grafica
#include "GUI.h"
//Fila de impressão
#include "Fila.h"
//Capturar strings um caracter por vez dinamicamente
#include "Strdin.h"
//Funções principais e relacionados
#include "Tad.h"

int main() {
    Unidade *unid = NULL;
    DBF *aberto = NULL;
    Status *atual = NULL;
    char comando[50], cmd[15], arg[15], dado[60];
    char args[4][15];

    Fila F; 
    inicializar(&F);

    mainScreen();

    //Loop principal
    do {
        baseCmd("Command Line");
        dica1(0, "");
        dica2(0, "Enter a dBASE III PLUS command");
        lerComando(comando);
        strSplit(comando, cmd, ' ');
        // gotoxy(5, 8);
        // printf("String digitada: %s\n", comando);
        // printf("Comando extraido: %s\n", cmd);
        // printf("Chave do Comando: %d\n", chaveComando(cmd));

        switch (chaveComando(cmd)) {
            case 0:
                //Foi digitado comando que começa com "SET"
                extrairParametros(args, comando);
                //"SET DEFAULT ..."
                if (compare(args[1], "DEFAULT") && compare(args[2], "TO")) {
                    if (compare(args[3], "C:") || compare(args[3], "D:")) {
                        setDefaltTo(&unid, args[3]);
                        baseDir(unid->und);
                        aberto = unid->arqs;
                        baseDBF("");
                        if (aberto != NULL) {
                            strSplit(aberto->nomeDBF, cmd, '.');
                            baseDBF(cmd);
                        }
                        baseRec(0,0);
                    }
                }
            break;

            case 1:
                //Foi digitado comando que começa com "CREATE"
                extrairParametro(comando, arg);
                if (validaCreate(arg) && unid != NULL) {
                    baseDBF(arg);
                    baseCmd("CREATE");
                    clear(&F);
                    Create(&unid, &aberto, arg);
                    clear(&F);
                }
            break;

            case 2:
                //Foi digitado o comando "DIR"
                if (unid != NULL) {
                    Dir(unid, &F);
                    exibir(&F);
                }
            break;

            case 3:
                //Foi digitado o comando "QUIT" irá sair do loop do while
                quit();
            break;

            case 4:
                //Foi digitado o comando "USE"
                extrairParametro(comando, arg);
                sprintf(arg, "%s.DBF", arg);
                
                if (buscaDBF(arg, aberto) != NULL) {
                    USE(&aberto, buscaDBF(arg, aberto));
                    strSplit(arg, cmd, '.');
                    baseDBF(cmd);
                    baseRec(0,contaRecords(aberto));
                }

            break;

            case 5:
                //Foi digitado comando que começa com "LIST"
                if (compare(comando, "LIST")) {
                    list(aberto, &F);
                    exibir(&F);
                }
                //Comando tem paramestros alem de LIST
                else {
                    extrairParametros(args, comando);
                    //LIST STRUCTURE
                    if (compare(args[1], "STRUCTURE")) {
                        ListStructure(unid, aberto, &F);
                        exibir(&F);
                    }

                    //LIST FOR ... = " "
                    else {                        
                        if (compare(args[1], "FOR")) {
                            extrairParametros(args, comando);
                            extraiAspas(comando, arg);
                            listFor(aberto, args[2], arg, &F);
                            exibir(&F);
                        }
                    }
                }
            break;
            
            case 6:
                //Foi digitado o comando "APPEND"
                clear(&F);
                Append(&aberto,&atual);
                clear(&F);
            break;

            case 7:
                //Foi digitado o comando "CLEAR"
                clear(&F);
            break;

            case 8:
                extrairParametros(args, comando);
                //"LOCATE FOR..."
                //LOCATE FOR MEMO ACIMA DE 35 CARACTERES NAO FUNCIONA, 
                // RESOLVER DEPOIS
                //aZqWeRtYuIoPlKjHgFdScVbNmXxCcVvBbN
                //aZqWeRtYuIoPlKjHgFdScVbNmXxCcVvBbNnMmLlKkJjHhGgFf
                if (compare(args[1], "FOR")) {
                    extraiAspas(comando, dado);
                    locate(&aberto, args[2], dado, &F);
                    exibir(&F);
                }
                
            break;

            case 9:
                //Foi digitado o comando "GOTO"
                extrairParametros(args, comando);
                gotodado(&aberto,&atual, args[1]);
            break;

            case 10:
                //Foi digitado o comando "DISPLAY"
                display(aberto,atual,&F);
                exibir(&F);
            break;

            case 12:
                //Foi digitado o comando "DELETE"
                extrairParametros(args, comando);
                if(compare(args[1],"ALL")){
                    deleteAll(&aberto,&atual);

                }else
                    deleteUni(&aberto,&atual);
            break;

            case 13:
                //Foi digitado o comando "RECALL"
            break;

            case 16:
                //Foi digitado comando que começa com "MODIFY"
                extrairParametro(comando, arg);
                if (compare(arg, "STRUCTURE")) {
                    clear(&F);
                    baseCmd("MODIFY STRUCTURE");
                    modifyStrucutre(&aberto);
                    clear(&F);
                }
            break;

            default:
                //print2(7, 19, "Opcao invalida.\n");
            break;
        }
        
    } while (chaveComando(cmd) != 3);

    return 0;
}

char validaCreate(char str[]) {
    char flag = 1;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            flag = 0;
        }
    }

    return flag;
}

char validaComando(char str[]) {
    char flag = 0;
    char mat[18][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "APPEND",
                        "CLEAR","LOCATE" ,"GOTO" , "DISPLAY", "EDIT", "DELETE",
                        "RECALL", "PACK", "ZAP", "MODIFY", "SORT"};

    for (int i = 0; i < 18; i++) {
        if (strcmp(str, mat[i]) == 0) {
            flag = 1;
        }
    }

    return flag;
}

//Divide a string str baseada no delimitador del e guarda na string str2
void strSplit(char str[], char str2[], char del) {
    int i;

    for (i = 0; str[i] != del && str[i] != '\0'; i++) {
        str2[i] = str[i];
    }
    str2[i] = '\0';
}

void lerComando(char str[]) {
    char cmd[50];
    int i;

    do {
        print2(5, 19, ". ");

        strSplit(gets(str), cmd, ' ');

        gotoxy(5, 19);
        //Linha de comando
        limparLinha(7, 87, 19);
    } while (!validaComando(cmd));
}

int chaveComando(char str[]) {
    char flag = 0;
    char mat[18][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "APPEND",
                        "CLEAR","LOCATE" ,"GOTO" , "DISPLAY", "EDIT", "DELETE",
                        "RECALL", "PACK", "ZAP", "MODIFY", "SORT"};

    for (int i = 0; i < 18; i++) {
        if (strcmp(str, mat[i]) == 0) {
            flag = i;
        }
    } 

    return flag;
}

//Retorna tudo depois do primeiro espaço
void extrairParametro(char str[], char str2[]) {
    int i = 0, j;
    while (str[i] != ' ') {
        i++;
    }
    i++;

    for (j = 0; str[i] != '\0'; i++, j++) {
        str2[j] = str[i];
    }
    
    str2[j] = '\0';
}

void extrairParametros(char mat[5][15], char comando[]) {
    int i, j, k;
    for (i = 0, j = 0, k = 0; comando[i] != '\0'; i++) {
        if (comando[i] == ' ') {
            mat[j++][k] = '\0';
            k = 0;
        } else {
            mat[j][k++] = comando[i];
        }
    }
    mat[j][k] = '\0';
}

char compare(char str[], char str2[]) {
    return (strcmp(str, str2) == 0);
}

void extraiAspas(char str[],char str2[]){
    int i = 0, j;

    while (str[i] != '"') {
        i++;
    }
    i++;

    for (j = 0; str[i] != '\0' && str[i] != '"'; i++, j++) {
        str2[j] = str[i];
    }
    str2[j] = '\0';
}