#include "clientes.h"

// =================== VISUALES Y SONIDO ===================
void limpiarPantalla() { system("cls"); }
void gotoxy(int x, int y) {
    COORD coord; coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void colorNormal()      { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }
void colorTitulo()      { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); }
void colorMenu()        { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); }
void colorError()       { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
void colorDato()        { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); }
void colorLinea()       { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); }
void beepOk()      { Beep(1200, 120); }
void beepError()   { Beep(400, 400); }
void beepMenu()    { Beep(700, 60); }
void beepConfirm() { Beep(900, 80); Beep(1200, 80); }

void printCenter(int y, const char* texto) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int x = (columns - (int)strlen(texto)) / 2;
    gotoxy(x, y); printf("%s", texto);
}
void printLinea(int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    colorLinea(); gotoxy(0, y);
    for (int i=0; i<columns; i++) printf("%c", 205);
    colorNormal();
}
void printMarco() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows    = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    colorLinea();
    for(int x=0; x<columns; x++) {
        gotoxy(x,0); printf("%c", 205);
        gotoxy(x,rows-1); printf("%c", 205);
    }
    for(int y=0; y<rows; y++) {
        gotoxy(0,y); printf("%c", 186);
        gotoxy(columns-1,y); printf("%c", 186);
    }
    gotoxy(0,0); printf("%c",201);
    gotoxy(columns-1,0); printf("%c",187);
    gotoxy(0,rows-1); printf("%c",200);
    gotoxy(columns-1,rows-1); printf("%c",188);
    colorNormal();
}
void esperaTecla() {
    colorDato(); printCenter(23, "\nPresione una tecla para continuar..."); colorNormal(); getch();
}

// =================== UTILITARIAS ===================
void limpiarBuffer() { int c; while((c = getchar()) != '\n' && c != EOF); }
int anioActual()  { time_t t = time(NULL); struct tm tm = *localtime(&t); return tm.tm_year + 1900; }
int mesActual()   { time_t t = time(NULL); struct tm tm = *localtime(&t); return tm.tm_mon + 1; }
int diaActual()   { time_t t = time(NULL); struct tm tm = *localtime(&t); return tm.tm_mday; }
int esNumerico(const char* str) { for (int i = 0; str[i]; i++) if (str[i] < '0' || str[i] > '9') return 0; return 1; }
int proximoId(const char* archivo, size_t tam) {
    FILE* f = fopen(archivo, "rb");
    if (!f) return 1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return (size / tam) + 1;
}

// =================== MENUS VISUALES ===================
void menuClientes() {
    limpiarPantalla();
    printMarco();
    colorTitulo();
    printCenter(3, "==== Gestion de Clientes ===="); colorNormal();
    colorMenu();
    printCenter(6, "1. Alta");
    printCenter(7, "2. Baja");
    printCenter(8, "3. Modificacion");
    printCenter(9, "4. Consulta");
    printCenter(10,"5. Listado");
    printCenter(11,"0. Volver");
    colorNormal();
    printLinea(12);
    printCenter(14, "Seleccione una opcion: ");
    beepMenu();
}
void menuCuentas() {
    limpiarPantalla();
    printMarco();
    colorTitulo();
    printCenter(3, "==== Gestion de Cuentas ===="); colorNormal();
    colorMenu();
    printCenter(6, "1. Alta");
    printCenter(7, "2. Baja");
    printCenter(8, "3. Modificacion");
    printCenter(9, "4. Consulta");
    printCenter(10,"5. Listado por cliente");
    printCenter(11,"0. Volver");
    colorNormal();
    printLinea(12);
    printCenter(14, "Seleccione una opcion: ");
    beepMenu();
}
void menuMovimientos() {
    limpiarPantalla();
    printMarco();
    colorTitulo();
    printCenter(3, "==== Gestion de Movimientos ===="); colorNormal();
    colorMenu();
    printCenter(6, "1. Alta");
    printCenter(7, "2. Baja");
    printCenter(8, "3. Modificacion");
    printCenter(9, "4. Consulta");
    printCenter(10,"5. Listado por cuenta");
    printCenter(11,"6. Listado por mes");
    printCenter(12,"0. Volver");
    colorNormal();
    printLinea(13);
    printCenter(15, "Seleccione una opcion: ");
    beepMenu();
}

