#include "cuentas.h"
#include "clientes.h"
#include "utils.h"

//============================MENU=============================
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

// ========================== FUNCIONES CUENTAS ==========================

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
