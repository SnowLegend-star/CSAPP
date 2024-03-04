// cache.c

#include "cache.h"
#include "csapp.h"

void cache_init(cache_t *cache) {
    // cache=(cache_t*)Malloc(sizeof(cache_t));
    cache->cache_Item_Using = 0;
}

void cache_insert(cache_t *cache, cache_Item *item) {
    int n = cache->cache_Item_Using;
    if (n < MAX_ITEM_SUM) {
        strcpy(cache->cache_Set[n].item_Content, item->item_Content);
        cache->cache_Set[n].tiemTamp = item->tiemTamp;
        cache->cache_Set[n].item_Size = item->item_Size;
        strcpy(cache->cache_Set[n].url, item->url);
        cache->cache_Item_Using++;
    } else {
        cache_remove(cache, item);
    }
}

void cache_remove(cache_t *cache, cache_Item *item) {
    time_t time_Min = cache->cache_Set[0].tiemTamp;
    int min;
    for (int i = 1; i < MAX_ITEM_SUM; i++) {
        if (time_Min > cache->cache_Set[i].tiemTamp) {
            time_Min = cache->cache_Set[i].tiemTamp;
            min = i;
        }
    }

    strcpy(cache->cache_Set[min].item_Content, item->item_Content);
    cache->cache_Set[min].tiemTamp = item->tiemTamp;
    cache->cache_Set[min].item_Size = item->item_Size;
    strcpy(cache->cache_Set[min].url, item->url);
}
