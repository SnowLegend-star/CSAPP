// main.c

#include <stdio.h>
#include "csapp.h"
#include "cache.h"

cache_t cache;

int main() {
    cache_Item item;
    strcpy(item.item_Content, "123");
    strcpy(item.url, "url");
    item.tiemTamp = time(NULL);
    item.item_Size = 4;

    cache_init(&cache);
    cache_insert(&cache, &item);

    printf("item_Content is: %s\n", cache.cache_Set[0].item_Content);

    return 0;
}

