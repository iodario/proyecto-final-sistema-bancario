#include "movimientos.h"
#include "cuentas.h"


//============================MENU=============================
void menuMovimientos() {
    limpiarPantalla();
    printMarco();
    colorTitulo();
    printCenter(3, "==== Gestion de Movimientos ====");
    colorNormal();
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



// ========================== FUNCIONES MOVIMIENTOS ==========================

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
