#include "cachelab.h"
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <getopt.h>

typedef struct Cache_Line {
    int valid;
    int tag;
    int time_tamp;
    //int* line;
}cache_Line;

int hit_count = 0, miss_count = 0, eviction_count = 0; // 记录冲突不命中、缓存不命中
int verbose = 0;                                       //是否打印详细信息
char t[1000];
cache_Line** Cache;
int S,E;//E的值不会变，而且没有二义性

void print_help() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("-h         Print this help message.\n");
    printf("-v         Optional verbose flag.\n");
    printf("-s <num>   Number of set index bits.\n");
    printf("-E <num>   Number of lines per set.\n");
    printf("-b <num>   Number of block offset bits.\n");
    printf("-t <file>  Trace file.\n\n\n");
    printf("Examples:\n");
    printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(0);
}

void init_Cache(int s, int E, int b) {//直接映射型的cache可以看成是一个大二维数组，开始把它初始化
    int i, j;
    S = 1 << s;
    //int B = 1 << b;
    Cache = (cache_Line**)malloc(sizeof(cache_Line*) * S);
    for (i = 0; i < S; i++) {
        Cache[i] = (cache_Line*)malloc(sizeof(cache_Line)*E);
        //Cache[i]->line = (int*)malloc(sizeof(int) * B);
        for (j = 0; j < E; j++) {
            Cache[i][j].valid = 0;
            Cache[i][j].tag = -1;
            Cache[i][j].time_tamp = 0;
            
            //Cache[i]->line[j] = 0;            
        }
    }
}

int is_Full(int opt_s) {//判断这一组的行是不是都缓存满了
    int j;
    for (j = 0; j < E; j++) {
        if (Cache[opt_s][j].valid == 0)
            return j;//如果没满，就把第一个没满的行返回去     
    }
    return -1;//满了就返回-1
}

int is_Hit(int opt_tag, int opt_s) {//判断是否命中
    int i;
    for (i = 0; i < E; i++) {//开始遍历相应的组是否有这个缓存
        if (Cache[opt_s][i].valid && Cache[opt_s][i].tag == opt_tag) {//缓存命中
            return i;
        }
    }
    // printf("In is_Hit:E=%d",E);
    return -1;
}

int replace_Line(int opt_s) {//找出最久时间没被访问的那行
    int max_tamp=0, i;
    int index=0;            //就因为这里index没有初始化导致一直有三个地方过不了
    for (i = 0; i < E; i++) {
        if (Cache[opt_s][i].time_tamp > max_tamp) {//
            max_tamp = Cache[opt_s][i].time_tamp;
            index = i;
        }
    }
    return index;
}

void update_Line(int line_replaced, int opt_s, int opt_tag) {//更新每一行的时间tamp
    int j=0;
    Cache[opt_s][line_replaced].valid = 1;
    Cache[opt_s][line_replaced].tag = opt_tag;
    for (j = 0; j < E; j++) {//这组所有行的时间都加1，不用考虑这行的valid是否为1
        Cache[opt_s][j].time_tamp++;
    }
    Cache[opt_s][line_replaced].time_tamp = 0;
}

void update_Cache(int opt_tag, int opt_s) {
    int flag = is_Hit(opt_tag, opt_s);
    int line_replaced = -1;
    if (flag == -1) {
        miss_count++;
        if (verbose)
            printf("miss \n");
        line_replaced = is_Full(opt_s);
        if (line_replaced==-1) {//如果是冲突不命中(改组的缓存行满了)
            eviction_count++;
            printf("1111");//怎么不打印啊。。。
            if (verbose)
                printf("eviction \n");
            //如果index没有初始化，打印完eviction直接就报错了，有些奇怪。难道不是得执行完replace_Line后再报错吗？
            line_replaced = replace_Line(opt_s);
        }
        update_Line(line_replaced, opt_s, opt_tag);
    }
    else {
        hit_count++;
        if (verbose)
            printf("hit \n");
        update_Line(flag, opt_s, opt_tag);//命中了也得更新行信息
    }
}

void get_Trace(int s, int E, int b) {
    FILE* pFile;
    pFile = fopen(t, "r");
    int mask_s = ((1 << s)-1) << b; //索引位s的掩码
    if (pFile == NULL) {
        exit(1);
    }
    char option;
    unsigned address;
    int size;
    while (fscanf(pFile, "%c %x,%d", &option, &address, &size) > 0) {
        int opt_tag = address >> (s + b);
        // int opt_s = (address >> b) & ((unsigned)(-1) >> (8 * sizeof(unsigned) - s));//进行逻辑右移
        int opt_s=(unsigned)(address&mask_s)>>b;//这样其实也可以的，可恶
        switch (option)
        {
        case 'M'://可以分解为一次L和一次S
            update_Cache(opt_tag, opt_s);
            update_Cache(opt_tag, opt_s);
            break;
        case 'L':
            update_Cache(opt_tag, opt_s);
            break;
        case 'S':
            update_Cache(opt_tag, opt_s);
            break;
        default:
            break;
        }
    }
    fclose(pFile);
}

void free_Cache(){
    int i;
    for(i=0;i<S;i++)
        free(Cache[i]);
    free(Cache);
}

int main(int argc,char *argv[]) {
    char opt;
    int s,b;//是否定义全局变量E怎么也会影响
    while ((opt = getopt(argc, argv, "hvs:E:b:t:"))!=-1) {//这里t后面没：怎么不报错啊  这里是while而不是if
        switch (opt) {
        case 'h':
            print_help();
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            // E=e;
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(t, optarg);
            break;
        default:
            printf("Something wrong with your command,try './csim-ref -h' for help.");
            exit(0);
        }
    }
    init_Cache(s, E, b);
    get_Trace(s, E, b);
    printSummary(hit_count, miss_count, eviction_count);
    free_Cache();
    return 0;
}