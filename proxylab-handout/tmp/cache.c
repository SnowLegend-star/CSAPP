#include "csapp.h"
#include "cache.h"


void cache_init(cache_t* cache){
    cache->cache_Item_Using=0;
}

void cache_insert(cache_t* cache,cache_Item* item){
    int n=cache->cache_Item_Using;
    if(n<9){    //剩下的缓存空间够
        strcpy(cache->cache_Set[n].item_Content,item->item_Content);
        n++;
        cache->cache_Set->tiemTamp=item->tiemTamp;
        cache->cache_Set->item_Size=item->item_Size;
        strcpy(cache->cache_Set->url,item->url);
    }
    else{       //缓存空间满了，准备驱逐一条item
        cache_remove(cache, item);
    }
}

void cache_remove(cache_t* cache, cache_Item* item){
    time_t time_Min=cache->cache_Set[0].tiemTamp;
    int min;
    for(int i=1;i<MAX_ITEM_SUM;i++){
        if(time_Min>cache->cache_Set[i].tiemTamp){
            time_Min=cache->cache_Set[i].tiemTamp;
            min=i;
        }
    }
    //驱逐最久没使用的item(LRU) 即时间最小
    strcpy(cache->cache_Set[min].item_Content,item->item_Content);
    cache->cache_Set->tiemTamp=item->tiemTamp;
    cache->cache_Set->item_Size=item->item_Size;
    strcpy(cache->cache_Set->url,item->url);
}