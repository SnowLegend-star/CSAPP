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

//define function
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp,size_t asize);

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//basic constants and macros
#define WSIZE   4                         //字的大小和首部/脚部的大小
#define DSIZE   8                         //双字的大小
#define CHUNKSIZE (1<<12)              //扩展堆时的默认大小

#define MAX(x,y)    ((x)>(y)?(x):(y))

//pack a size and allocated bit into a word in header/footer 很绕啊
#define PACK(size ,alloc)   ((size)|(alloc))

//read and write a word at address p
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p,val)  (*(unsigned int *)(p)=(val))

//read the size and allocated filds from address p
#define GET_SIZE(p)     (GET(p)&~0x7)
#define GET_ALLOC(p)    (GET(p)&0x1 )

//given block ptr bp,compute address of its header and footer
#define HDRP(bp)    ((char *)(bp)-WSIZE)
#define FTRP(bp)    ((char*)(bp)+GET_SIZE(HDRP(bp))-DSIZE)  //没把bp定位到头部坏大事

//given block ptr bp,computer address of next and previous blocks
#define NEXT_BLKP(bp)   ((char*)(bp)+GET_SIZE( (char*)(bp) - WSIZE) )
#define PREV_BLKP(bp)   ((char*)(bp)-GET_SIZE( (char*)(bp) - DSIZE) )

static void *heap_listp;

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

    for(bp=heap_listp;GET_SIZE(HDRP(bp))>0;bp=NEXT_BLKP(bp)){
        if(!GET_ALLOC(HDRP(bp))&&(asize<=GET_SIZE(HDRP(bp))))   //这个块没被分配且容量合适
            return bp;
    }

    return NULL;
}

static void place(void *bp,size_t asize){
    size_t cur_size=GET_SIZE(HDRP(bp));

    if((cur_size-asize)>=(2*WSIZE)){    //给16字节的头部、序言、结尾块腾位置
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
        bp=NEXT_BLKP(bp);               //移动到下一个块,就是分割完剩下的部分
        PUT(HDRP(bp),PACK(cur_size-asize,0));
        PUT(FTRP(bp),PACK(cur_size-asize,0));
    }
    else{                               //能用到place说明cur_size-asize>0  直接把这个给分配掉
        PUT(HDRP(bp),PACK(cur_size,1)); //因为剩下的空间也就0、1这两种，但是一个可用块最小为2WSIZE
        PUT(FTRP(bp),PACK(cur_size,1));
    }
}

static void *coalesce(void *bp){
    size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size=GET_SIZE(HDRP(bp));

    if(prev_alloc&&next_alloc){
        return bp;
    }

    else if(prev_alloc&&!next_alloc){
        size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp),PACK(size,0));
        PUT(FTRP(bp),PACK(size,0));
    }

    else if(!prev_alloc&&next_alloc){
        size+=GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp=PREV_BLKP(bp);
    }

    else{
        size+=GET_SIZE(HDRP(PREV_BLKP(bp)))+GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
    }

    return bp;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    //开始创建初始的空堆,大小为4字
    if((heap_listp=mem_sbrk(4*WSIZE))==(void *) -1)
        return -1;
    // return 0;   //牛魔的，怎么这里有个return

    PUT(heap_listp,0);                              //alignment padding
    PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));        //prologue header
    PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));        //prologue footer
    PUT(heap_listp+(3*WSIZE),PACK(0,1));            //epologue block
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
    void *old_ptr=ptr,*next_ptr,*new_ptr;
    size_t asize;
    size_t extendsize;
    size_t blocksize_Cur,blocksize_Next,blocksize_Sum;    //当前块的大小，下一个块的大小

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

        place(ptr,asize);
        return ptr;
    }
    else{                               //当前块的大小<要求分配的空间大小

        next_ptr=NEXT_BLKP(ptr);
        blocksize_Next=GET_SIZE(HDRP(next_ptr));
        blocksize_Sum=blocksize_Cur+blocksize_Next;
        if(GET_ALLOC(HDRP(next_ptr))==0&&blocksize_Sum>=asize){  //当前块大小+下一块大小>asize
            PUT(HDRP(ptr),PACK(blocksize_Sum,0));           //把当前块和下一块合并
            place(ptr,asize);
            return ptr;
        }
        else{
            new_ptr=find_fit(asize);
            if(new_ptr==NULL){      //如果当前链表找不到合适的块，则申请额外的空间
                extendsize=MAX(CHUNKSIZE,asize);
                if((new_ptr=extend_heap(extendsize/WSIZE))==NULL)
                    return NULL;
            }
            place(new_ptr,asize);
            memcpy(new_ptr,old_ptr,blocksize_Cur);
            mm_free(old_ptr);
            return new_ptr;
        }
    }
}
