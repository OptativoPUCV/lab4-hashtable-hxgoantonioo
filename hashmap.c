#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
    if(map==NULL || key==NULL) return;
    if(map->size==map->capacity)
    {
        enlarge(map);
    }
    long posicion = hash(key,map->capacity);
    while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL)
      {
        posicion++;
        posicion%=map->capacity;
      }
    map->buckets[posicion]=createPair(key,value);
    map->size++;
}

void enlarge(HashMap * map) 
{
    if(map == NULL) return;

    long new_capacity = map->capacity * 2;

    Pair **new_buckets = (Pair **)calloc(new_capacity, sizeof(Pair *));

    for (long i = 0; i < map->capacity; i++) 
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) 
        {
            long index = hash(map->buckets[i]->key, new_capacity);
            while (new_buckets[index] != NULL && new_buckets[index]->key != NULL) 
            {
                index = (index + 1) % new_capacity;
            }
            new_buckets[index] = map->buckets[i];
        }
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}


HashMap * createMap(long capacity) 
{
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) 
{
    if(map==NULL || key==NULL) return;
    long posicion = hash(key,map->capacity);
    while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL)
      {
        if(is_equal(map->buckets[posicion]->key,key))
        {
          map->buckets[posicion]->key=NULL;
          map->size--;
          return;
        }
        posicion++;
        posicion%=map->capacity;
      }
}

Pair * searchMap(HashMap * map,  char * key) 
{
    if(map==NULL || key==NULL) return NULL;
    long posicion = hash(key,map->capacity);
    while(map->buckets[posicion]!=NULL)
      {
        if(is_equal(map->buckets[posicion]->key,key))
        {
          map->current=posicion;
          return map->buckets[posicion];
        }
        posicion++;
        posicion%=map->capacity;
      }
    return NULL;
}

Pair * firstMap(HashMap * map) 
{
    if(map==NULL) return NULL;
    while(map->current!=-1)
      {
        if(map->buckets[map->current]!=NULL && map->buckets[map->current]->key!=NULL)
        {
          return map->buckets[map->current];
        }
        map->current++;
        map->current%=map->capacity;
      }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL) return NULL;

    if (map->current == -1 || map->current == map->capacity - 1) {
        map->current = -1; // Reset the current index to -1 when at the end
        return NULL;
    }

    map->current++;
    while (map->current < map->capacity) {
        if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL) {
            return map->buckets[map->current];
        }
        map->current++;
    }

    map->current = -1; // Reset current index to -1 when reaching the end
    return NULL;
}