// =================== (ABMCL Y OTROS: USA PATRON VISUAL EN CADA FUNCIÓN) ===================

void altaCliente() {
    stCliente c;

    // Abre el archivo de clientes en modo lectura/escritura y agrega si no existe
    FILE* f = fopen(ARCH_CLIENTES, "ab+");
    if (!f) {
        colorError();                      // Color rojo para errores
        printCenter(7, "Error al abrir archivo de clientes.");
        colorNormal();
        beepError();                       // Sonido de error
        esperaTecla();                     // Espera una tecla antes de continuar
        return;
    }

    // Limpia la pantalla y dibuja marco (interfaz visual simple)
    limpiarPantalla();
    printMarco();                          // Marco con bordes en consola
    colorTitulo();
    printCenter(3, "ALTA DE CLIENTE");     // Título centrado
    printf("\n\n");
    colorNormal();
    beepMenu();                            // Sonido corto de entrada

    // Genera ID y número de cliente aleatorio
    c.id = proximoId(ARCH_CLIENTES, sizeof(stCliente));
    c.nroCliente = c.id * 100 + rand() % 100;

    // Solicita y carga datos del cliente (nombre, apellido, DNI, etc.)
    leerDatosCliente(&c, f);

    // Marca como activo (no eliminado)
    c.eliminado = 0;

    // Guarda en archivo binario
    fwrite(&c, sizeof(stCliente), 1, f);
    fclose(f);

    // Muestra mensaje de éxito con efecto visual básico
    colorMenu();                           // Color verde para éxito
    printCenter(20, "Cliente dado de alta correctamente.");
    colorNormal();
    beepConfirm();                         // Sonido de confirmación
    esperaTecla();
}

int dniExiste(FILE* f, const char* dni) {
    rewind(f);  // Volver al inicio del archivo
    stCliente aux;
    while (fread(&aux, sizeof(stCliente), 1, f)) {
        if (aux.eliminado == 0 && strcmp(aux.dni, dni) == 0) {
            return 1; // Existe
        }
    }
    return 0; // No existe
}

int emailValido(const char* email) {
    return (strchr(email, '@') != NULL);
}

int camposObligatoriosCompletos(stCliente c) {
    if (strlen(c.nombre) == 0) {
        printf("El nombre no puede estar vacío.\n");
        return 0;
    }
    if (strlen(c.apellido) == 0) {
        printf("El apellido no puede estar vacío.\n");
        return 0;
    }
    if (strlen(c.email) == 0) {
        printf("El email no puede estar vacío.\n");
        return 0;
    }
    return 1; // Todos los campos obligatorios están completos
}

void leerCampoObligatorio(char* destino, int maxLen, const char* etiqueta) {
    //destino = dónde se va a guardar lo que el usuario escribe (el campo real del cliente)
    //maxLen = cantidad máxima de caracteres permitidos (por seguridad y tamaño de campo)
    //etiqueta = el nombre del campo que se va a mostrar al usuario (ej: "Nombre", "Email")
    do {
        printf("%s: ", etiqueta);
        fgets(destino, maxLen, stdin);
        destino[strcspn(destino, "\n")] = 0;

        if (strlen(destino) == 0) {
            printf("El campo \"%s\" no puede estar vacío.\n", etiqueta);
        }
        fflush(stdin); //limpia buffer de entrada
    } while (strlen(destino) == 0);
}


