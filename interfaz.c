#include "interfaz.h"

void limpiarPantalla() {
    system("cls");
}

// Colores de texto
void colorNormal()     { color(15); }
void colorTitulo()     { color(11); }
void colorMenu()       { color(10); }
void colorError()      { color(12); }
void colorDato()       { color(14); }
void colorLinea()      { color(8); }

// Sonidos
void beepOk()          { Beep(1200, 120); }
void beepError()       { Beep(400, 400); }
void beepMenu()        { Beep(700, 60); }
void beepConfirm()     { Beep(900, 80); Beep(1200, 80); }

// Imprime un texto centrado horizontalmente en la fila 'y' de la consola, Calcula el ancho de la consola y posiciona el texto justo en el medio
void printCenter(int y, const char* texto) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int x = (columns - (int)strlen(texto)) / 2;
    gotoxy(x, y);
    printf("%s", texto);
}

// Línea horizontal en una fila
void printLinea(int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    colorLinea();
    gotoxy(0, y);
    for (int i = 0; i < columns; i++) {
        printf("%c", 205);
    }
    colorNormal();
}

// Marco completo alrededor de la consola
void printMarco() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows    = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    colorLinea();
    for (int x = 0; x < columns; x++) {
        gotoxy(x, 0);            printf("%c", 205);
        gotoxy(x, rows - 1);     printf("%c", 205);
    }

    for (int y = 0; y < rows; y++) {
        gotoxy(0, y);            printf("%c", 186);
        gotoxy(columns - 1, y);  printf("%c", 186);
    }

    gotoxy(0, 0);                printf("%c", 201);
    gotoxy(columns - 1, 0);      printf("%c", 187);
    gotoxy(0, rows - 1);         printf("%c", 200);
    gotoxy(columns - 1, rows - 1); printf("%c", 188);
    colorNormal();
}

// Mensaje de pausa esperando tecla
void esperaTecla() {
    colorDato();
    printCenter(23, "\nPresione una tecla para continuar...");
    colorNormal();
    getch();
}
