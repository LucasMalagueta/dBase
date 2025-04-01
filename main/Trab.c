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

//Interface Grafica
#include "GUI.h"
//Fila de impressão
#include "Fila.h"
//Funções principais e relacionados
#include "Tad.h"

int main() {
    Unidade *unid = NULL;
    DBF *dbf = NULL, *aberto = NULL;

    char comando[50], cmd[15], arg[15], dado[25];
    char args[4][15];

    Fila F; 
    inicializar(&F);

    clrscr();
    textcolor(CYAN);
    moldura(3, 1, 88, 23);
    textcolor(WHITE);
    instrucoes();
    base();
    baseDir(" ");
    baseField(" ");

    do {
        baseCmd("Command Line");
        lerComando(comando);
        strSplit(comando, cmd, ' ');
        // gotoxy(1, 1);
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
                        baseDBF("");
                        dbf = unid->arqs;
                        aberto = dbf;
                    }
                }
            break;

            case 1:
                //Foi digitado comando que começa com "CREATE"
                extrairParametro(comando, arg);
                strSplit(arg, cmd, '.');
                baseDBF(cmd);
                baseCmd("CREATE");
                Create(&unid, &aberto, arg);
                exibir(&F);
            break;

            case 2:
                //Foi digitado o comando "DIR"
                Dir(unid, &F);
                exibir(&F);
            break;

            case 3:
                //Foi digitado o comando "QUIT" irá sair do loop do while
                quit();
            break;

            case 4:
                //Foi digitado o comando "USE"
                extrairParametro(comando, arg);

                if (buscaDBF(arg, dbf) != NULL) {
                    USE(&aberto, buscaDBF(arg, dbf));
                    baseDBF(arg);
                }
            break;

            case 5:
                //Foi digitado comando que começa com "LIST"
                if (compare(comando, "LIST")) {
                    list(&aberto, &F);
                    
                } else {
                    extrairParametros(args, comando);
                    if (compare(args[1], "STRUCTURE")) {
                        ListStructure(unid, dbf);
                    }
                }
            break;
            
            case 6:
                //Foi digitado o comando "CLEAR"
                clear(&F);
            break;

            case 7:
                //Foi digitado o comando "APPEND"
                Append(&aberto);
                clear(&F);
            break;

            case 9:
                extrairParametros(args, comando);
                //"LOCATE FOR..."
                if (compare(args[1], "FOR")) {
                    extraiAspas(comando,dado);
                    locate(&aberto, args[2],dado);
                }
            break;

            case 10:
                //Foi digitado o comando "GOTO"
                extrairParametros(args, comando);
                gotodado(&aberto, args[1]);
            break;

            default:
                //print2(7, 19, "Opcao invalida.\n");
            break;
        }
        
    } while (chaveComando(cmd) != 3);

    return 0;
}



char validaComando(char str[]) {
    char flag = 0;
    char mat[18][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "CLEAR",
                        "APPEND", "DISPLAY", "LOCATE", "GOTO", "EDIT", "DELETE",
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
    char mat[18][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "CLEAR",
                        "APPEND", "DISPLAY", "LOCATE", "GOTO", "EDIT", "DELETE", "RECALL", "PACK", "ZAP", 
                        "MODIFY", "SORT"};

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