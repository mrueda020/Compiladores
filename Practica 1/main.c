#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AFNNodo.c"
//Pila para convertir a postfijo
struct nodo
{
	char op;
	struct nodo *siguiente;
};

//Pila para evaluar la expresion postfijp
struct Pila
{
	struct AFNNodo *nodo;
	struct Pila *siguiente;
};

//Metodos para la primera pila
void agregar(char op, struct nodo **superior);
void eliminarUltimo(struct nodo **superior);
char ultimo(struct nodo *superior);

//Metodos para l segunda pila
void agregar1(struct AFNNodo *op, struct Pila **superior);
void eliminarUltimo1(struct Pila **superior);
struct AFNNodo *ultimo1(struct Pila *superior);

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
char *infixToPosfix(char *exp)
{
	//Se crrea una pila
	struct nodo *superior = NULL;
	int loncad = strlen(exp);
	char pos[loncad];
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

int main(int argc, char *argv[])
{
	char *exp;
	exp = malloc(strlen(argv[1]));
	strcpy(exp, argv[1]);
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
	//printf("%s\n",pos);
	//printf("%ld\n",strlen(pos));
	char *posfix = infixToPosfix(pos);
	//printf("%s\n",posfix);

	loncad = strlen(posfix);
	struct Pila *pila = NULL;
	int noEstados = 0;
	for (i = 0; i < loncad; i++)
	{
		if (esOperando(posfix[i]))
		{
			struct AFNNodo *tmp = CrearAutomata(posfix[i], noEstados);
			agregar1(tmp, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '.')
		{
			struct AFNNodo *tmp = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp1 = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp2 = concatenar(tmp1, tmp);
			//noEstados +=2;
			agregar1(tmp2, &pila);
		}
		else if (posfix[i] == '+')
		{
			struct AFNNodo *tmp = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp1 = crearCerraduraPostiva(tmp, noEstados);
			agregar1(tmp1, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '*')
		{
			struct AFNNodo *tmp = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp1 = crearCerraduraKleen(tmp, noEstados);
			agregar1(tmp1, &pila);
			noEstados += 2;
		}
		else if (posfix[i] == '|')
		{
			struct AFNNodo *tmp = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp1 = ultimo1(pila);
			eliminarUltimo1(&pila);
			struct AFNNodo *tmp2 = crearUnion(tmp1, tmp, noEstados);
			noEstados += 2;
			agregar1(tmp2, &pila);
		}
	}
	struct AFNNodo *automata = ultimo1(pila);
	generarArchivo(automata);

	return 0;
}

char ultimo(struct nodo *superior)
{
	if (superior == NULL)
		return -1;
	return superior->op;
}

void agregar(char op, struct nodo **superior)
{

	struct nodo *nuevoNodo = malloc(sizeof(struct nodo));
	nuevoNodo->op = op;
	nuevoNodo->siguiente = *superior;
	*superior = nuevoNodo;
}

void eliminarUltimo(struct nodo **superior)
{
	if (*superior != NULL)
	{
		struct nodo *temporal = *superior;
		*superior = (*superior)->siguiente;
		free(temporal);
	}
}

struct AFNNodo *ultimo1(struct Pila *superior)
{
	if (superior == NULL)
		return NULL;
	return superior->nodo;
}

void agregar1(struct AFNNodo *op, struct Pila **superior)
{

	struct Pila *nuevoNodo = malloc(sizeof(struct Pila));
	nuevoNodo->nodo = op;
	nuevoNodo->siguiente = *superior;
	*superior = nuevoNodo;
}

void eliminarUltimo1(struct Pila **superior)
{
	if (*superior != NULL)
	{
		struct Pila *temporal = *superior;
		*superior = (*superior)->siguiente;
		free(temporal);
	}
}
