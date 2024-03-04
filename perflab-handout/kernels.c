/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

#define COPY(d,s) *(d)=*(s)

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Glory Of Dawn",              /* Team name */

    "Michael",     /* First member full name */
    "1716438226@qq.com",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];//好巧妙的一行
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: 分块";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i,j,k,s;
    int bsize=32;
	for (i = 0; i < dim; i += bsize)
		for (j = 0; j < dim; j += bsize) {
			for (k = 0; k < bsize; k++) 
                for(s=0;s<bsize;s++)
                    dst[RIDX(dim - (j + s) - 1, i + k, dim)] = src[RIDX(i + k, j + s, dim)];
			
		}


}

char rotate_descr_bolck[] = "rotate: 分块 + 循环展开";
void rotate_32X32_loopUnroll(int dim, pixel *src, pixel *dst) 
{
    // naive_rotate(dim, src, dst);
    int i,j,k;
    //int s;
    int bsize=32;
	for (i = 0; i < dim; i += bsize)
		for (j = 0; j < dim; j += bsize) {
			for (k = 0; k < bsize; k++) {
                pixel a0 = src[RIDX(i + k, j + 0, dim)];
                pixel a1 = src[RIDX(i + k, j + 1, dim)];
                pixel a2 = src[RIDX(i + k, j + 2, dim)];
                pixel a3 = src[RIDX(i + k, j + 3, dim)];
                pixel a4 = src[RIDX(i + k, j + 4, dim)];
                pixel a5 = src[RIDX(i + k, j + 5, dim)];
                pixel a6 = src[RIDX(i + k, j + 6, dim)];
                pixel a7 = src[RIDX(i + k, j + 7, dim)];
                pixel a8 = src[RIDX(i + k, j + 8, dim)];
                pixel a9 = src[RIDX(i + k, j + 9, dim)];
                pixel a10 = src[RIDX(i + k, j + 10, dim)];
                pixel a11 = src[RIDX(i + k, j + 11, dim)];
                pixel a12 = src[RIDX(i + k, j + 12, dim)];
                pixel a13 = src[RIDX(i + k, j + 13, dim)];
                pixel a14 = src[RIDX(i + k, j + 14, dim)];
                pixel a15 = src[RIDX(i + k, j + 15, dim)];
                pixel a16 = src[RIDX(i + k, j + 16, dim)];
                pixel a17 = src[RIDX(i + k, j + 17, dim)];
                pixel a18 = src[RIDX(i + k, j + 18, dim)];
                pixel a19 = src[RIDX(i + k, j + 19, dim)];
                pixel a20 = src[RIDX(i + k, j + 20, dim)];
                pixel a21 = src[RIDX(i + k, j + 21, dim)];
                pixel a22 = src[RIDX(i + k, j + 22, dim)];
                pixel a23 = src[RIDX(i + k, j + 23, dim)];
                pixel a24 = src[RIDX(i + k, j + 24, dim)];
                pixel a25 = src[RIDX(i + k, j + 25, dim)];
                pixel a26 = src[RIDX(i + k, j + 26, dim)];
                pixel a27 = src[RIDX(i + k, j + 27, dim)];
                pixel a28 = src[RIDX(i + k, j + 28, dim)];
                pixel a29 = src[RIDX(i + k, j + 29, dim)];
                pixel a30 = src[RIDX(i + k, j + 30, dim)];
                pixel a31 = src[RIDX(i + k, j + 31, dim)];
                dst[RIDX(dim - 1 - (j + 0), i + k, dim)] = a0;
                dst[RIDX(dim - 1 - (j + 1), i + k, dim)] = a1;
                dst[RIDX(dim - 1 - (j + 2), i + k, dim)] = a2;
                dst[RIDX(dim - 1 - (j + 3), i + k, dim)] = a3;
                dst[RIDX(dim - 1 - (j + 4), i + k, dim)] = a4;
                dst[RIDX(dim - 1 - (j + 5), i + k, dim)] = a5;
                dst[RIDX(dim - 1 - (j + 6), i + k, dim)] = a6;
                dst[RIDX(dim - 1 - (j + 7), i + k, dim)] = a7;
                dst[RIDX(dim - 1 - (j + 8), i + k, dim)] = a8;
                dst[RIDX(dim - 1 - (j + 9), i + k, dim)] = a9;
                dst[RIDX(dim - 1 - (j + 10), i + k, dim)] = a10;
                dst[RIDX(dim - 1 - (j + 11), i + k, dim)] = a11;
                dst[RIDX(dim - 1 - (j + 12), i + k, dim)] = a12;
                dst[RIDX(dim - 1 - (j + 13), i + k, dim)] = a13;
                dst[RIDX(dim - 1 - (j + 14), i + k, dim)] = a14;
                dst[RIDX(dim - 1 - (j + 15), i + k, dim)] = a15;
                dst[RIDX(dim - 1 - (j + 16), i + k, dim)] = a16;
                dst[RIDX(dim - 1 - (j + 17), i + k, dim)] = a17;
                dst[RIDX(dim - 1 - (j + 18), i + k, dim)] = a18;
                dst[RIDX(dim - 1 - (j + 19), i + k, dim)] = a19;
                dst[RIDX(dim - 1 - (j + 20), i + k, dim)] = a20;
                dst[RIDX(dim - 1 - (j + 21), i + k, dim)] = a21;
                dst[RIDX(dim - 1 - (j + 22), i + k, dim)] = a22;
                dst[RIDX(dim - 1 - (j + 23), i + k, dim)] = a23;
                dst[RIDX(dim - 1 - (j + 24), i + k, dim)] = a24;
                dst[RIDX(dim - 1 - (j + 25), i + k, dim)] = a25;
                dst[RIDX(dim - 1 - (j + 26), i + k, dim)] = a26;
                dst[RIDX(dim - 1 - (j + 27), i + k, dim)] = a27;
                dst[RIDX(dim - 1 - (j + 28), i + k, dim)] = a28;
                dst[RIDX(dim - 1 - (j + 29), i + k, dim)] = a29;
                dst[RIDX(dim - 1 - (j + 30), i + k, dim)] = a30;
                dst[RIDX(dim - 1 - (j + 31), i + k, dim)] = a31;
            }
			
		}
}

