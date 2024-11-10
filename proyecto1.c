#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



typedef struct {
    float probCruza;
    float probMutacion;
    int numObjetos;
    float capacidadMaxima;
    float *areas;
    float *longitudes;
    int numAreas;
} DatosArchivo;

void imprimeLogo();
void limpiarPantalla();
void pantallaInicial();
DatosArchivo* procesarArchivoDat(const char* nombreArchivo);
void liberarDatos(DatosArchivo *datos);


void imprimeLogo() {
    printf("\n");
    printf("        ****     ******     ****\n");
    printf("       **   **  **    **  **    **\n");
    printf("      **     ****      ****      **\n");
    printf("     **       ***      ***        **\n");
    printf("    **       **  ******  **        **\n");
    printf("\n");
    printf("    Unidad Iztapalapa\n");
    printf("\n");
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pantallaInicial() {
    imprimeLogo();
    printf("\n");
    printf("    PROGRAMACIÓN CONCURRENTE\n");
    printf("    PRÁCTICA III\n");
    printf("    ALGORITMO GENÉTICO DE ARMADURAS PLANAS\n\n");
    printf("    PROFESOR.- BEJAMÍN MORENO MONTIEL\n");
    printf("    INTEGRANTES:\n");
    printf("    1. Ulises Juárez Guzmán\n");
    printf("    2. Josué Mizalis García Carbajal\n");
    printf("    3. Arturo Tom Ortiz\n");
    printf("    FECHA DE CREACION: 09/11/2024\n\n");
    printf("    Presione cualquier tecla para continuar...");
    getchar();
}

int main() {
    int opcion;
    char enter;
    
    limpiarPantalla();
    pantallaInicial();
    limpiarPantalla();
    
    do {
        imprimeLogo();
        printf("    MENÚ PRINCIPAL\n\n");
        printf("    1. Leer archivo e inicializar algoritmo genético\n");
        printf("    2. Salir\n\n");
        printf("    Seleccione una opción (1-2): ");
        scanf("%d", &opcion);
        while(getchar() != '\n'); 
        
        switch(opcion) {
            case 1:
                limpiarPantalla();
                imprimeLogo();
                char nombreArchivo[100];
                printf("APERTURA DE ARCHIVO PARA CARGAR DATOS\n");
                printf("Proporciona el nombre del archivo con extension\n");
                scanf("%s", nombreArchivo);
                procesarArchivoDat(nombreArchivo);
                break;
                
            case 2:
                limpiarPantalla();
                imprimeLogo();
                printf("\n    Gracias por usar el programa.\n\n");
                printf("    Presiona Enter para salir...");
                getchar();
                break;
                
            default:
                printf("\n    Opción no válida. Presiona Enter para continuar...");
                getchar();
        }
    } while(opcion != 2);
    
    return 0;
}

DatosArchivo* procesarArchivoDat(const char* nombreArchivo) {
    FILE *archivo;
    char linea[256];
    DatosArchivo *datos = malloc(sizeof(DatosArchivo));
    
    // Asignar memoria
    datos->areas = malloc(100 * sizeof(float));  // Tamaño inicial
    datos->longitudes = malloc(100 * sizeof(float));
    datos->numAreas = 0;
    
    // Abrir archivo
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Fichero: %s -> Error de Apertura!!! (NO ABIERTO)\n", nombreArchivo);
        printf("Ingresa otro nombre correcto del archivo\n");
        free(datos->areas);
        free(datos->longitudes);
        free(datos);
        return NULL;
    }
    
    printf("\nAPERTURA DE ARCHIVO PARA CARGAR DATOS\n");
    printf("Proporciona el nombre del archivo con extension\n");
    printf("Fichero: %s -> Apertura Exitosa!!! (ABIERTO)\n", nombreArchivo);
    printf("\nLos Datos leidos fueron los siguientes\n\n");

    fscanf(archivo, "%f", &datos->probCruza);
    fscanf(archivo, "%f", &datos->probMutacion);
    fscanf(archivo, "%d", &datos->numObjetos);
    fscanf(archivo, "%f", &datos->capacidadMaxima);

    //convertimos en prob los porcentajes
    datos->probCruza = datos->probCruza / 100;
    datos->probMutacion = datos->probMutacion / 100;

    // Imprimimos
    printf("Probabilidad de Cruza = %.2f\n", datos->probCruza);
    printf("Probabilidad de Mutacion = %.2f\n", datos->probMutacion);
    printf("Num de objetos = %d\n", datos->numObjetos);
    printf("Capacidad maxima = %.2f\n\n", datos->capacidadMaxima);

    // Imprimir tabla
    printf("Areas\tLongitud\n");

    float area, longitud;
    while (fscanf(archivo, "%f %f", &area, &longitud) == 2) {
        datos->areas[datos->numAreas] = area;
        datos->longitudes[datos->numAreas] = longitud;
        printf("%.2f\t%.0f\n", area, longitud);
        datos->numAreas++;
    }

    fclose(archivo);
    while(getchar() != '\n');
    
    printf("    Presiona Enter para continuar...");
    getchar();
    return datos;
}

void liberarDatos(DatosArchivo *datos) {
    if (datos != NULL) {
        free(datos->areas);
        free(datos->longitudes);
        free(datos);
    }
}