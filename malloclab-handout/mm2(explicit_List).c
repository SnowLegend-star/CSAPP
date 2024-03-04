/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Glory",
    /* First member's full name */
    "Michael",
    /* First member's email address */
    "1716438226@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8                     //对齐8个字节(2个字)

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) //头部、脚部、两指针、8字节数据

//basic constants and macros
#define WSIZE   4                         //字的大小和首部/脚部的大小
#define DSIZE   8                         //双字的大小
#define CHUNKSIZE   (1<<12)              //扩展堆时的默认大小
#define MINBLOCK    (DSIZE+2*WSIZE+2*WSIZE)

#define MAX(x,y)    ((x)>(y)?(x):(y))

//pack a size and allocated bit into a word in header/footer 很绕啊
#define PACK(size ,alloc)   ((size)|(alloc))

//read and write a word at address p
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p,val)  (*(unsigned int *)(p)=(val))
#define GETADDR(p) (*(unsigned int **)(p)) //读地址p处的一个指针
#define PUTADDR(p,addr) (*(unsigned int **)(p)=(unsigned int *)(addr)) //在地址p处写的指针

//read the size and allocated filds from address p
#define GET_SIZE(p)     (GET(p)&~0x7)
#define GET_ALLOC(p)    (GET(p)&0x1 )

//given block ptr bp,compute address of its header and footer
#define HDRP(bp)    ((char *)(bp)-WSIZE)
#define FTRP(bp)    ((char*)(bp)+GET_SIZE(HDRP(bp))-DSIZE)  //没把bp定位到头部坏大事

//given block ptr bp,computer address of next and previous blocks
#define NEXT_BLKP(bp)   ((char*)(bp)+GET_SIZE( (char*)(bp) - WSIZE) )
#define PREV_BLKP(bp)   ((char*)(bp)-GET_SIZE( (char*)(bp) - DSIZE) )

//链表特有的指针
#define PRED_POINTER(bp)    (bp)    //指向父指针的指针
#define SUCC_POINTER(bp)    ((char*)(bp)+WSIZE) //指向后继的指针

static void *heap_listp;
static void *head_free;

//define function
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp,size_t asize);

//链表操作
static void insert_freelist(void *bp);
static void remove_freelist(void *bp);
static void place_freelist(void *bp);

static void *extend_heap(size_t words){
    char *bp;
    size_t size;

    //分配偶数字或者进行填充
    size=(words%2)?(words+1)*WSIZE:words*WSIZE;
    if((long)(bp=mem_sbrk(size))==-1)
        return NULL;

    //初始化头部/脚部块和结束块
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1)); //有点没看懂

    //如果前一个块空闲则合并
    return coalesce(bp);
}

static void *find_fit(size_t asize){
    //h第一次适应算法
    void *bp;

    for(bp=GETADDR(head_free);bp!=NULL;bp=GETADDR(SUCC_POINTER(bp))){ //遍历空闲链表
        if((asize<=GET_SIZE(HDRP(bp))))   //这个块没被分配且容量合适
            return bp;
    }

    return NULL;
}

static void place(void *bp,size_t asize){
    size_t cur_size=GET_SIZE(HDRP(bp));
    void *next_bp;
    if((cur_size-asize)>=(MINBLOCK)){    //最小块的大小为24B，这里包括了有效载荷的部分
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
        next_bp=NEXT_BLKP(bp);               //移动到下一个块,就是分割完剩下的部分
        PUT(HDRP(next_bp),PACK(cur_size-asize,0));
        PUT(FTRP(next_bp),PACK(cur_size-asize,0));
        place_freelist(bp);
    }
    else{                               //能用到place说明cur_size-asize>0  直接把这个给分配掉
        PUT(HDRP(bp),PACK(cur_size,1)); //因为剩下的空间也就0、1这两种，但是一个可用块最小为2WSIZE
        PUT(FTRP(bp),PACK(cur_size,1));
        remove_freelist(bp);
    }
}

static void *coalesce(void *bp){//基本思路没变，加入对空闲链表的操作
    size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size=GET_SIZE(HDRP(bp));
    char *pre_block,*next_block;

    if(prev_alloc&&next_alloc){
        insert_freelist(bp);
        return bp;
    }

    else if(prev_alloc&&!next_alloc){   //合并下一块
        size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
        next_block=NEXT_BLKP(bp);   
        remove_freelist(next_block);
        insert_freelist(bp);
    }

    else if(!prev_alloc&&next_alloc){   //合并前一块
        size+=GET_SIZE(HDRP(PREV_BLKP(bp)));
        bp=PREV_BLKP(bp);
        remove_freelist(bp);
        insert_freelist(bp);
    }

    else{   //前后块都合并
        size+=GET_SIZE(HDRP(PREV_BLKP(bp)))+GET_SIZE(FTRP(NEXT_BLKP(bp)));
        pre_block=PREV_BLKP(bp);
        next_block=NEXT_BLKP(bp);
        bp=PREV_BLKP(bp);
        remove_freelist(pre_block);
        remove_freelist(next_block);
        insert_freelist(bp);
    }
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    return bp;
}


