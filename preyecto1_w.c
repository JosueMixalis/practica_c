#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

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
void algoritmoGenetico(DatosArchivo *datos);

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
    printf("    2. Josué Mixalis García Carbajal\n");
    printf("    3. Arturo Tom Ortiz\n");
    printf("    FECHA DE CREACION: 09/11/2024\n\n");
    printf("    Presione cualquier tecla para continuar...");
    getchar();
}

int main() {
    int opcion;
    
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
            case 1: {
                limpiarPantalla();
                imprimeLogo();
                char nombreArchivo[100];
                printf("APERTURA DE ARCHIVO PARA CARGAR DATOS\n");
                printf("Proporciona el nombre del archivo con extension\n");
                scanf("%s", nombreArchivo);
                DatosArchivo* datos = procesarArchivoDat(nombreArchivo);
                if (datos != NULL) {
                    algoritmoGenetico(datos);
                    liberarDatos(datos);
                }
                break;
            }
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
    DatosArchivo *datos = malloc(sizeof(DatosArchivo));
    
    datos->areas = malloc(100 * sizeof(float));
    datos->longitudes = malloc(100 * sizeof(float));
    datos->numAreas = 0;
    
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Fichero: %s -> Error de Apertura!!! (NO ABIERTO)\n", nombreArchivo);
        printf("Ingresa otro nombre correcto del archivo\n");
        free(datos->areas);
        free(datos->longitudes);
        free(datos);
        return NULL;
    }
    
    fscanf(archivo, "%f", &datos->probCruza);
    fscanf(archivo, "%f", &datos->probMutacion);
    fscanf(archivo, "%d", &datos->numObjetos);
    fscanf(archivo, "%f", &datos->capacidadMaxima);

    datos->probCruza /= 100;
    datos->probMutacion /= 100;

    printf("Probabilidad de Cruza = %.2f\n", datos->probCruza);
    printf("Probabilidad de Mutacion = %.2f\n", datos->probMutacion);
    printf("Num de objetos = %d\n", datos->numObjetos);
    printf("Capacidad maxima = %.2f\n\n", datos->capacidadMaxima);

    printf("Areas\tLongitud\n");
    float area, longitud;
    while (fscanf(archivo, "%f %f", &area, &longitud) == 2) {
        datos->areas[datos->numAreas] = area;
        datos->longitudes[datos->numAreas] = longitud;
        printf("%.2f\t%.0f\n", area, longitud);
        datos->numAreas++;
    }

    fclose(archivo);
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

