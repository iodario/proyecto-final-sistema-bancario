#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gotoxy.h"
#include <windows.h> // Para Sleep
#include "interfaz.h"
#include "utils.h"

#include "clientes.h"
#include "cuentas.h"





// Prototipos de los submenús
void abmClientes();
void abmCuentas();
void abmMovimientos();

int main() {
    int opcion;
    do {
        limpiarPantalla();
        printMarco();
        colorTitulo();
        printCenter(2, "=========================================");
        printCenter(3, "    Banco B.F.S.A. (Bicicleta Facil S.A)");
        printCenter(4, "=========================================");
        colorMenu();
        printCenter(6,  "1. Clientes");
        printCenter(7,  "2. Cuentas");
        printCenter(8,  "3. Movimientos");
        printCenter(9,  "4. Cargar 1000 movimientos aleatorios");
        printCenter(10, "5. Buscar clientes por apellido/dni");
        printCenter(11, "6. Buscar movimientos por fecha");
        printCenter(12, "0. Salir");
        colorNormal();
        printLinea(13);
        printCenter(15, "Seleccione una opcion: ");
        opcion = getch();
        beepMenu();

        switch(opcion) {
            case '1': abmClientes(); break;
            case '2': abmCuentas(); break;
            case '3': abmMovimientos(); break;
            case '4': cargarMovimientosAleatorios(); break;
            case '5': buscarClientePorApellidoODni(); break;
            case '6': buscarMovimientosPorFecha(); break;
        }
    } while(opcion != '0');

    colorTitulo();
    limpiarPantalla();
    printMarco();
    printCenter(12, "Gracias por usar el sistema B.F.S.A.");
    colorNormal();
    beepOk(); beepOk();
    Sleep(700);
    limpiarPantalla();
    return 0;
}

// =================== Submenú Clientes ===================
void abmClientes() {
    int opcion;
    do {
        menuClientes();
        opcion = getch();
        beepMenu();
        switch(opcion) {
            case '1': altaCliente(); break;
            case '2': bajaCliente(); break;
            case '3': modificarCliente(); break;
            case '4': consultarCliente(); break;
            case '5': listarClientes(); break;
            case '0': break;
            default:
                colorError(); printCenter(16, "Opción inválida."); colorNormal(); beepError(); esperaTecla();
        }
    } while(opcion != '0');
}

// =================== Submenú Cuentas ===================
void abmCuentas() {
    int opcion;
    do {
        menuCuentas();
        opcion = getch();
        beepMenu();
        switch(opcion) {
            case '1': altaCuenta(); break;
            case '2': bajaCuenta(); break;
            case '3': modificarCuenta(); break;
            case '4': consultarCuenta(); break;
            case '5': listarCuentasPorCliente(); break;
            case '0': break;
            default:
                colorError(); printCenter(16, "Opción inválida."); colorNormal(); beepError(); esperaTecla();
        }
    } while(opcion != '0');
}

// =================== Submenú Movimientos ===================
void abmMovimientos() {
    int opcion;
    do {
        menuMovimientos();
        opcion = getch();
        beepMenu();
        switch(opcion) {
            case '1': altaMovimiento(); break;
            case '2': bajaMovimiento(); break;
            case '3': modificarMovimiento(); break;
            case '4': consultarMovimiento(); break;
            case '5': listarMovimientosPorCuenta(); break;
            case '6': listarMovimientosPorMes(); break;
            case '0': break;
            default:
                colorError(); printCenter(18, "Opción inválida."); colorNormal(); beepError(); esperaTecla();
        }
    } while(opcion != '0');
}