char rotate_descr_bolck_codeMove[] = "rotate: 分块+循环展开+减少函数调用";
void rotate_32X32_loopUnroll_codeMove(int dim, pixel *src, pixel *dst) 
{
    int i,j,k;
    //int s;
    int bsize=32;
	for (i = 0; i < dim; i += bsize)
		for (j = 0; j < dim; j += bsize) {
			for (k = 0; k < bsize; k++) {
                pixel *tmp=&src[RIDX(i + k, j + 0, dim)];
               
                dst[RIDX(dim - 1 - (j + 0), i + k, dim)] = *(tmp+0);
                dst[RIDX(dim - 1 - (j + 1), i + k, dim)] = *(tmp+1);
                dst[RIDX(dim - 1 - (j + 2), i + k, dim)] = *(tmp+2);
                dst[RIDX(dim - 1 - (j + 3), i + k, dim)] = *(tmp+3);
                dst[RIDX(dim - 1 - (j + 4), i + k, dim)] = *(tmp+4);
                dst[RIDX(dim - 1 - (j + 5), i + k, dim)] = *(tmp+5);
                dst[RIDX(dim - 1 - (j + 6), i + k, dim)] = *(tmp+6);
                dst[RIDX(dim - 1 - (j + 7), i + k, dim)] = *(tmp+7);
                dst[RIDX(dim - 1 - (j + 8), i + k, dim)] = *(tmp+8);
                dst[RIDX(dim - 1 - (j + 9), i + k, dim)] = *(tmp+9);
                dst[RIDX(dim - 1 - (j + 10), i + k, dim)] = *(tmp+10);
                dst[RIDX(dim - 1 - (j + 11), i + k, dim)] = *(tmp+11);
                dst[RIDX(dim - 1 - (j + 12), i + k, dim)] = *(tmp+12);
                dst[RIDX(dim - 1 - (j + 13), i + k, dim)] = *(tmp+13);
                dst[RIDX(dim - 1 - (j + 14), i + k, dim)] = *(tmp+14);
                dst[RIDX(dim - 1 - (j + 15), i + k, dim)] = *(tmp+15);
                dst[RIDX(dim - 1 - (j + 16), i + k, dim)] = *(tmp+16);
                dst[RIDX(dim - 1 - (j + 17), i + k, dim)] = *(tmp+17);
                dst[RIDX(dim - 1 - (j + 18), i + k, dim)] = *(tmp+18);
                dst[RIDX(dim - 1 - (j + 19), i + k, dim)] = *(tmp+19);
                dst[RIDX(dim - 1 - (j + 20), i + k, dim)] = *(tmp+20);
                dst[RIDX(dim - 1 - (j + 21), i + k, dim)] = *(tmp+21);
                dst[RIDX(dim - 1 - (j + 22), i + k, dim)] = *(tmp+22);
                dst[RIDX(dim - 1 - (j + 23), i + k, dim)] = *(tmp+23);
                dst[RIDX(dim - 1 - (j + 24), i + k, dim)] = *(tmp+24);
                dst[RIDX(dim - 1 - (j + 25), i + k, dim)] = *(tmp+25);
                dst[RIDX(dim - 1 - (j + 26), i + k, dim)] = *(tmp+26);
                dst[RIDX(dim - 1 - (j + 27), i + k, dim)] = *(tmp+27);
                dst[RIDX(dim - 1 - (j + 28), i + k, dim)] = *(tmp+28);
                dst[RIDX(dim - 1 - (j + 29), i + k, dim)] = *(tmp+29);
                dst[RIDX(dim - 1 - (j + 30), i + k, dim)] = *(tmp+30);
                dst[RIDX(dim - 1 - (j + 31), i + k, dim)] = *(tmp+31);
            }
			
		}
}

