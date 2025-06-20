#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define ARCH_CLIENTES "clientes.dat"
#define ARCH_CUENTAS "cuentas.dat"
#define ARCH_MOVIMIENTOS "movimientos.dat"
#define MAX_STR 100

// ========== ESTRUCTURAS ==========
typedef struct {
    char calle[30];
    char nro[6];
    char localidad[50];
    char provincia[40];
    char cpos[6];
} stDomicilio;

typedef struct {
    int id;
    int nroCliente;
    char nombre[30];
    char apellido[30];
    char dni[10];
    char email[30];
    stDomicilio domicilio;
    char telefonoFijo[12];
    char telefonoMovil[12];
    int eliminado;
} stCliente;

typedef struct {
    int id;
    int idCliente;
    char nroCuenta[24];
    char cbu[24];
    int tipoDeCuenta;
    float costoMensual;
    float saldo;
    int eliminado;
} stCuenta;

typedef struct {
    int id;
    int idCuenta;
    char detalle[100];
    float importe;
    int anio;
    int mes;
    int dia;
    int eliminado;
} stMovimiento;

// ========== FUNCIONES VISUALES ==========
void limpiarPantalla();
void gotoxy(int x, int y);
void colorNormal();
void colorTitulo();
void colorMenu();
void colorError();
void colorDato();
void colorLinea();
void beepOk();
void beepError();
void beepMenu();
void beepConfirm();
void printMarco();
void printCenter(int y, const char* texto);
void printLinea(int y);
void esperaTecla();

// ========== FUNCIONES UTILITARIAS ==========
void limpiarBuffer();
int anioActual();
int mesActual();
int diaActual();
int esNumerico(const char* str);
int proximoId(const char* archivo, size_t tam);

// ========== CLIENTES ==========
void altaCliente();
void bajaCliente();
void modificarCliente();
void consultarCliente();
void listarClientes();
void buscarClientePorApellidoODni();

// ========== CUENTAS ==========
void altaCuenta();
void bajaCuenta();
void modificarCuenta();
void consultarCuenta();
void listarCuentasPorCliente();

// ========== MOVIMIENTOS ==========
void altaMovimiento();
void bajaMovimiento();
void modificarMovimiento();
void consultarMovimiento();
void listarMovimientosPorCuenta();
void listarMovimientosPorMes();
void buscarMovimientosPorFecha();
void cargarMovimientosAleatorios();

// ========== MENUS ==========
void menuClientes();
void menuCuentas();
void menuMovimientos();

#endif
