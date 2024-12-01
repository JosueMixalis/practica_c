#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NODOS 100       // Número máximo de nodos en el grafo
#define MAX_COLORES 10      // Número máximo de colores
#define TAM_POBLACION 20    // Tamaño de la población

// Estructura de un cromosoma
typedef struct {
    int colores[MAX_NODOS]; // Asignación de colores para los nodos
    int fitness;            // Valor de aptitud (conflictos)
} Cromosoma;

// Prototipos de funciones
void inicializarCromosoma(Cromosoma *cromosoma, int numNodos, int numColores) {
    for (int i = 0; i < numNodos; i++) {
        cromosoma->colores[i] = rand() % numColores;
    }
    cromosoma->fitness = 0;
}

// Inicializa una población
void inicializarPoblacion(Cromosoma *poblacion, int tamPoblacion, int numNodos, int numColores) {
    for (int i = 0; i < tamPoblacion; i++) {
        inicializarCromosoma(&poblacion[i], numNodos, numColores);
    }
}

int calcularFitness(Cromosoma *cromosoma, int grafo[MAX_NODOS][MAX_NODOS], int numNodos) {
    int conflictos = 0;
    for (int i = 0; i < numNodos; i++) {
        for (int j = i + 1; j < numNodos; j++) {
            if (grafo[i][j] && cromosoma->colores[i] == cromosoma->colores[j]) {
                conflictos++;
            }
        }
    }
    cromosoma->fitness = conflictos;
    return conflictos;
}

// Evaluar toda la población
void evaluarPoblacion(Cromosoma *poblacion, int tamPoblacion, int grafo[MAX_NODOS][MAX_NODOS], int numNodos) {
    for (int i = 0; i < tamPoblacion; i++) {
        calcularFitness(&poblacion[i], grafo, numNodos);
    }
}

void cruzarCromosomas(Cromosoma *padre1, Cromosoma *padre2, Cromosoma *hijo1, Cromosoma *hijo2, int numNodos) {
    int puntoCruza = rand() % numNodos;
    for (int i = 0; i < numNodos; i++) {
        if (i < puntoCruza) {
            hijo1->colores[i] = padre1->colores[i];
            hijo2->colores[i] = padre2->colores[i];
        } else {
            hijo1->colores[i] = padre2->colores[i];
            hijo2->colores[i] = padre1->colores[i];
        }
    }
}

void mutarCromosoma(Cromosoma *cromosoma, int numNodos, int numColores, float probMutacion) {
    for (int i = 0; i < numNodos; i++) {
        if ((float)rand() / RAND_MAX <= probMutacion) {
            cromosoma->colores[i] = rand() % numColores;
        }
    }
}

void seleccionarMejores(Cromosoma *poblacion, int tamPoblacion, Cromosoma *nuevaPoblacion, int tamNuevaPoblacion) {
    // Ordenar la población por fitness (menor es mejor)
    for (int i = 0; i < tamPoblacion - 1; i++) {
        for (int j = i + 1; j < tamPoblacion; j++) {
            if (poblacion[i].fitness > poblacion[j].fitness) {
                Cromosoma temp = poblacion[i];
                poblacion[i] = poblacion[j];
                poblacion[j] = temp;
            }
        }
    }

    // Copiar los mejores cromosomas
    for (int i = 0; i < tamNuevaPoblacion; i++) {
        nuevaPoblacion[i] = poblacion[i];
    }
}

void algoritmoGenetico(int grafo[MAX_NODOS][MAX_NODOS], int numNodos, int numColores, int maxGeneraciones) {
    Cromosoma poblacion[TAM_POBLACION];
    Cromosoma nuevaPoblacion[TAM_POBLACION];
    float probMutacion = 0.1;
    int generacion = 0;

    // Inicializar
    inicializarPoblacion(poblacion, TAM_POBLACION, numNodos, numColores);
    evaluarPoblacion(poblacion, TAM_POBLACION, grafo, numNodos);

    while (generacion < maxGeneraciones) {
        printf("Generación %d\n", generacion);

        // Evaluar población
        evaluarPoblacion(poblacion, TAM_POBLACION, grafo, numNodos);

        // Seleccionar mejores
        seleccionarMejores(poblacion, TAM_POBLACION, nuevaPoblacion, TAM_POBLACION / 2);

        // Cruce y mutación
        for (int i = 0; i < TAM_POBLACION / 2; i += 2) {
            cruzarCromosomas(&nuevaPoblacion[i], &nuevaPoblacion[i + 1], &poblacion[i], &poblacion[i + 1], numNodos);
            mutarCromosoma(&poblacion[i], numNodos, numColores, probMutacion);
            mutarCromosoma(&poblacion[i + 1], numNodos, numColores, probMutacion);
        }

        generacion++;
    }
}

// Programa principal
int main() {
    srand(time(NULL)); // Inicializar semilla para números aleatorios

    // **Definición de variables del problema**
    int grafo[MAX_NODOS][MAX_NODOS] = {0}; // Matriz de adyacencia del grafo
    int numNodos, numColores, maxGeneraciones;

    // **Lectura de datos del problema**
    printf("Ingrese el número de nodos del grafo: ");
    scanf("%d", &numNodos);
    printf("Ingrese el número de colores disponibles: ");
    scanf("%d", &numColores);
    printf("Ingrese el número máximo de generaciones: ");
    scanf("%d", &maxGeneraciones);

    // **Construcción del grafo (matriz de adyacencia)**
    printf("Ingrese la matriz de adyacencia (%d x %d):\n", numNodos, numNodos);
    for (int i = 0; i < numNodos; i++) {
        for (int j = 0; j < numNodos; j++) {
            scanf("%d", &grafo[i][j]);
        }
    }

    // **Llamar al algoritmo genético**
    algoritmoGenetico(grafo, numNodos, numColores, maxGeneraciones);

    return 0;
}

// **Aquí incluirás los módulos previamente diseñados**
