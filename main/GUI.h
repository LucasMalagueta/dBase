//Interface Gráfica
void moldura(int xi, int yi, int xf, int yf);
void limparLinha(int xi, int xf, int y);
void limparArea(int xi, int yi, int xf, int yf);
void linhaVertical(int xi, int yi, int yf);
void instrucoes();
void base();
void baseDir(char*);
void baseCmd(char*);
void baseDBF(char*);
void baseRec(int,int);
void baseField(int,int);
void baseNum();
void createCampos();
void createCampos2();
void mainScreen();
void sizeToSpace(int x, int y, int z);
void dica1(int x, char str[]);
void dica2(int x, char str[]);
void print2(int x, int y, char str[]);

void sizeToSpace(int x, int y, int z) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(x, x + z, y);
    gotoxy(x, y);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void mainScreen() {
    clrscr();
    textcolor(CYAN);
    moldura(3, 1, 88, 23);
    textcolor(WHITE);
    instrucoes();
    base();
    baseDir(" ");
    baseRec(0, 0);
    baseNum();
    dica1(0, "Enter a dBASE III PLUS command");
}

void print2(int x, int y, char str[]) {
    gotoxy(x, y);
    printf("%s", str);
}

void base() {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Barra completa
    limparLinha(5, 87, 20);
    //Separadores
    gotoxy(21, 20); printf("%c", 186);
    gotoxy(26, 20); printf("%c", 186);
    gotoxy(51, 20); printf("%c", 186);
    gotoxy(71, 20); printf("%c", 186);
    gotoxy(78, 20); printf("%c", 186);

    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseCmd(char cmd[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(5, 21, 20);
    //Linha de comando
    print2(5, 20, cmd);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseDir(char dir[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Diretório atual
    if (dir != " ") {
        gotoxy(22, 20); printf("<%s>", dir);
    }

    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseDBF(char DBF[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(27, 42, 20);
    //Linha dbf
    print2(27, 20, DBF);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseRec(int qtd,int total) {
    char str[10];
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(52, 67, 20);
    if (qtd == 0 && total == 0) {
        print2(52, 20, "Rec: None");
    } else {
        sprintf(str, "Rec: %d/%d", qtd, total);
        print2(52, 20, str);
    }
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseField(int qtd, int total) {
    char str[10];
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(52, 67, 20);
    sprintf(str, "Field: %d/%d", qtd, total);
    print2(52, 20, str);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseNum() {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    limparLinha(79, 86, 20);
    print2(79, 20, "Num");
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void moldura(int xi, int yi, int xf, int yf) {
    gotoxy(xi, yi); printf("%c", 201); // Canto superior esquerdo
    gotoxy(xf, yi); printf("%c", 187); // Canto superior direito
    gotoxy(xi, yf); printf("%c", 200); // Canto inferior esquerdo
    gotoxy(xf, yf); printf("%c", 188); // Canto inferior direito

    for (int a = xi + 1; a < xf; a++) {
        gotoxy(a, yi); printf("%c", 205); // Linha superior
        gotoxy(a, yf); printf("%c", 205); // Linha inferior
    }

    for (int b = yi + 1; b < yf; b++) {
        gotoxy(xi, b); printf("%c", 186); // Linha lateral esquerda
        gotoxy(xf, b); printf("%c", 186); // Linha lateral direita
    }
}

void linhaVertical(int xi, int yi, int yf) {
    gotoxy(xi, yi++); printf("%c", 203);
    gotoxy(xi, yf); printf("%c", 202);

    while(yi < yf) {
        gotoxy(xi, yi++); printf("%c", 186);
    }
}

void instrucoes() {
    //Moldura -> 71 de largura e 5 de altura
    moldura(5, 2, 76, 7);
    //Caixa 1
    print2(7, 3, "CURSOR  <-- -->");
    print2(8, 4, "Char:    ");
    printf("%c ", 27);
    printf("%c", 26);
    print2(8, 5, "Word: Home End");
    print2(8, 6, "Pan:    ^");
    printf("%c ^", 27);
    printf("%c", 26);
    linhaVertical(23, 2, 7);

    //Caixa 2
    print2(28, 3, "INSERT");
    print2(26, 4, "Char:  Ins");
    print2(26, 5, "Field: ^N");
    print2(26, 6, "Help:  F1");
    linhaVertical(38, 2, 7);

    //Caixa3
    print2(44, 3, "DELETE");
    print2(41, 4, "Char:   Del");
    print2(41, 5, "Word:  ^Y");
    print2(41, 6, "Field: ^U");
    linhaVertical(53, 2, 7);

    //Caixa4
    print2(56, 3, "Up a field:    ");
    printf("%c", 24);
    print2(56, 4, "Down a field:  ");
    printf("%c", 25);
    print2(56, 5, "Exit/Save:    ^End");
    print2(56, 6, "Abort:         Esc");
}

void createCampos() {
    print2(10, 8, "Field Name  Type     Width  Dec");
    gotoxy(10, 9);
    for (int i = 0; i < 31; i++) {
        printf("%c", 205);
    }
}

void createCampos2() {
    print2(55, 8, "Field Name  Type     Width  Dec");
    gotoxy(55, 9);
    for (int i = 0; i < 31; i++) {
        printf("%c", 205);
    }
    print2(52, 10, "1");
}

void limparLinha(int xi, int xf, int y) {
    for (; xi < xf; xi++) {
        gotoxy(xi, y); printf("%c", ' ');
	}
}

void limparArea(int xi, int yi, int xf, int yf) {
    for (; yi < yf; yi++) {
        gotoxy(xi, yi);
        for (int i = xi; i < xf; i++) {
            printf("%c", ' ');
        }
    }
}

void dica1(int x, char str[]) {
    textbackground(BLACK); textcolor(LIGHTGRAY);
    limparLinha(5, 87, 21);
    if (x > 0) {
        print2(x, 21, str);
    }
    if (x == 0) {
        x = 5 + ((82 - strlen(str)) / 2);
        print2(x, 21, str);
    }
}

void dica2(int x, char str[]) {
    textbackground(BLACK); textcolor(LIGHTGRAY);
    limparLinha(5, 87, 22);
    if (x > 0) {
        print2(x, 22, str);
    }

    if (x == 0) {
        x = 5 + ((82 - strlen(str)) / 2);
        print2(x, 22, str);
    }
}