void leerDatosCliente(stCliente* c, FILE* f) {
    leerCampoObligatorio(c->nombre, 30, "Nombre");
    leerCampoObligatorio(c->apellido, 30, "Apellido");

    // === Email con validación adicional de @ ===
    do {
        leerCampoObligatorio(c->email, 30, "Email");
        if (!emailValido(c->email)) {
            printf("El email debe contener un '@'. Intente nuevamente.\n");
            c->email[0] = '\0';
        }
    } while (strlen(c->email) == 0);

    // === DNI con validaciones especiales ===
    do {
        leerCampoObligatorio(c->dni, 10, "DNI");

        if (!esNumerico(c->dni)) {
            printf("El DNI debe ser numérico.\n");
        } else if (dniExiste(f, c->dni)) {
            printf("Ya existe un cliente con ese DNI.\n");
        } else if (strlen(c->dni) < 7 || strlen(c->dni) > 8) {
            printf("El DNI debe tener entre 7 y 8 dígitos.\n");
        } else {
            break;
        }
    } while (1);

    leerCampoObligatorio(c->domicilio.calle,     30, "Calle");
    leerCampoObligatorio(c->domicilio.nro,        6, "Nro");
    leerCampoObligatorio(c->domicilio.localidad, 50, "Localidad");
    leerCampoObligatorio(c->domicilio.provincia, 40, "Provincia");
    leerCampoObligatorio(c->domicilio.cpos,       6, "Código Postal");
    leerCampoObligatorio(c->telefonoFijo,        12, "Teléfono Fijo");
    leerCampoObligatorio(c->telefonoMovil,       12, "Teléfono Móvil");
}




// ========================== CLIENTES ========================== //


void bajaCliente() {
    char dni[10];
    printf("\n--- Baja de Cliente ---\n");
    printf("Ingrese DNI: "); fflush(stdin); fgets(dni, 10, stdin); dni[strcspn(dni, "\n")] = 0;

    FILE* f = fopen(ARCH_CLIENTES, "rb+");
    if(!f) {
        printf("Error al abrir archivo de clientes.\n");
        esperaTecla(); return;
    }
    stCliente c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCliente), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.dni, dni) == 0) {
            fseek(f, -sizeof(stCliente), SEEK_CUR);
            c.eliminado = 1;
            fwrite(&c, sizeof(stCliente), 1, f);
            printf("Cliente dado de baja.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el cliente.\n");
    fclose(f);
    esperaTecla();
}

void modificarCliente() {
    char dni[10];
    printf("\n--- Modificar Cliente ---\n");
    printf("Ingrese DNI: "); fflush(stdin); fgets(dni, 10, stdin); dni[strcspn(dni, "\n")] = 0;

    FILE* f = fopen(ARCH_CLIENTES, "rb+");
    if(!f) {
        printf("Error al abrir archivo de clientes.\n");
        esperaTecla(); return;
    }
    stCliente c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCliente), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.dni, dni) == 0) {
            printf("Modificar nombre (actual: %s): ", c.nombre); fflush(stdin); fgets(c.nombre, 30, stdin); c.nombre[strcspn(c.nombre, "\n")] = 0;
            printf("Modificar apellido (actual: %s): ", c.apellido); fflush(stdin); fgets(c.apellido, 30, stdin); c.apellido[strcspn(c.apellido, "\n")] = 0;
            printf("Modificar email (actual: %s): ", c.email); fflush(stdin); fgets(c.email, 30, stdin); c.email[strcspn(c.email, "\n")] = 0;
            fseek(f, -sizeof(stCliente), SEEK_CUR);
            fwrite(&c, sizeof(stCliente), 1, f);
            printf("Datos modificados.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el cliente.\n");
    fclose(f);
    esperaTecla();
}

void consultarCliente() {
    char dni[10];
    printf("\n--- Consultar Cliente ---\n");
    printf("Ingrese DNI: "); fflush(stdin); fgets(dni, 10, stdin); dni[strcspn(dni, "\n")] = 0;

    FILE* f = fopen(ARCH_CLIENTES, "rb");
    if(!f) {
        printf("Error al abrir archivo de clientes.\n");
        esperaTecla(); return;
    }
    stCliente c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCliente), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.dni, dni) == 0) {
            printf("ID: %d\nNroCliente: %d\nNombre: %s %s\nDNI: %s\nEmail: %s\nProvincia: %s\n",
                c.id, c.nroCliente, c.nombre, c.apellido, c.dni, c.email, c.domicilio.provincia);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el cliente.\n");
    fclose(f);
    esperaTecla();
}


