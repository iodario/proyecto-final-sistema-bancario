#include "utils.h"
#include "string.h"

/*Contiene funciones de uso com�n que no pertenecen a un m�dulo espec�fico
    (como clientes o cuentas), pero que son reutilizadas en varios archivos .c del proyecto.

    Incluye:
    Limpieza de buffer de entrada,
    Obtenci�n de fecha actual (d�a, mes, a�o),
    Validaciones de strings num�ricos y email valido,
    Generaci�n del pr�ximo ID para archivos binarios
*/

/** DARIO TURCHI **/
int emailValido(const char* email) {
    return (strchr(email, '@') != NULL);
}

/** DARIO TURCHI **/
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);    //trabaja como si fuera fflush(stdin) pero tiene mejor portabilidad, seguridad y comportamiento
}

int anioActual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

int mesActual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon + 1;
}

int diaActual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday;
}

// esNumerico verifica si una cadena contiene solo caracteres num�ricos (d�gitos 0�9).
// Devuelve 1 si todos los caracteres son n�meros, o 0 si encuentra alguno que no lo es.
/** DARIO TURCHI **/
int esNumerico(const char* str) {
    for (int i = 0; str[i]; i++)
        if (str[i] < '0' || str[i] > '9') return 0;
    return 1;
}

// proximoId abre "clientes.dat", calcula su tama�o, divide por sizeof(stCliente), devuelve la cantidad de registros + 1, ese ser� el nuevo ID
/** DARIO TURCHI **/
int proximoId(const char* archivo, size_t tam) {
    FILE* f = fopen(archivo, "rb");
    if (!f) return 1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return (size / tam) + 1;
}
