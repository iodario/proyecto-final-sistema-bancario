#include "clientes.h"
#include "interfaz.h"
#include "utils.h"
#define MAX_STR 100
//============================MENU=============================
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

// =================== ( FUNCIONES CLIENTES ) ===================
/** DARIO TURCHI **/
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
    printCenter(3, "ALTA DE CLIENTE");     // T�tulo centrado
    printf("\n\n");
    colorNormal();
    beepMenu();                            // Sonido corto de entrada

    // Genera ID y n�mero de cliente aleatorio
    c.id = proximoId(ARCH_CLIENTES, sizeof(stCliente));
    c.nroCliente = c.id * 100 + rand() % 100;

    // Solicita y carga datos del cliente (nombre, apellido, DNI, etc.)
    leerDatosCliente(&c, f);

    // Marca como activo (no eliminado)
    c.eliminado = 0;

    // Guarda en archivo binario
    fwrite(&c, sizeof(stCliente), 1, f);
    fclose(f);

    // Muestra mensaje de �xito con efecto visual b�sico
    colorMenu();                           // Color verde para �xito
    printCenter(20, "Cliente dado de alta correctamente.");
    colorNormal();
    beepConfirm();                         // Sonido de confirmaci�n
    esperaTecla();
}

/** DARIO TURCHI **/
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

/** DARIO TURCHI **/
int emailExiste(char nombreArchivo[], char email[]) {
    FILE* f = fopen(nombreArchivo, "rb");
    stCliente aux;

    if (f) {
        while (fread(&aux, sizeof(stCliente), 1, f)) {
            if (aux.eliminado == 0 && strcmp(aux.email, email) == 0) {
                fclose(f);
                return 1; // Email duplicado
            }
        }
        fclose(f);
    }
    return 0; // No encontrado o archivo inv�lido
}

/** DARIO TURCHI **/
void leerCampoObligatorio(char* destino, int maxLen, const char* etiqueta) {
    //destino = d�nde se va a guardar lo que el usuario escribe
    //maxLen = cantidad m�xima de caracteres permitidos
    //etiqueta = el nombre del campo que se va a mostrar al usuario (ej: "Nombre", "Email")
    do {
        printf("%s: ", etiqueta);
        fgets(destino, maxLen, stdin);
        destino[strcspn(destino, "\n")] = 0;

        if (strlen(destino) == 0) {
            printf("El campo \"%s\" no puede estar vac�o. Intente nuevamente\n", etiqueta);
        }
        fflush(stdin); //limpia buffer de entrada
    } while (strlen(destino) == 0);
}

/** DARIO TURCHI **/
void leerDatosCliente(stCliente* c, FILE* f) {
    //toma todos los datos que el cliente ingresa, y llama a las funciones de validacion leerCampoObligatorio( ), emailValido( ) y dniExiste( )

    leerCampoObligatorio(c->nombre, 30, "Nombre");
    leerCampoObligatorio(c->apellido, 30, "Apellido");


    do {
        leerCampoObligatorio(c->email, 30, "Email");

        if (!emailValido(c->email)) {
            printf("Email invalido. Intente nuevamente.\n");
            c->email[0] = '\0';
        } else if (emailExiste(ARCH_CLIENTES, c->email)) {
            printf("Ya existe un cliente con ese Email.\n");
            c->email[0] = '\0';
        }
    } while (strlen(c->email) == 0);


    do {
        leerCampoObligatorio(c->dni, 10, "DNI");

        if (!esNumerico(c->dni)) {
            printf("El DNI debe ser num�rico.\n");
        } else if (dniExiste(f, c->dni)) {
            printf("Ya existe un cliente con ese DNI.\n");
        } else if (strlen(c->dni) > 8) {
            printf("El DNI debe tener como maximo 8 d�gitos.\n");
        } else {
            break;
        }
    } while (1);

    leerCampoObligatorio(c->domicilio.calle,     30, "Calle");
    leerCampoObligatorio(c->domicilio.nro,        6, "Nro");
    leerCampoObligatorio(c->domicilio.localidad, 50, "Localidad");
    leerCampoObligatorio(c->domicilio.provincia, 40, "Provincia");
    leerCampoObligatorio(c->domicilio.cpos,       6, "C�digo Postal");
    leerCampoObligatorio(c->telefonoFijo,        12, "Tel�fono Fijo");
    leerCampoObligatorio(c->telefonoMovil,       12, "Tel�fono M�vil");
}


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
    if(!encontrado) printf("No se encontr� el cliente.\n");
    fclose(f);
    esperaTecla();
}

/** DARIO TURCHI **/
void modificarCliente() {
    char dni[10];
    printf("\n--- Modificar Cliente ---\n");
    printf("Ingrese DNI: "); fflush(stdin); fgets(dni, 10, stdin); dni[strcspn(dni, "\n")] = 0;

    FILE* f = fopen(ARCH_CLIENTES, "rb+"); // Modo lectura/escritura. No crea si no existe.
    if (!f) {
            printf("Error al abrir archivo de clientes.\n");
            esperaTecla();  // Evita continuar si no hay datos que modificar
            return;         //sale de la funcion.
    }

    stCliente c;
    int encontrado = 0;

     while (fread(&c, sizeof(stCliente), 1, f)) {
        if (c.eliminado == 0 && strcmp(c.dni, dni) == 0) {
            printf("Modificar nombre (actual: %s): ", c.nombre); fflush(stdin); fgets(c.nombre, 30, stdin); c.nombre[strcspn(c.nombre, "\n")] = 0;
            printf("Modificar apellido (actual: %s): ", c.apellido); fflush(stdin); fgets(c.apellido, 30, stdin); c.apellido[strcspn(c.apellido, "\n")] = 0;

            // Validar nuevo email
            char emailOriginal[30];         //Antes de pedir el nuevo email, guard� el original en una variable auxiliar emailOriginal
            strcpy(emailOriginal, c.email);  //As� no te bloquea si el cliente quiere dejar su mismo email (lo cual es v�lido).

            do {
                printf("Modificar email (actual: %s): ", c.email);
                fflush(stdin);
                fgets(c.email, 30, stdin);
                c.email[strcspn(c.email, "\n")] = 0;

                if (!emailValido(c.email)) {
                    printf("Email invalido. Intente nuevamente.\n");
                } else if (emailExiste(ARCH_CLIENTES, c.email) && strcmp(c.email, emailOriginal) != 0) {


                    printf("Ya existe un cliente con ese Email.\n");
                } else {
                    break;
                }
            } while (1);

            fseek(f, -sizeof(stCliente), SEEK_CUR);
            fwrite(&c, sizeof(stCliente), 1, f);
            printf("Datos modificados.\n");
            encontrado = 1;
            break;
        }
    }

    if(!encontrado) printf("No se encontr� el cliente.\n");
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
    if(!encontrado) printf("No se encontr� el cliente.\n");
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
    int hay = 0;    // Flag para saber si se encontr� al menos un cliente activo

    while (fread(&c, sizeof(stCliente), 1, f)) {
        if (c.eliminado == 0) {       // Si el cliente no est� eliminado (es decir, est� activo)
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




