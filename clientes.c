#include "clientes.h"
#include "interfaz.h"
#include "utils.h"

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
    printCenter(3, "ALTA DE CLIENTE");     // Título centrado
    printf("\n\n");
    colorNormal();
    beepMenu();                            // Sonido corto de entrada

    // Genera ID y número de cliente aleatorio
    c.id = proximoId(ARCH_CLIENTES, sizeof(stCliente));
    c.nroCliente = c.id * 100 + rand() % 100;

    // Solicita y carga datos del cliente (nombre, apellido, DNI, etc.)
    leerDatosCliente(&c, f);

    // Marca como activo (no eliminado)
    c.eliminado = 0;

    // Guarda en archivo binario
    fwrite(&c, sizeof(stCliente), 1, f);
    fclose(f);

    // Muestra mensaje de éxito con efecto visual básico
    colorMenu();                           // Color verde para éxito
    printCenter(20, "Cliente dado de alta correctamente.");
    colorNormal();
    beepConfirm();                         // Sonido de confirmación
    esperaTecla();
}

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

void leerCampoObligatorio(char* destino, int maxLen, const char* etiqueta) {
    //destino = dónde se va a guardar lo que el usuario escribe
    //maxLen = cantidad máxima de caracteres permitidos
    //etiqueta = el nombre del campo que se va a mostrar al usuario (ej: "Nombre", "Email")
    do {
        printf("%s: ", etiqueta);
        fgets(destino, maxLen, stdin);
        destino[strcspn(destino, "\n")] = 0;

        if (strlen(destino) == 0) {
            printf("El campo \"%s\" no puede estar vacío. Intente nuevamente\n", etiqueta);
        }
        fflush(stdin); //limpia buffer de entrada
    } while (strlen(destino) == 0);
}


void leerDatosCliente(stCliente* c, FILE* f) {
    //toma todos los datos que el cliente ingresa, y llama a las funciones de validacion leerCampoObligatorio( ), emailValido( ) y dniExiste( )

    leerCampoObligatorio(c->nombre, 30, "Nombre");
    leerCampoObligatorio(c->apellido, 30, "Apellido");

    do {
        leerCampoObligatorio(c->email, 30, "Email");
        if (!emailValido(c->email)) {
            printf("Email invalido. Intente nuevamente.\n");
            c->email[0] = '\0';
        }
    } while (strlen(c->email) == 0);

    do {
        leerCampoObligatorio(c->dni, 10, "DNI");

        if (!esNumerico(c->dni)) {
            printf("El DNI debe ser numérico.\n");
        } else if (dniExiste(f, c->dni)) {
            printf("Ya existe un cliente con ese DNI.\n");
        } else if (strlen(c->dni) > 8) {
            printf("El DNI debe tener como maximo 8 dígitos.\n");
        } else {
            break;
        }
    } while (1);

    leerCampoObligatorio(c->domicilio.calle,     30, "Calle");
    leerCampoObligatorio(c->domicilio.nro,        6, "Nro");
    leerCampoObligatorio(c->domicilio.localidad, 50, "Localidad");
    leerCampoObligatorio(c->domicilio.provincia, 40, "Provincia");
    leerCampoObligatorio(c->domicilio.cpos,       6, "Código Postal");
    leerCampoObligatorio(c->telefonoFijo,        12, "Teléfono Fijo");
    leerCampoObligatorio(c->telefonoMovil,       12, "Teléfono Móvil");
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
    if(!encontrado) printf("No se encontró el cliente.\n");
    fclose(f);
    esperaTecla();
}

void modificarCliente() {
    char dni[10];
    printf("\n--- Modificar Cliente ---\n");
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
            printf("Modificar nombre (actual: %s): ", c.nombre); fflush(stdin); fgets(c.nombre, 30, stdin); c.nombre[strcspn(c.nombre, "\n")] = 0;
            printf("Modificar apellido (actual: %s): ", c.apellido); fflush(stdin); fgets(c.apellido, 30, stdin); c.apellido[strcspn(c.apellido, "\n")] = 0;
            printf("Modificar email (actual: %s): ", c.email); fflush(stdin); fgets(c.email, 30, stdin); c.email[strcspn(c.email, "\n")] = 0;
            fseek(f, -sizeof(stCliente), SEEK_CUR);
            fwrite(&c, sizeof(stCliente), 1, f);
            printf("Datos modificados.\n");
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) printf("No se encontró el cliente.\n");
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
    if(!encontrado) printf("No se encontró el cliente.\n");
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
    int hay = 0;    // Flag para saber si se encontró al menos un cliente activo

    while (fread(&c, sizeof(stCliente), 1, f)) {
        if (c.eliminado == 0) {       // Si el cliente no está eliminado (es decir, está activo)
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