void listarClientes() {
    printf("\n--- Listado de Clientes ---\n");

    FILE* f = fopen(ARCH_CLIENTES, "rb");
    if (!f) {
        printf("Error al abrir archivo de clientes. No hay datos\n");
        esperaTecla();
        return;
    }

    stCliente c;
    int hay = 0;    // Flag para saber si se encontró al menos un cliente activo

    while (fread(&c, sizeof(stCliente), 1, f)) {
        if (c.eliminado == 0) {       // Si el cliente no está eliminado (es decir, está activo)
            printf("ID:%d | Nro:%d | %s %s | DNI:%s | Email:%s\n",
                   c.id, c.nroCliente, c.nombre, c.apellido, c.dni, c.email);
            hay = 1;
        }
    }

    if (!hay) {
        printf("No hay clientes activos.\n");
    } else {
        printf("\n");  // Deja espacio antes de "Presione una tecla..."
    }

    fclose(f);
    esperaTecla();
}



void buscarClientePorApellidoODni() {
    char busq[MAX_STR];
    printf("\n--- Buscar Cliente por Apellido o DNI ---\n");
    printf("Ingrese apellido o DNI: "); fflush(stdin); fgets(busq, MAX_STR, stdin); busq[strcspn(busq, "\n")] = 0;

    FILE* f = fopen(ARCH_CLIENTES, "rb");
    if(!f) {
        printf("Error al abrir archivo de clientes.\n");
        esperaTecla(); return;
    }
    stCliente c;
    int hay = 0;
    while(fread(&c, sizeof(stCliente), 1, f)) {
        if(c.eliminado == 0 && (strcmp(c.apellido, busq) == 0 || strcmp(c.dni, busq) == 0)) {
            printf("ID:%d | Nro:%d | %s %s | DNI:%s | Email:%s\n", c.id, c.nroCliente, c.nombre, c.apellido, c.dni, c.email);
            hay = 1;
        }
    }
    if(!hay) printf("No se encontraron coincidencias.\n");
    fclose(f);
    esperaTecla();
}

// ========================== CUENTAS ==========================
void altaCuenta() {
    stCuenta cta;
    FILE* f = fopen(ARCH_CLIENTES, "rb");
    if(!f) { printf("Debe haber clientes antes de crear cuentas!\n"); esperaTecla(); return; }
    fclose(f);

    cta.id = proximoId(ARCH_CUENTAS, sizeof(stCuenta));
    printf("\n--- Alta de Cuenta ---\n");
    printf("ID de Cliente: "); scanf("%d", &cta.idCliente); limpiarBuffer();

    FILE* fcli = fopen(ARCH_CLIENTES, "rb");
    stCliente cli; int existe = 0;
    while(fread(&cli, sizeof(stCliente), 1, fcli)) {
        if(cli.id == cta.idCliente && cli.eliminado == 0) { existe = 1; break; }
    }
    fclose(fcli);
    if(!existe) { printf("Cliente no encontrado.\n"); esperaTecla(); return; }

    printf("Nro de Cuenta: "); fgets(cta.nroCuenta, 24, stdin); cta.nroCuenta[strcspn(cta.nroCuenta, "\n")] = 0;
    printf("CBU: "); fgets(cta.cbu, 24, stdin); cta.cbu[strcspn(cta.cbu, "\n")] = 0;
    printf("Tipo de cuenta (1. CA$ 2. CAU$S 3. Cta Cte$): "); scanf("%d", &cta.tipoDeCuenta); limpiarBuffer();
    printf("Costo mensual: "); scanf("%f", &cta.costoMensual); limpiarBuffer();
    printf("Saldo inicial: "); scanf("%f", &cta.saldo); limpiarBuffer();
    cta.eliminado = 0;

    FILE* fctas = fopen(ARCH_CUENTAS, "ab");
    fwrite(&cta, sizeof(stCuenta), 1, fctas);
    fclose(fctas);
    printf("Cuenta dada de alta.\n");
    esperaTecla();
}