void algoritmoGenetico(DatosArchivo *datos) {
    printf("SOLICITUD DE DATOS PARA INICIAR EL ALGORITMO GENETICO\n");
    printf("Escribe el numero de cromosomas para la poblacion: ");
    int numCromosomas;
    scanf("%d", &numCromosomas);
    printf("Escribe el numero de iteraciones (Generaciones): ");
    int numGeneraciones;
    scanf("%d", &numGeneraciones);

    srand(time(NULL));

    int **poblacion = (int **)malloc(numCromosomas * sizeof(int *));
    for (int i = 0; i < numCromosomas; i++) {
        poblacion[i] = (int *)malloc(datos->numObjetos * sizeof(int));
    }
    float *pesos = (float *)malloc(numCromosomas * sizeof(float));
    float *aptitudes = (float *)malloc(numCromosomas * sizeof(float));

    // Inicializar la poblacion
    for (int i = 0; i < numCromosomas; i++) {
        printf("Cromosoma[%d] = [ ", i);
        for (int j = 0; j < datos->numObjetos; j++) {
            poblacion[i][j] = rand() % 2;
            printf("%d ", poblacion[i][j]);
        }
        printf("]\n");
    }

    for (int gen = 0; gen < numGeneraciones; gen++) {
        printf("\nEVALUACION DE LA POBLACION DE LA ITERACION %d\n", gen + 1);
        printf("Cromosoma                             Peso           Funcion de Aptitud\n");
        printf("---------------------------------------------------|-------------|----------------\n");

        float aptitudTotal = 0.0;

        // Evaluación de la poblacion
        for (int i = 0; i < numCromosomas; i++) {
            pesos[i] = 0;
            for (int j = 0; j < datos->numObjetos; j++) {
                if (poblacion[i][j] == 1) {
                    pesos[i] += datos->areas[j];
                }
            }

            float diferencia = datos->capacidadMaxima - pesos[i];
            aptitudes[i] = diferencia > 0 ? 1.0 / (1.0 + diferencia) : 1.0;
            aptitudTotal += aptitudes[i];

            // Imprimir el cromosoma, su peso y aptitud
            printf("[%d] = [ ", i);
            for (int j = 0; j < datos->numObjetos; j++) printf("%d ", poblacion[i][j]);
            printf("]   %.2f         %.6f\n", pesos[i], aptitudes[i]);
        }

        printf("\nF = %.6f\n", aptitudTotal);
        printf("Generacion No. : %d\n", gen + 1);

        // Seleccion por ruleta
        int **nuevaPoblacion = (int **)malloc(numCromosomas * sizeof(int *));
        for (int i = 0; i < numCromosomas; i++) {
            nuevaPoblacion[i] = (int *)malloc(datos->numObjetos * sizeof(int));
            float r = ((float)rand() / RAND_MAX) * aptitudTotal;
            float suma = 0;
            for (int j = 0; j < numCromosomas; j++) {
                suma += aptitudes[j];
                if (suma >= r) {
                    memcpy(nuevaPoblacion[i], poblacion[j], datos->numObjetos * sizeof(int));
                    break;
                }
            }
        }

        // Cruzamiento de un punto
        for (int i = 0; i < numCromosomas; i += 2) {
            if ((float)rand() / RAND_MAX < datos->probCruza) {
                int puntoCruza = rand() % datos->numObjetos;
                printf("Cruzamiento entre cromosomas %d y %d en el punto %d\n", i, i + 1, puntoCruza);
                for (int j = puntoCruza; j < datos->numObjetos; j++) {
                    int temp = nuevaPoblacion[i][j];
                    nuevaPoblacion[i][j] = nuevaPoblacion[i + 1][j];
                    nuevaPoblacion[i + 1][j] = temp;
                }
            }
        }

        // Mutacion
        printf("\nMUTACION\n");
        for (int i = 0; i < numCromosomas; i++) {
            for (int j = 0; j < datos->numObjetos; j++) {
                if ((float)rand() / RAND_MAX < datos->probMutacion) {
                    nuevaPoblacion[i][j] = !nuevaPoblacion[i][j];
                    printf("Mutacion en cromosoma %d, gen %d\n", i, j);
                }
            }
            // Imprimir el cromosoma después de la mutacion
            printf("Cromosoma[%d] despues de mutacion = [ ", i);
            for (int j = 0; j < datos->numObjetos; j++) {
                printf("%d ", nuevaPoblacion[i][j]);
            }
            printf("]\n");
        }

        // Reemplazo de la población
        for (int i = 0; i < numCromosomas; i++) {
            free(poblacion[i]);
            poblacion[i] = nuevaPoblacion[i];
        }
        free(nuevaPoblacion);

        // Calcular y mostrar probabilidades para seleccion
        printf("\nPROBABILIDADES POR CADA CROMOSOMA\n");
        for (int i = 0; i < numCromosomas; i++) {
            float probabilidad = aptitudes[i] / aptitudTotal;
            printf("p[%d] = %.6f\n", i, probabilidad);
        }

        printf("\nPresiona Enter para continuar a la siguiente generacion...");
        getchar();
    }

    for (int i = 0; i < numCromosomas; i++) {
        free(poblacion[i]);
    }
    free(poblacion);
    free(pesos);
    free(aptitudes);
}
