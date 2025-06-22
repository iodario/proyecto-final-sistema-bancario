#ifndef MOVIMIENTOS_H_INCLUDED
#define MOVIMIENTOS_H_INCLUDED

#include <stdio.h>
#include "utils.h"
#include "interfaz.h"

#define ARCH_MOVIMIENTOS "movimientos.dat"

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


// ========== PROTOTIPADO MOVIMIENTOS ==========
void menuMovimientos();
void altaMovimiento();
void bajaMovimiento();
void modificarMovimiento();
void consultarMovimiento();
void listarMovimientosPorCuenta();
void listarMovimientosPorMes();
void buscarMovimientosPorFecha();
void cargarMovimientosAleatorios();


#endif // MOVIMIENTOS_H_INCLUDED