void bajaCuenta() {
    char nroCuenta[24];
    printf("\n--- Baja de Cuenta ---\n");
    printf("Ingrese Nro de Cuenta: "); fflush(stdin); fgets(nroCuenta, 24, stdin); nroCuenta[strcspn(nroCuenta, "\n")] = 0;

    FILE* f = fopen(ARCH_CUENTAS, "rb+");
    if(!f) { printf("Error al abrir archivo de cuentas.\n"); esperaTecla(); return; }
    stCuenta c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCuenta), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.nroCuenta, nroCuenta) == 0) {
            fseek(f, -sizeof(stCuenta), SEEK_CUR);
            c.eliminado = 1;
            fwrite(&c, sizeof(stCuenta), 1, f);
            printf("Cuenta dada de baja.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró la cuenta.\n");
    fclose(f);
    esperaTecla();
}

void modificarCuenta() {
    char nroCuenta[24];
    printf("\n--- Modificar Cuenta ---\n");
    printf("Ingrese Nro de Cuenta: "); fflush(stdin); fgets(nroCuenta, 24, stdin); nroCuenta[strcspn(nroCuenta, "\n")] = 0;

    FILE* f = fopen(ARCH_CUENTAS, "rb+");
    if(!f) { printf("Error al abrir archivo de cuentas.\n"); esperaTecla(); return; }
    stCuenta c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCuenta), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.nroCuenta, nroCuenta) == 0) {
            printf("Modificar costo mensual (actual: %.2f): ", c.costoMensual); scanf("%f", &c.costoMensual); limpiarBuffer();
            fseek(f, -sizeof(stCuenta), SEEK_CUR);
            fwrite(&c, sizeof(stCuenta), 1, f);
            printf("Datos modificados.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró la cuenta.\n");
    fclose(f);
    esperaTecla();
}

