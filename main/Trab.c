#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <stdlib.h>

#include "Tad.h"

int main() {
    Unidade *unid = NULL;
    DBF *dbf;
    unsigned int op;

    setDefaltTo(&unid, "D:");
    printf("%s\n", unid->und); 
    
    criarDBF(&unid,&dbf,"Lucas","12/01/2025","13:00:00");

    printf("%s\n", unid->arqs->nomeDBF);

    criarDBF(&unid,&dbf,"Matias","17/01/2035","11:00:00");

    printf("%s\n", unid->arqs->prox->nomeDBF);
    system("pause");
    

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