char rotate_descr_bolck_wrtFirst[] = "rotate: 分块+采用写优先";
void rotate_32X32_wrtFirst(int dim, pixel *src, pixel *dst){
    int i,j,k,s;
    int bsize=32;

    for(i=0;i<dim;i+=bsize)
        for(j=0;j<dim;j+=bsize){
            for(k=0;k<bsize;k++)
                for(s=0;s<bsize;s++){
                    dst[RIDX(dim-1-(i+k), j+s, dim)] = src[RIDX(j+s, i+k, dim)];
            }
             
        }
}

char rotate_descr_bolck16_wrtFirst[] = "rotate: 分块+采用写优先(16X16)";
void rotate_16X16_wrtFirst(int dim, pixel *src, pixel *dst){
    int i,j,k,s;
    int bsize=32;

    for(i=0;i<dim;i+=bsize)
        for(j=0;j<dim;j+=bsize){
            for(k=0;k<bsize;k++)
                for(s=0;s<bsize;s++){
                    dst[RIDX(dim-1-(i+k), j+s, dim)] = src[RIDX(j+s, i+k, dim)];
            }
             
        }
}

char rotate_descr_bolck_loopUnroll_codeMove_wrtFirst[] = "rotate: 分块+循环展开+采用写优先";
void rotate_32X32_loopUnroll_codeMove_wrtFirst(int dim, pixel *src, pixel *dst){
    int i,j,k;
    int bsize=32;

    for(i=0;i<dim;i+=bsize)
        for(j=0;j<dim;j+=bsize){
            for(k=0;k<bsize;k++){
                    dst[RIDX(dim-1-(i+k), j+0, dim)] = src[RIDX(j+0, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+1, dim)] = src[RIDX(j+1, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+2, dim)] = src[RIDX(j+2, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+3, dim)] = src[RIDX(j+3, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+4, dim)] = src[RIDX(j+4, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+5, dim)] = src[RIDX(j+5, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+6, dim)] = src[RIDX(j+6, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+7, dim)] = src[RIDX(j+7, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+8, dim)] = src[RIDX(j+8, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+9, dim)] = src[RIDX(j+9, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+10, dim)] = src[RIDX(j+10, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+11, dim)] = src[RIDX(j+11, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+12, dim)] = src[RIDX(j+12, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+13, dim)] = src[RIDX(j+13, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+14, dim)] = src[RIDX(j+14, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+15, dim)] = src[RIDX(j+15, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+16, dim)] = src[RIDX(j+16, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+17, dim)] = src[RIDX(j+17, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+18, dim)] = src[RIDX(j+18, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+19, dim)] = src[RIDX(j+19, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+20, dim)] = src[RIDX(j+20, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+21, dim)] = src[RIDX(j+21, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+22, dim)] = src[RIDX(j+22, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+23, dim)] = src[RIDX(j+23, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+24, dim)] = src[RIDX(j+24, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+25, dim)] = src[RIDX(j+25, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+26, dim)] = src[RIDX(j+26, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+27, dim)] = src[RIDX(j+27, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+28, dim)] = src[RIDX(j+28, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+29, dim)] = src[RIDX(j+29, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+30, dim)] = src[RIDX(j+30, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+31, dim)] = src[RIDX(j+31, i + k, dim)];

            }   
        }
}