void consultarCuenta() {
    char nroCuenta[24];
    printf("\n--- Consultar Cuenta ---\n");
    printf("Ingrese Nro de Cuenta: "); fflush(stdin); fgets(nroCuenta, 24, stdin); nroCuenta[strcspn(nroCuenta, "\n")] = 0;

    FILE* f = fopen(ARCH_CUENTAS, "rb");
    if(!f) { printf("Error al abrir archivo de cuentas.\n"); esperaTecla(); return; }
    stCuenta c;
    int encontrado = 0;
    while(fread(&c, sizeof(stCuenta), 1, f)) {
        if(c.eliminado == 0 && strcmp(c.nroCuenta, nroCuenta) == 0) {
            printf("ID:%d | IDCliente:%d | Nro:%s | Saldo:%.2f\n", c.id, c.idCliente, c.nroCuenta, c.saldo);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró la cuenta.\n");
    fclose(f);
    esperaTecla();
}

void listarCuentasPorCliente() {
    int idCliente;
    printf("\n--- Listar Cuentas por Cliente ---\n");
    printf("Ingrese ID de Cliente: "); scanf("%d", &idCliente); limpiarBuffer();

    FILE* f = fopen(ARCH_CUENTAS, "rb");
    if(!f) { printf("Error al abrir archivo de cuentas.\n"); esperaTecla(); return; }
    stCuenta c;
    int hay = 0;
    while(fread(&c, sizeof(stCuenta), 1, f)) {
        if(c.eliminado == 0 && c.idCliente == idCliente) {
            printf("ID:%d | Nro:%s | Tipo:%d | Saldo:%.2f\n", c.id, c.nroCuenta, c.tipoDeCuenta, c.saldo);
            hay = 1;
        }
    }
    if(!hay) printf("No hay cuentas para ese cliente.\n");
    fclose(f);
    esperaTecla();
}

// ========================== MOVIMIENTOS ==========================
void altaMovimiento() {
    stMovimiento m;
    m.id = proximoId(ARCH_MOVIMIENTOS, sizeof(stMovimiento));
    printf("\n--- Alta de Movimiento ---\n");
    printf("ID de Cuenta: "); scanf("%d", &m.idCuenta); limpiarBuffer();

    FILE* fctas = fopen(ARCH_CUENTAS, "rb+");
    if(!fctas) { printf("No hay cuentas creadas.\n"); esperaTecla(); return; }
    stCuenta cta; int existe = 0, posCta = -1, idx = 0;
    while(fread(&cta, sizeof(stCuenta), 1, fctas)) {
        if(cta.id == m.idCuenta && cta.eliminado == 0) { existe = 1; posCta = idx; break; }
        idx++;
    }
    if(!existe) { fclose(fctas); printf("Cuenta no encontrada.\n"); esperaTecla(); return; }

    printf("Detalle: "); fflush(stdin); fgets(m.detalle, 100, stdin); m.detalle[strcspn(m.detalle, "\n")] = 0;
    printf("Importe (+/-): "); scanf("%f", &m.importe); limpiarBuffer();
    printf("Año: "); scanf("%d", &m.anio); limpiarBuffer();
    printf("Mes: "); scanf("%d", &m.mes); limpiarBuffer();
    printf("Dia: "); scanf("%d", &m.dia); limpiarBuffer();
    m.eliminado = 0;

    fseek(fctas, posCta * sizeof(stCuenta), SEEK_SET);
    fread(&cta, sizeof(stCuenta), 1, fctas);
    cta.saldo += m.importe;
    fseek(fctas, posCta * sizeof(stCuenta), SEEK_SET);
    fwrite(&cta, sizeof(stCuenta), 1, fctas);
    fclose(fctas);

    FILE* f = fopen(ARCH_MOVIMIENTOS, "ab");
    fwrite(&m, sizeof(stMovimiento), 1, f);
    fclose(f);

    printf("Movimiento registrado y saldo actualizado.\n");
    esperaTecla();
}

void bajaMovimiento() {
    int id;
    printf("\n--- Baja de Movimiento ---\n");
    printf("Ingrese ID: "); scanf("%d", &id); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb+");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int encontrado = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.id == id) {
            fseek(f, -sizeof(stMovimiento), SEEK_CUR);
            m.eliminado = 1;
            fwrite(&m, sizeof(stMovimiento), 1, f);
            printf("Movimiento dado de baja.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el movimiento.\n");
    fclose(f);
    esperaTecla();
}

void modificarMovimiento() {
    int id;
    printf("\n--- Modificar Movimiento ---\n");
    printf("Ingrese ID: "); scanf("%d", &id); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb+");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int encontrado = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.id == id) {
            printf("Modificar detalle (actual: %s): ", m.detalle); fflush(stdin); fgets(m.detalle, 100, stdin); m.detalle[strcspn(m.detalle, "\n")] = 0;
            fseek(f, -sizeof(stMovimiento), SEEK_CUR);
            fwrite(&m, sizeof(stMovimiento), 1, f);
            printf("Detalle modificado.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el movimiento.\n");
    fclose(f);
    esperaTecla();
}

void consultarMovimiento() {
    int id;
    printf("\n--- Consultar Movimiento ---\n");
    printf("Ingrese ID: "); scanf("%d", &id); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int encontrado = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.id == id) {
            printf("ID:%d | Cuenta:%d | Detalle:%s | Importe:%.2f | %02d/%02d/%d\n", m.id, m.idCuenta, m.detalle, m.importe, m.dia, m.mes, m.anio);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el movimiento.\n");
    fclose(f);
    esperaTecla();
}

void listarMovimientosPorCuenta() {
    int idCuenta;
    printf("\n--- Listar Movimientos por Cuenta ---\n");
    printf("Ingrese ID de Cuenta: "); scanf("%d", &idCuenta); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int hay = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.idCuenta == idCuenta) {
            printf("ID:%d | Detalle:%s | Importe:%.2f | Fecha:%02d/%02d/%d\n", m.id, m.detalle, m.importe, m.dia, m.mes, m.anio);
            hay = 1;
        }
    }
    if(!hay) printf("No hay movimientos para esa cuenta.\n");
    fclose(f);
    esperaTecla();
}

void listarMovimientosPorMes() {
    int mes, anio;
    printf("\n--- Listar Movimientos por Mes ---\n");
    printf("Ingrese mes (1-12): "); scanf("%d", &mes); limpiarBuffer();
    printf("Ingrese año: "); scanf("%d", &anio); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int hay = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.mes == mes && m.anio == anio) {
            printf("ID:%d | Cuenta:%d | Detalle:%s | Importe:%.2f | Fecha:%02d/%02d/%d\n", m.id, m.idCuenta, m.detalle, m.importe, m.dia, m.mes, m.anio);
            hay = 1;
        }
    }
    if(!hay) printf("No hay movimientos para esa fecha.\n");
    fclose(f);
    esperaTecla();
}

