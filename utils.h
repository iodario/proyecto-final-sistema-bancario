#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_STR 100

int emailValido(char email[]);
void limpiarBuffer();
int anioActual();
int mesActual();
int diaActual();
int esNumerico(const char* str);
int proximoId(const char* archivo, size_t tam);


#endif // UTILS_H_INCLUDED
