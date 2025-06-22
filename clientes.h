#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "interfaz.h"

#define ARCH_CLIENTES "clientes.dat"

// ========== ESTRUCTURAS CLIENTES ==========

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

// ========== PROTOTIPADO CLIENTES  ==========

void menuClientes();
void altaCliente();
int  dniExiste(FILE* f, const char* dni);
int emailExiste(char nombreArchivo[], char email[]);
void leerCampoObligatorio(char* destino, int maxLen, const char* etiqueta);
void leerDatosCliente(stCliente* c, FILE* f);

void bajaCliente();

void modificarCliente();
void consultarCliente();
void listarClientes();
void buscarClientePorApellidoODni();



#endif
