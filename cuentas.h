#ifndef CUENTAS_H_INCLUDED
#define CUENTAS_H_INCLUDED

#include <stdio.h>
#include "utils.h"
#include "interfaz.h"

#define ARCH_CUENTAS "cuentas.dat"
#define ARCH_CLIENTES "clientes.dat"

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

// ========== PROTOTIPADO CUENTAS ==========

void menuCuentas();
void altaCuenta();
void bajaCuenta();
void modificarCuenta();
void consultarCuenta();
void listarCuentasPorCliente();





#endif // CUENTAS_H_INCLUDED