void buscarMovimientosPorFecha() {
    int dia, mes, anio;
    printf("\n--- Buscar Movimientos por Fecha ---\n");
    printf("Ingrese día: "); scanf("%d", &dia); limpiarBuffer();
    printf("Ingrese mes: "); scanf("%d", &mes); limpiarBuffer();
    printf("Ingrese año: "); scanf("%d", &anio); limpiarBuffer();

    FILE* f = fopen(ARCH_MOVIMIENTOS, "rb");
    if(!f) { printf("Error al abrir archivo de movimientos.\n"); esperaTecla(); return; }
    stMovimiento m;
    int hay = 0;
    while(fread(&m, sizeof(stMovimiento), 1, f)) {
        if(m.eliminado == 0 && m.dia == dia && m.mes == mes && m.anio == anio) {
            printf("ID:%d | Cuenta:%d | Detalle:%s | Importe:%.2f\n", m.id, m.idCuenta, m.detalle, m.importe);
            hay = 1;
        }
    }
    if(!hay) printf("No hay movimientos para esa fecha.\n");
    fclose(f);
    esperaTecla();
}

void cargarMovimientosAleatorios() {
    printf("\n--- Cargando 1000 movimientos aleatorios ---\n");
    FILE* fctas = fopen(ARCH_CUENTAS, "rb+");
    if(!fctas) { printf("Debe haber cuentas creadas!\n"); esperaTecla(); return; }
    fseek(fctas, 0, SEEK_END);
    int cantCtas = ftell(fctas) / sizeof(stCuenta);
    if (cantCtas == 0) { fclose(fctas); printf("Debe haber cuentas creadas!\n"); esperaTecla(); return; }
    rewind(fctas);

    stCuenta* cuentas = malloc(sizeof(stCuenta) * cantCtas);
    fread(cuentas, sizeof(stCuenta), cantCtas, fctas);
    fclose(fctas);

    FILE* fmov = fopen(ARCH_MOVIMIENTOS, "ab");
    FILE* fctasw = fopen(ARCH_CUENTAS, "rb+");
    srand(time(NULL));
    int anio = anioActual();

    for (int i = 0; i < 1000; i++) {
        stMovimiento m;
        m.id = proximoId(ARCH_MOVIMIENTOS, sizeof(stMovimiento));
        int idxCta = rand() % cantCtas;
        m.idCuenta = cuentas[idxCta].id;
        m.anio = anio;
        m.mes = (rand() % mesActual()) + 1;
        m.dia = (rand() % 28) + 1;
        m.eliminado = 0;

        float importe = ((rand() % 20000) / 100.0f) * ((rand() % 2) ? 1 : -1);
        m.importe = importe;

        if(importe > 0) {
            char* ops[] = {"Credito haberes", "Transf propia", "Acred plazo fijo", "Transf ajena"};
            strcpy(m.detalle, ops[rand() % 4]);
        } else {
            char* ops[] = {"Pago TC", "Transf terceros", "Pago TD", "Pago servicio"};
            strcpy(m.detalle, ops[rand() % 4]);
        }
        for (int j = 0; j < cantCtas; j++) {
            if(cuentas[j].id == m.idCuenta) {
                cuentas[j].saldo += importe;
                break;
            }
        }
        fwrite(&m, sizeof(stMovimiento), 1, fmov);
    }
    fctasw = fopen(ARCH_CUENTAS, "rb+");
    for (int j = 0; j < cantCtas; j++) {
        fseek(fctasw, j * sizeof(stCuenta), SEEK_SET);
        fwrite(&cuentas[j], sizeof(stCuenta), 1, fctasw);
    }
    fclose(fctasw);
    fclose(fmov);
    free(cuentas);

    printf("1000 movimientos aleatorios cargados.\n");
    esperaTecla();
}

// ========================== MENÚS ==========================

