// cache.h

#ifndef CACHE_H
#define CACHE_H

#include "csapp.h"
#include <time.h>

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_ITEM_SUM 10

typedef struct {
    char url[MAXLINE];
    int item_Size;
    time_t tiemTamp;
    char item_Content[MAX_OBJECT_SIZE];
} cache_Item;

typedef struct {
    cache_Item cache_Set[MAX_ITEM_SUM];
    int cache_Item_Using;
} cache_t;

void cache_init(cache_t *cache);
void cache_insert(cache_t *cache, cache_Item *item);
void cache_remove(cache_t *cache, cache_Item *item);

#endif /* CACHE_H */

