#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <stdlib.h>

#include "Tad.h"

void print2(int x, int y, char str[]) {
    gotoxy(x, y);
    printf("%s", str);
}

char validaComando(char str[]) {
    if (strcmp(str, "SET") && strcmp(str, "CREATE") &&
        strcmp(str, "DIR") && strcmp(str, "QUIT") &&
        strcmp(str, "USE") && strcmp(str, "LIST") && strcmp(str, "CLEAR") &&
        strcmp(str, "LOCATE") && strcmp(str, "GOTO") &&
        strcmp(str, "DISPLAY") && strcmp(str, "EDIT") && 
        strcmp(str, "DELETE") && strcmp(str, "RECALL") &&
        strcmp(str, "PACK") && strcmp(str, "ZAP") &&
        strcmp(str, "MODIFY") && strcmp(str, "SORT")) {
        return 0;
    }

    return 1;
}

void base(void) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    for (int i = 5; i < 68; i++) {
        gotoxy(i, 13); printf("%c", 205);
	}
    
    gotoxy(5, 13);
    clreol();
    print2(5, 13, "Command Line");
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

//Divide a string str baseada no delimitador del e guarda na string str2
void strSplit(char str[], char *str2, char del) {
    int i;

    for (i = 0; str[i] != del && str[i] != '\0'; i++) {
        str2[i] = str[i];
    }
    str2[i] = '\0';
}

void lerComando(char *str) {
    char cmd[50];
    int i;

    do {
        base();
        print2(5, 12, ". ");

        strSplit(gets(str), cmd, ' ');

        gotoxy(5, 12);
        clreol();
    } while (!validaComando(cmd));
}

int main() {
    Unidade *unid = NULL;
    DBF *dbf = NULL;
    unsigned int op;
    
    //Extrair até o primeiro espaço
    // avaliar se é valido
    // baseado na primeira palavra fazer o resto
    char comando[50], cmd[50];
    lerComando(comando);
    strSplit(comando, cmd, ' ');
    gotoxy(1, 1);
    printf("String digitada: %s\n", comando);
    printf("Comando extraido: %s\n", cmd);

    system("pause");

    setDefaltTo(&unid, "D:"); 
    
    Create(&unid,&dbf,"Lucas.dbf","12/01/2025","13:00:00");
    Create(&unid,&dbf,"Matias.dbf","17/01/2035","11:00:00");

    Dir(&unid);

    ListStructure(unid, dbf);
    system("pause");
    
    // op = Menu1(op);
    // switch (op){
    //     case 1:
    //         //1
    //         break;
    //     case 2:
    //         //2
    //         break;
    //     case 0:
    //         quit();
    //         break;
    //     default:
    //         printf("Opcao invalida.\n");
    // }

    return 0;
}