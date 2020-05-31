#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct AFNNodo
{
	bool fueVisitado;
	int noEstado;
	char transicion;
	char transicion2;
	bool esInicial;
	bool esFinal;
	bool esEpsilon;
	struct AFNNodo *ant1; //Derecha
	struct AFNNodo *ant2;
	struct AFNNodo *sig1; //Derecha
	struct AFNNodo *sig2;
};

struct AFNNodo *CrearAutomata(char c, int noEstados);
struct AFNNodo *concatenar(struct AFNNodo *tmp, struct AFNNodo *tmp2);
struct AFNNodo *crearCerraduraKleen(struct AFNNodo *tmp, int noEstados);
struct AFNNodo *crearCerraduraPostiva(struct AFNNodo *tmp, int noEstados);
struct AFNNodo *crearUnion(struct AFNNodo *tmp, struct AFNNodo *tmp2, int noEstados);
void recorrerAutomata(struct AFNNodo *tmp, FILE *pfile);
void generarArchivo(struct AFNNodo *tmp);