//使用头插法，将空闲块插入空闲链表中
static void insert_freelist(void *bp){  //LIFO，先进后出
    if(GETADDR(head_free)==NULL){
        PUTADDR(SUCC_POINTER(bp),NULL);
        PUTADDR(PRED_POINTER(bp),head_free);
        PUTADDR(head_free,bp);
    }
    else{
        void *tmp;
        tmp=GETADDR(head_free);
        PUTADDR(SUCC_POINTER(bp),tmp);
        PUTADDR(PRED_POINTER(bp),head_free);
        PUTADDR(head_free,bp);
        PUTADDR(PRED_POINTER(tmp),bp);
        tmp=NULL;
    }
}


//将bp指向的空闲块从空闲链表中移除
static void remove_freelist(void *bp){
    void *pred_ptr,*succ_ptr;
    pred_ptr=GETADDR(PRED_POINTER(bp));
    succ_ptr=GETADDR(SUCC_POINTER(bp));
    //处理前驱节点
    if(pred_ptr==head_free){
        PUTADDR(head_free,succ_ptr);
    }
    else{
        PUTADDR(SUCC_POINTER(pred_ptr),succ_ptr);
    }
    //处理后继节点
    if(succ_ptr!=NULL){
        PUTADDR(PRED_POINTER(succ_ptr),pred_ptr);
    }
}

//对空闲链表中的空闲块进行分割
static void place_freelist(void *bp){
    void *pred_ptr,*succ_ptr,*next_bp;
    //储存前后结点的地址
    pred_ptr=GETADDR(PRED_POINTER(bp));
    succ_ptr=GETADDR(SUCC_POINTER(bp));
    next_bp=NEXT_BLKP(bp);
    //处理新的bp，进行前后链接
    PUTADDR(PRED_POINTER(next_bp),pred_ptr);
    PUTADDR(SUCC_POINTER(next_bp),succ_ptr);
    //处理前序节点，针对head_free是前序节点的特殊处理
    if(pred_ptr==head_free){
        PUTADDR(head_free,next_bp);
    }
    else{
        PUTADDR(SUCC_POINTER(pred_ptr),next_bp);
    }
    //处理后序节点
    if(succ_ptr!=NULL){
        PUTADDR(PRED_POINTER(succ_ptr),next_bp);
    }
}
/* 
 * mm_init - initialize the malloc package.
 */
//设立序言块、结尾块，以及序言块前的对齐块（4B），总共需要4个4B的空间
int mm_init(void)
{
    //开始创建初始的空堆,大小为4字
    if((heap_listp=mem_sbrk(4*WSIZE))==(void *) -1)
        return -1;

    PUTADDR(heap_listp,NULL);                       //堆起始位置的对齐块，是bp对齐8字节
    // PUT(heap_listp,0);                              //alignment padding
    PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));        //prologue header
    PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));        //prologue footer
    PUT(heap_listp+(3*WSIZE),PACK(0,1));            //epologue block 存疑
    head_free=heap_listp;
    PUTADDR(head_free,NULL);
    heap_listp+=(2*WSIZE);
    
    //增加堆的大小
    if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
        return -1;
    return 0;
}



/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

    size_t asize;   //adjusted block size
    size_t extendsize;  //如果大小超过堆的大小应该增加的总数
    char* bp;

    if(size==0)
        return NULL;

    //双字对齐
    if(size<=DSIZE)
        asize=2*DSIZE;
    else
        asize=DSIZE*((size+(DSIZE)+(DSIZE-1))/DSIZE);   //加1向下舍入

    //从空闲链表里找合适的块进行分配
    if((bp=find_fit(asize))!=NULL){
        place(bp,asize);
        return bp;
    }

    //如果没有合适的空闲块，堆请求更大的空间
    extendsize=MAX(asize,CHUNKSIZE);
    if((bp=extend_heap(extendsize/WSIZE))==NULL)
        return NULL;

    place(bp,asize);
    return bp;

}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size=GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr),PACK(size,0));
    PUT(FTRP(ptr),PACK(size,0));
    coalesce(ptr);
}


/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *old_ptr=ptr,*new_ptr;
    size_t asize;
    size_t extendsize;
    size_t blocksize_Cur;    //当前块的大小，下一个块的大小

    if(ptr==NULL&&size!=0)
        return mm_malloc(size);
    if(ptr!=NULL&&size==0){
        mm_free(ptr); 
        return NULL;       
    }

    //接下来就是指针不为空，且分配大小非0的正常情况了
    //双字对齐
    if(size<=DSIZE)
        asize=2*DSIZE;
    else
        asize=DSIZE*((size+(DSIZE)+(DSIZE-1))/DSIZE);

    // blocksize_Cur=GET_SIZE(ptr);    //ptr得定位到头部☆☆☆
    blocksize_Cur=GET_SIZE(HDRP(ptr));
    if(asize==blocksize_Cur){

        return ptr;        
    }
    else if(asize<blocksize_Cur){       //当前块的大小>要求分配的空间大小
        if(blocksize_Cur-asize>=MINBLOCK)
            place(ptr,asize);
        return ptr;
    }
    else{                               //当前块的大小<要求分配的空间大
            new_ptr=find_fit(asize);
            if(new_ptr==NULL){      //如果当前链表找不到合适的块，则申请额外的空间
                extendsize=MAX(CHUNKSIZE,asize);
                if((new_ptr=extend_heap(extendsize/WSIZE))==NULL)
                    return NULL;
            }
            place(new_ptr,asize);
            memcpy(new_ptr,old_ptr,blocksize_Cur-2*WSIZE);
            mm_free(old_ptr);
            return new_ptr;
    }
}

