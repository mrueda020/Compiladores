#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AFNNodo.h"

//Pila para convertir a postfijo
struct Nodo
{
	char op;
	struct Nodo *siguiente;
};

//Pila para evaluar la expresion postfijo
struct PilaAutomata
{
	struct AFNNodo *nodo;
	struct PilaAutomata *siguiente;
};

//Metodos para la primera pila
char ultimo(struct Nodo *superior)
{
	if (superior == NULL)
		return -1;
	return superior->op;
}

void agregar(char op, struct Nodo **superior)
{

	struct Nodo *nuevoNodo = malloc(sizeof(struct Nodo));
	nuevoNodo->op = op;
	nuevoNodo->siguiente = *superior;
	*superior = nuevoNodo;
}

void eliminarUltimo(struct Nodo **superior)
{
	if (*superior != NULL)
	{
		struct Nodo *temporal = *superior;
		*superior = (*superior)->siguiente;
		free(temporal);
	}
}

//Metodos para la segunda pila
struct AFNNodo *ultimoAutomata(struct PilaAutomata *superior)
{
	if (superior == NULL)
		return NULL;
	return superior->nodo;
}

void agregarAutomata(struct AFNNodo *op, struct PilaAutomata **superior)
{

	struct PilaAutomata *nuevoNodo = malloc(sizeof(struct PilaAutomata));
	nuevoNodo->nodo = op;
	nuevoNodo->siguiente = *superior;
	*superior = nuevoNodo;
}

void eliminarAutomata(struct PilaAutomata **superior)
{
	if (*superior != NULL)
	{
		struct PilaAutomata *temporal = *superior;
		*superior = (*superior)->siguiente;
		free(temporal);
	}
}

//Checa si es un opernado
int esOperando(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

//Regresa la presedencia de la operaciones
int prec(char c)
{
	if (c == '+' || c == '*')
		return 3;
	else if (c == '.')
		return 2;
	else if (c == '|')
		return 1;
	else
		return -1;
}

//Convierte de infijo a postfijo
char *infijoAPosfijo(char *exp)
{
	//Se crrea una pila
	struct Nodo *superior = NULL;
	int loncad = strlen(exp);
	int i, k;
	agregar('#', &superior);
	//Recorremos la expresion
	for (i = 0, k = -1; i < loncad; i++)
	{
		//Si es un operando se guarda en la expresion postfija
		if (esOperando(exp[i]))
			exp[++k] = exp[i];
		//Si es un ( se guarda en la pila
		else if (exp[i] == '(')
			agregar('(', &superior);
		// Se sacan todos los elementos de la pila hasta que se encuentre el (
		else if (exp[i] == ')')
		{
			while (ultimo(superior) != '#' && ultimo(superior) != '(')
			{
				char c = ultimo(superior);
				eliminarUltimo(&superior);
				exp[++k] = c;
			}
			if (ultimo(superior) != '#' && ultimo(superior) != '(')
			{

				printf("Expresion no valida");
			}

			else
			{
				eliminarUltimo(&superior);
			}
		}
		//Un operador fue encontrado en la pila
		else
		{
			while (ultimo(superior) != '#' && prec(exp[i]) <= prec(ultimo(superior)))
			{
				char c = ultimo(superior);
				eliminarUltimo(&superior);
				exp[++k] = c;
			}

			agregar(exp[i], &superior);
		}
	}

	while (ultimo(superior) != '#')
	{
		char c = ultimo(superior);
		eliminarUltimo(&superior);
		exp[++k] = c;
	}
	exp[++k] = '\0';
	return exp;
}

char *agregarConcatenacion(char *exp)
{
	int loncad = strlen(exp);
	char *pos = malloc(100);

	int i;
	int k = -1;
	//Agregamos la concatenacion a la expresion regular
	for (i = 0; i < loncad; i++)
	{
		//Si es el primer elemento se copia tal como esta
		if (i == 0)
		{
			//strcat(pos,&exp[i]);
			pos[++k] = exp[i];
		}

		else
		{
			// si es alguno de esos elementos entoncs puede concatenarse
			if (exp[i] == '(' || exp[i] == ')')
			{

				if (exp[i] == '(' && esOperando(exp[i - 1]))
				{
					char c = '.';
					pos[++k] = c;
					pos[++k] = exp[i];
				}
				else if (exp[i] == '(' && exp[i - 1] == ')')
				{
					char c = '.';
					pos[++k] = c;
					pos[++k] = exp[i];
				}

				else if (exp[i] == '(' && exp[i - 1] == '*')
				{
					char c = '.';
					pos[++k] = c;
					pos[++k] = exp[i];
				}
				else if (exp[i] == '(' && exp[i - 1] == '+')
				{
					char c = '.';
					pos[++k] = c;
					pos[++k] = exp[i];
				}

				else
				{
					pos[++k] = exp[i];
				}
			}
			//Se concatena si se encontro un operador
			else if (esOperando(exp[i]))
			{
				if (esOperando(exp[i - 1]) || exp[i - 1] == ')' || exp[i - 1] == '+' || exp[i - 1] == '*')
				{
					char c = '.';
					pos[++k] = c;
					pos[++k] = exp[i];
				}

				else
				{
					//strcat(pos,&exp[i]);
					pos[++k] = exp[i];
				}
			}
			//No se conctean si se encuentra el operador |
			else
			{

				pos[++k] = exp[i];
			}
		}
	}
	pos[++k] = '\0';
	return pos;
}

struct AFNNodo *evaluarAutomata(char *posfix)
{
	int loncad = strlen(posfix);
	struct PilaAutomata *pila = NULL;
	int i, noEstados = 0;
	for (i = 0; i < loncad; i++)
	{
		if (esOperando(posfix[i]))
		{
			struct AFNNodo *tmp = crearAutomata(posfix[i], noEstados);
			agregarAutomata(tmp, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '.')
		{
			struct AFNNodo *tmp = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp1 = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp2 = concatenar(tmp1, tmp);
			//noEstados +=2;
			agregarAutomata(tmp2, &pila);
		}
		else if (posfix[i] == '+')
		{
			struct AFNNodo *tmp = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp1 = crearCerraduraPostiva(tmp, noEstados);
			agregarAutomata(tmp1, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '*')
		{
			struct AFNNodo *tmp = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp1 = crearCerraduraKleen(tmp, noEstados);
			agregarAutomata(tmp1, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '|')
		{
			struct AFNNodo *tmp = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp1 = ultimoAutomata(pila);
			eliminarAutomata(&pila);
			struct AFNNodo *tmp2 = crearUnion(tmp1, tmp, noEstados);
			noEstados += 2;
			agregarAutomata(tmp2, &pila);
		}
	}
	struct AFNNodo *automata = ultimoAutomata(pila);
	return automata;
}

int main(int argc, char *argv[])
{

	if (argc <= 1)
	{
		printf("Ingresa una expresion regular\n");
		return 0;
	}

	char *exp = malloc(strlen(argv[1]));
	strcpy(exp, argv[1]);
	char *exp_cot = agregarConcatenacion(exp);
	char *posfix = infijoAPosfijo(exp_cot);
	struct AFNNodo *automata = evaluarAutomata(posfix);
	generarArchivo(automata);

	return 0;
}
