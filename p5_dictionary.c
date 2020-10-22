// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h> //strcpy
#include <strings.h> //strcasecmp
#include <stdlib.h>
#include <stdio.h> //fopen
#include <ctype.h> //tolower

#include "dictionary.h"
//primes for hash function found in stackoverflow
//https://stackoverflow.com/questions/8317508/hash-function-for-a-string/8317622#8317622
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define FIRSTH 37 /* also prime */

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; //no es un string, es un array de char
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100003; //jugar con esto

// Hash table
node * table[N];

// Number of words in dictionary
int wordsindict;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    //should be case insensitive, use strcasecmp
    node * cursor = NULL;
    //pass the word to hash()
    int bucket = hash(word);
    cursor = table[bucket];
    //go to array[hashcode] and create a cursor pointing at the first element
    //traverse the linked list and check if the word is there use strcasecmp
    //strcasecmp compare to pointers and return 0 if the arguments are equal
    while (cursor != NULL) //o cursor!= NULL??
    {
        if (strcasecmp(cursor -> word, word) != 0)
        {
            cursor = cursor -> next;
        }
        else
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char * word) // ver si cambiar la hash function me afecta mucho
{
    // TODO
    //input word, with alphabetical o apostrophe
    //output, number between 0 and N-1 (USE % N to wrap around)
    unsigned h = FIRSTH;
    while (*word)
    {
        h = (h * A) ^ (tolower(word[0]) * B);
        word++;
    }
    return h % N;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE * dict = fopen(dictionary, "r"); //dictionary esta definido al tope de speller como un string, ahora le puse small
    if (!dict)
    {
        return false;
    }
    //crear buffer para que lo salga de fscanf (despues paso del buffer al nodo)

    char * bword = malloc(sizeof(char) * (LENGTH + 1)); //esto es un buffer, no necesito usarlo despues de haberlo usado para copiar node
    //el tercer parametro de fscanf tiene que ser una locacion en memoria
    //esto es algo a liberar, para que la memoria que usó este disponible para otras cosas. Si no voy a estar ocupando memoria en un buffer
    //char bword[LENGTH+1]; //si lo defino estático con cada nueva asignación creo que ocupo un espacio de memoria distinto.
    node * newnode = NULL;
    node * head = NULL; //este es un pointer que voy a usar para apuntar al primer nodo de la lista en cierto espacio de array
    //node * temp;
    int hashcode;

    //en todos los elementos de table estoy poniendo una cabecera apuntando a nada, lista para que venga un node y apuntarle despues
    //es lo mismo al node* list que usaba antes
    for (int i = 0; i < N; i++)
    {
        table[i] = head;
    }

    while (fscanf(dict, "%s", bword) != EOF)
    {
        newnode = malloc(sizeof(node));

        if (newnode == NULL)
        {
            return false;
        }

        strcpy(newnode -> word, bword);//(destination, source) ambos deben ser pointers
        newnode -> next = NULL;
        hashcode = hash(newnode -> word);
        //como inserto en el array?
        //cambia si es el primer nodo en un bucket o no?
        //head es table[hashcode]
        if (table[hashcode] == NULL)
        {
            table[hashcode] = newnode;
        }
        //quisiera insertar al principio de la lista para no gastar en tiempo en ponerlo al final
        //tengo que hacer que el nuevo nodo apunte al antiguo primer nodo (copiando la dirección a la que apunta head/list)
        //y que list/head pase a apuntar al nuevo nodo
        else
        {
            newnode -> next = table[hashcode];
            table[hashcode] = newnode;
        }

        wordsindict++;
        //free(bword); si lo pongo acá, desasigno memoria, y como la asignación esta fuera del lazo, creo fscanf me va a dar error
        //creo que free(bword) va afuera del lazo
        //free(newnode) //newnode no es un nodo, es una direccion a un nodo pero free deasigna memoria, probablemente quiero esto en unload
    }
    //printf("Se alcanza final de archivo. \n");
    fclose(dict);
    free(bword);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (wordsindict > 0) //si el diccionario fue cargado, esto tuvo que haber cambiado, si o si.
    {
        return wordsindict;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    //necesito dos cursores para ir borrando los nodos en cada elemento de la table[N]
    node * cursor = NULL;
    node * temp = NULL;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i]; //make cursor point the same thing table[i] (head) is pointing
        temp = cursor; //make temp point the same thing cursor is pointing to.
        while (cursor != NULL)
        {
            cursor = cursor -> next; //move to the next node
            free(temp);
            temp = cursor; //catch up temp with cursor
        }
    }
    return true;
}
