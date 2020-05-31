#include "AFNNodo.h"
/*Recorremos el automata de manera recursiva para generar el archivo gv*/
void recorrerAutomata(struct AFNNodo *tmp, FILE *pfile)
{
	if (tmp == NULL)
	{
		return;
	}
	else
	{ /*Si el estado tienes dos transciciones estas se escriben en el archivo*/
		if (tmp->sig2)
		{
			//Condicion de parada
			tmp->fueVisitado = true;
			if (tmp->transicion == 'E')
			{
				fprintf(pfile, "%d -> %d[label=\"&epsilon;\"];\n", tmp->noEstado, tmp->sig1->noEstado);
			}
			else
			{
				fprintf(pfile, "%d -> %d[label=\"%c\"];\n", tmp->noEstado, tmp->sig1->noEstado, tmp->transicion);
			}

			if (tmp->transicion2 == 'E')
			{
				fprintf(pfile, "%d -> %d[label=\"&epsilon;\"];\n", tmp->noEstado, tmp->sig2->noEstado);
			}
			else
			{
				fprintf(pfile, "%d -> %d[label=\"%c\"];\n", tmp->noEstado, tmp->sig2->noEstado, tmp->transicion2);
			}
		}
		/*Si solo se tiene una transicion se escribe en el archivo*/
		else if (tmp->sig1)
		{
			if (tmp->transicion == 'E')
			{
				fprintf(pfile, "%d -> %d[label=\"&epsilon;\"];\n", tmp->noEstado, tmp->sig1->noEstado);
			}
			else
			{
				fprintf(pfile, "%d -> %d[label=\"%c\"];\n", tmp->noEstado, tmp->sig1->noEstado, tmp->transicion);
			}
			tmp->fueVisitado = true;
		}
		//Si no tiene mas transiciones entonces no se recorre mas el automata
		if (tmp->sig1)
		{
			//Si no ha sido visitado entonces se recorre el estado
			if (tmp->sig1->fueVisitado == false)
				recorrerAutomata(tmp->sig1, pfile);
		}
		if (tmp->sig2)
		{
			if (tmp->sig2->fueVisitado == false)
				recorrerAutomata(tmp->sig2, pfile);
		}
	}
}
/*Genera el archivo gv y lo ejecuta despues de haberlo creado*/
void generarArchivo(struct AFNNodo *tmp)
{
	FILE *automata;
	struct AFNNodo *aux = tmp;
	automata = fopen("automata.gv", "w");
	fprintf(automata, "digraph automata{\ninicio[shape=point];\nnode[shape=circle];\nrankdir = LR;\n");
	//Obtiene informacion del primer estado del automata
	fprintf(automata, "inicio -> %d[label=\"inicio\"];\n", aux->noEstado);
	//Obtiene informacion del ultimo estado del automata
	while (aux->sig1 != NULL)
	{
		aux = aux->sig1;
	}
	fprintf(automata, "%d[shape=doublecircle];\n", aux->noEstado);

	recorrerAutomata(tmp, automata);

	fputs("}", automata);
	fclose(automata);
	system("dot -Tpng automata.gv > salida.png");
	system("eog salida.png");
	//printf("Visitado? : %d\n",tmp->fueVisitado);
}

struct AFNNodo *CrearAutomata(char c, int noEstados)
{
	//Crea el automata basico con un estado inicial y final
	struct AFNNodo *nodo1 = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *nodo2 = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	nodo1->transicion = c;
	nodo1->ant1 = NULL;
	nodo1->ant2 = NULL;
	nodo1->esInicial = true;
	nodo1->esFinal = false;
	nodo1->esEpsilon = false;
	nodo1->fueVisitado = false;
	nodo1->noEstado = ++noEstados;
	nodo1->transicion2 = '#';
	nodo2->noEstado = ++noEstados;
	nodo2->fueVisitado = false;
	nodo2->esInicial = false;
	nodo2->esFinal = true;
	nodo2->esEpsilon = false;
	nodo2->transicion = '#';
	nodo1->transicion2 = '#';
	nodo2->ant1 = nodo1;
	nodo2->ant2 = NULL;
	nodo2->sig1 = NULL;
	nodo2->sig2 = NULL;
	nodo1->sig1 = nodo2;
	nodo1->sig2 = NULL;

	return nodo1;
}

struct AFNNodo *concatenar(struct AFNNodo *tmp, struct AFNNodo *tmp2)
{
	while (tmp->sig1 != NULL)
		tmp = tmp->sig1;
	tmp->esFinal = false;
	tmp2->esInicial = false;
	tmp2->sig1->ant1 = tmp;
	tmp->sig1 = tmp2->sig1;
	tmp->transicion = tmp2->transicion;
	tmp->transicion2 = tmp2->transicion2;
	tmp->sig2 = tmp2->sig2;
	//tmp->estado2 = tmp2->estado2;
	tmp->esFinal = false;
	while (tmp->ant1 != NULL)
		tmp = tmp->ant1;
	return tmp;
}

