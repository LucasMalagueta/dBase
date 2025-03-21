#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <stdlib.h>

#include "Tad.h"

int main() {
    Unidade *unid = NULL;
    DBF *dbf;
    unsigned int op;

    setDefaltTo(&unid, "C:");
    printf("%s\n", unid->und); system("pause");
    
    criarDBF(&dbf,"Lucas",12012025,13);
    op = Menu1(op);
    switch (op){
        case 1:
            //1
            break;
        case 2:
            //2
            break;
        case 0:
            quit();
            break;
        default:
            printf("Opcao invalida.\n");
    }

    return 0;
}