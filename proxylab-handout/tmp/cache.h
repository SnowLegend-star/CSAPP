#include "csapp.h"
#include<time.h>
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_ITEM_SUM    10

typedef struct{
    char url[MAXLINE];  //url一样就说明是请求同一个item
    int item_Size;      //单条item不得超过最大值102400
    time_t tiemTamp;
    char item_Content[MAX_OBJECT_SIZE];
}cache_Item;

typedef struct {
    cache_Item cache_Set[MAX_ITEM_SUM];
    int cache_Item_Using;   //0 1 ... 9
}cache_t;

//write to cache
//read cache
//search cache

void cache_init(cache_t* cache);
void cache_insert(cache_t* cache, cache_Item* item);
void cache_remove(cache_t* cache, cache_Item* item);