struct AFNNodo *crearCerraduraPostiva(struct AFNNodo *tmp, int noEstados)
{
	struct AFNNodo *inicio = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *fin = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *aux = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));

	inicio->esInicial = true;
	inicio->esEpsilon = true;
	inicio->transicion = 'E';
	inicio->transicion2 = 'E';
	inicio->fueVisitado = false;
	inicio->esFinal = false;
	inicio->noEstado = ++noEstados;
	inicio->sig1 = tmp;
	//tmp->ant1 = inicio;
	tmp->esInicial = false;
	aux = tmp;
	while (aux->sig1 != NULL)
		aux = aux->sig1;

	aux->transicion2 = 'E';
	aux->sig2 = tmp;
	tmp = aux;
	while (tmp->ant1 != NULL)
		tmp = tmp->ant1;
	tmp->ant1 = inicio;

	while (tmp->sig1 != NULL)
		tmp = tmp->sig1;

	tmp->transicion = 'E';
	tmp->esFinal = false;
	fin->esInicial = false;
	fin->esEpsilon = true;
	fin->esFinal = true;
	fin->transicion = '#';
	fin->transicion2 = '#';
	fin->noEstado = ++noEstados;
	fin->ant1 = tmp;
	fin->ant2 = inicio;
	fin->sig1 = NULL;
	fin->sig2 = NULL;
	tmp->sig1 = fin;
	inicio->sig2 = NULL;
	return inicio;
}

struct AFNNodo *crearCerraduraKleen(struct AFNNodo *tmp, int noEstados)
{
	struct AFNNodo *inicio = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *fin = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *aux = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));

	inicio->esInicial = true;
	inicio->esEpsilon = true;
	inicio->transicion = 'E';
	inicio->transicion2 = 'E';
	inicio->fueVisitado = false;
	inicio->esFinal = false;
	inicio->noEstado = ++noEstados;
	inicio->sig1 = tmp;
	//
	tmp->esInicial = false;
	aux = tmp;
	while (aux->sig1 != NULL)
		aux = aux->sig1;

	aux->transicion2 = 'E';
	aux->sig2 = tmp;
	tmp = aux;
	while (tmp->ant1 != NULL)
		tmp = tmp->ant1;
	tmp->ant1 = inicio;

	while (tmp->sig1 != NULL)
		tmp = tmp->sig1;

	tmp->transicion = 'E';
	tmp->esFinal = false;
	fin->esInicial = false;
	fin->esEpsilon = true;
	fin->esFinal = true;
	fin->transicion = '#';
	fin->transicion2 = '#';
	fin->noEstado = ++noEstados;
	fin->ant1 = tmp;
	fin->ant2 = inicio;
	fin->sig1 = NULL;
	fin->sig2 = NULL;
	tmp->sig1 = fin;
	inicio->sig2 = fin;
	return inicio;
}

struct AFNNodo *crearUnion(struct AFNNodo *tmp, struct AFNNodo *tmp2, int noEstados)
{
	struct AFNNodo *inicio = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *fin = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *aux = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));
	struct AFNNodo *aux2 = (struct AFNNodo *)malloc(sizeof(struct AFNNodo));

	tmp->esInicial = false;
	tmp->esFinal = false;

	tmp2->esFinal = false;
	tmp2->esInicial = false;

	inicio->esInicial = true;
	inicio->esEpsilon = true;
	inicio->transicion = 'E';
	inicio->transicion2 = 'E';
	inicio->fueVisitado = false;
	inicio->esFinal = false;
	inicio->noEstado = ++noEstados;

	fin->esInicial = false;
	fin->esEpsilon = true;
	fin->transicion = '#';
	fin->transicion2 = '#';
	fin->fueVisitado = false;
	fin->esFinal = true;
	fin->noEstado = ++noEstados;

	while (tmp->sig1 != NULL)
		tmp = tmp->sig1;

	tmp->sig1 = fin;
	tmp->esFinal = false;
	tmp->transicion = 'E';

	while (tmp2->sig1 != NULL)
		tmp2 = tmp2->sig1;

	tmp2->sig1 = fin;
	tmp2->esFinal = false;
	tmp2->transicion = 'E';

	fin->ant1 = tmp;
	fin->ant2 = tmp2;

	while (tmp->ant1 != NULL)
		tmp = tmp->ant1;
	tmp->ant1 = inicio;
	inicio->sig1 = tmp;

	while (tmp2->ant1 != NULL)
		tmp2 = tmp2->ant1;
	tmp2->ant1 = inicio;
	inicio->sig2 = tmp2;
	return inicio;
}
