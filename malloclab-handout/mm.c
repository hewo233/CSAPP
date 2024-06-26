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
#include <stddef.h>
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
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE   (4)
#define DSIZE   (8)
#define CHUNKSIZE   (1 << 12)

#define MAX(x, y)   ((x) > (y) ? (x) : (y))

#define PACK(size, alloc)   ((size) | (alloc))

#define GET(p)          (*(unsigned int *)(p))
#define PUT(p, val)     (*(unsigned int *)(p) = (val))

#define GET_SIZE(p)     (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

// next or prev bp
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)) )

static char *heap_listp = NULL;
static char *save_bp;

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if(prev_alloc && next_alloc)
    {
        return bp;
    }

    else if(prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if(!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    
    else
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    if ((save_bp > (char *)bp) && (save_bp < NEXT_BLKP(bp)))
        save_bp = bp;

    return bp;

}

static void *extend_heap(size_t s)
{
    char *bp;
    
    if(s & 1) s += 1;
    size_t size = s * WSIZE;

    bp = mem_sbrk(size);

    if(bp == (void *)-1)
    {
        printf("mem_sbrk error\n");
        return NULL;
    }
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
    heap_listp += (2 * WSIZE);

    save_bp = heap_listp;

    if(extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}

// first fit search

static void *find_fit(size_t size)
{
    char *old_bp = save_bp;
    for(; GET_SIZE(HDRP(save_bp)) > 0; save_bp = NEXT_BLKP(save_bp))
    {
        if(!GET_ALLOC(HDRP(save_bp)) && (size <= GET_SIZE(HDRP(save_bp))))
        {
            return save_bp;
        }
    }

    for(save_bp = heap_listp; save_bp < old_bp; save_bp = NEXT_BLKP(save_bp))
    {
        if(!GET_ALLOC(HDRP(save_bp)) && (size <= GET_SIZE(HDRP(save_bp))))
        {
            return save_bp;
        }
    }
    return NULL;
}

static int place(void *bp, size_t size)
{
    size_t csize = GET_SIZE(HDRP(bp));
    //printf("\nNOW PLACE: size = %zu, csize = %zu\n", size, csize);
    
    if(csize - size < 0) printf("csize - size < DSIZE\n");
    if(GET_ALLOC(HDRP(bp))) printf("GET_ALLOC(bp)\n");
    if((csize - size < 0) || GET_ALLOC(HDRP(bp))) return -1;
    if((csize - size) >= (2 * DSIZE))
    {
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - size, 0));
        PUT(FTRP(bp), PACK(csize - size, 0));
    }
    else
    {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if(size == 0) return NULL;
    if(size <= DSIZE) asize = 2 * DSIZE;
    else asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);

    
    if((bp = find_fit(asize)) != NULL)
    {
        //printf("FIND\n");
        if(place(bp, asize) == -1)
        {
            printf("PLACEERR\n");
            return NULL;
        }
        //printf("NOERR1\n");
        return bp;
    }
    //printf("NOERR2\n");
    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extendsize / WSIZE)) == NULL)
    {
        return NULL;
    }
    if(place(bp, asize) == -1) return NULL;
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if(ptr == NULL) return mm_malloc(size);
    if(size == 0)
    {
        mm_free(ptr);
        return NULL;
    }
    void *newptr = mm_malloc(size);
    if(newptr == NULL) return NULL;
    size_t oldsize = GET_SIZE(HDRP(ptr));
    if(size < oldsize) oldsize = size;
    memcpy(newptr, ptr, oldsize);
    mm_free(ptr);
    return newptr;
}