char rotate_descr_bolck_loopUnroll_codeMove_wrtFirst_16x16[] = "rotate: 分块+循环展开+采用写优先(16×16)";
void rotate_16x16_loopUnroll_codeMove_wrtFirst(int dim, pixel *src, pixel *dst){
    int i,j,k;
    int bsize=16;

    for(i=0;i<dim;i+=bsize)
        for(j=0;j<dim;j+=bsize){
            for(k=0;k<bsize;k++){
                    dst[RIDX(dim-1-(i+k), j+0, dim)] = src[RIDX(j+0, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+1, dim)] = src[RIDX(j+1, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+2, dim)] = src[RIDX(j+2, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+3, dim)] = src[RIDX(j+3, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+4, dim)] = src[RIDX(j+4, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+5, dim)] = src[RIDX(j+5, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+6, dim)] = src[RIDX(j+6, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+7, dim)] = src[RIDX(j+7, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+8, dim)] = src[RIDX(j+8, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+9, dim)] = src[RIDX(j+9, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+10, dim)] = src[RIDX(j+10, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+11, dim)] = src[RIDX(j+11, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+12, dim)] = src[RIDX(j+12, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+13, dim)] = src[RIDX(j+13, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+14, dim)] = src[RIDX(j+14, i + k, dim)];
                    dst[RIDX(dim-1-(i+k), j+15, dim)] = src[RIDX(j+15, i + k, dim)];

            }   
        }
}

char rotate_descr_bolck_loopUnroll_copyOL[] = "rotate: 分块+循环展开(网上搬来的)";
void rotate_32X32_loopUnroll_copyOL(int dim, pixel *src, pixel *dst){
int i,j;
   for(i=0;i<dim;i+=32)//32路循环展开，32个像素依次存储
         for(j=dim-1;j>=0;j-=1){       
         pixel*dptr=dst+RIDX(dim-1-j,i,dim);
         pixel*sptr=src+RIDX(i,j,dim);
        //进行复制操作
            COPY(dptr,sptr);sptr+=dim;
            COPY(dptr+1,sptr);sptr+=dim;
            COPY(dptr+2,sptr);sptr+=dim;
            COPY(dptr+3,sptr);sptr+=dim;
            COPY(dptr+4,sptr);sptr+=dim;
            COPY(dptr+5,sptr);sptr+=dim;
            COPY(dptr+6,sptr);sptr+=dim;
            COPY(dptr+7,sptr);sptr+=dim;
            COPY(dptr+8,sptr);sptr+=dim;
            COPY(dptr+9,sptr);sptr+=dim;
            COPY(dptr+10,sptr);sptr+=dim;
            COPY(dptr+11,sptr);sptr+=dim;
            COPY(dptr+12,sptr);sptr+=dim;
            COPY(dptr+13,sptr);sptr+=dim;
            COPY(dptr+14,sptr);sptr+=dim;
            COPY(dptr+15,sptr);sptr+=dim;
            COPY(dptr+16,sptr);sptr+=dim;
            COPY(dptr+17,sptr);sptr+=dim;
            COPY(dptr+18,sptr);sptr+=dim;
            COPY(dptr+19,sptr);sptr+=dim;
            COPY(dptr+20,sptr);sptr+=dim;
            COPY(dptr+21,sptr);sptr+=dim;
            COPY(dptr+22,sptr);sptr+=dim;
            COPY(dptr+23,sptr);sptr+=dim;
            COPY(dptr+24,sptr);sptr+=dim;
            COPY(dptr+25,sptr);sptr+=dim;
            COPY(dptr+26,sptr);sptr+=dim;
            COPY(dptr+27,sptr);sptr+=dim;
            COPY(dptr+28,sptr);sptr+=dim;
            COPY(dptr+29,sptr);sptr+=dim;
            COPY(dptr+30,sptr);sptr+=dim;
            COPY(dptr+31,sptr);
   }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate_32X32_loopUnroll,rotate_descr_bolck);   
    add_rotate_function(&rotate_32X32_loopUnroll_codeMove,rotate_descr_bolck_codeMove);  
    add_rotate_function(&rotate_32X32_wrtFirst,rotate_descr_bolck_wrtFirst); 
    add_rotate_function(&rotate_16X16_wrtFirst,rotate_descr_bolck16_wrtFirst);

    add_rotate_function(&rotate_32X32_loopUnroll_copyOL,rotate_descr_bolck_loopUnroll_copyOL);
    add_rotate_function(&rotate_32X32_loopUnroll_codeMove_wrtFirst,rotate_descr_bolck_loopUnroll_codeMove_wrtFirst); 
    add_rotate_function(&rotate_16x16_loopUnroll_codeMove_wrtFirst,rotate_descr_bolck_loopUnroll_codeMove_wrtFirst_16x16);

    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++){
        for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);        
    } 


    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
    int i,j;
    int dim0=dim;
    int dim1=dim-1;
    int dim2=dim-2;
    pixel *P1, *P2, *P3;
    pixel *dst1;
    P1=src;
    P2=P1+dim0;
    //左上角像素处理
    dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red)>>2;
    dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green)>>2;
    dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue)>>2;
    dst++;
    //上边界处理
    for(i=1;i<dim1;i++)
    {
        dst->red=(P1->red+(P1+1)->red+(P1+2)->red+P2->red+(P2+1)->red+(P2+2)->red)/6;
        dst->green=(P1->green+(P1+1)->green+(P1+2)->green+P2->green+(P2+1)->green+(P2+2)->green)/6;
        dst->blue=(P1->blue+(P1+1)->blue+(P1+2)->blue+P2->blue+(P2+1)->blue+(P2+2)->blue)/6;
        dst++;
        P1++;
        P2++;
    }
    //右上角像素处理
    dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red)>>2;
    dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green)>>2;
    dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue)>>2;
    dst++;
    P1=src;
    P2=P1+dim0;
    P3=P2+dim0;
    //左边界处理
    for(i=1;i<dim1;i++)
    {
        dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red+P3->red+(P3+1)->red)/6;
        dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green+P3->green+(P3+ 1)->green)/6;
        dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue+P3->blue+(P3+1)->blue)/6;
        dst++;
        dst1=dst+1;
        //中间主体部分处理
        for(j=1;j<dim2;j+=2)
        {        
            //同时处理两个像素
           dst->red=(P1->red+(P1+1)->red+(P1+2)->red+P2->red+(P2+1)->red+(P2+2)->red+P3->red+(P3+1)->red+(P3+2)->red)/9;
           dst->green=(P1->green+(P1+1)->green+(P1+2)->green+P2->green+(P2+1)->green+(P2+2)->green+P3->green+(P3+1)->green+(P3+2)->green)/9;
           dst->blue=(P1->blue+(P1+1)->blue+(P1+2)->blue+P2->blue+(P2+1)->blue+(P2+2)->blue+P3->blue+(P3+1)->blue+(P3+2)->blue)/9;
           dst1->red=((P1+3)->red+(P1+1)->red+(P1+2)->red+(P2+3)->red+(P2+1)->red+(P2+2)->red+(P3+3)->red+(P3+1)->red+(P3+2)->red)/9;
           dst1->green=((P1+3)->green+(P1+1)->green+(P1+2)->green+(P2+3)->green+(P2+1)->green+(P2+2)->green+(P3+3)->green+(P3+1)->green+(P3+2)->green)/9;
           dst1->blue=((P1+3)->blue+(P1+1)->blue+(P1+2)->blue+(P2+3)->blue+(P2+1)->blue+(P2+2)->blue+(P3+3)->blue+(P3+1)->blue+(P3+2)->blue)/9;
           dst+=2;
           dst1+=2;
           P1+=2;
           P2+=2;
           P3+=2;
        }
        for(;j<dim1;j++)
        {         
           dst->red=(P1->red+(P1+1)->red+(P1+2)->red+P2->red+(P2+1)->red+(P2+2)->red+P3->red+(P3+1)->red+(P3+2)->red)/9;
           dst->green=(P1->green+(P1+1)->green+(P1+2)->green+P2->green+(P2+1)->green+(P2+2)->green+P3->green+(P3+1)->green+(P3+2)->green)/9;
           dst->blue=(P1->blue+(P1+1)->blue+(P1+2)->blue+P2->blue+(P2+1)->blue+(P2+2)->blue+P3->blue+(P3+1)->blue+(P3+2)->blue)/9;
           dst++;
           P1++;
           P2++;
           P3++;
        }
        //右侧边界处理
        dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red+P3->red+(P3+1)->red)/6;
        dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green+P3->green+(P3+1)->green)/6;
        dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue+P3->blue+(P3+1)->blue)/6;
        dst++;
        P1+=2;
        P2+=2;
        P3+=2;
     } 
    //右下角处理
     dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red)>>2;
     dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green)>>2;
     dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue)>>2;
     dst++;
    //下边界处理
     for(i=1;i<dim1;i++)
     {
        dst->red=(P1->red+(P1+1)->red+(P1+2)->red+P2->red+(P2+1)->red+(P2+2)->red)/6;
        dst->green=(P1->green+(P1+1)->green+(P1+2)->green+P2->green+(P2+1)->green+(P2+2)->green)/6;
        dst->blue=(P1->blue+(P1+1)->blue+(P1+2)->blue+P2->blue+(P2+1)->blue+(P2+2)->blue)/6;
        dst++;
        P1++;
        P2++;
     } 
    //右下角像素处理
     dst->red=(P1->red+(P1+1)->red+P2->red+(P2+1)->red)>>2;
     dst->green=(P1->green+(P1+1)->green+P2->green+(P2+1)->green)>>2;
     dst->blue=(P1->blue+(P1+1)->blue+P2->blue+(P2+1)->blue)>>2;
 }




/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

