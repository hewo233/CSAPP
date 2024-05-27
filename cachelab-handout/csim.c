#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int tag;
    int valid;
    int lru_tag;
}tCacheLine;

int my_time = 0;

int hits, misses, evictions;

int get_tag(int address, int s, int b)
{
    return address >> (s + b);
}

int get_set(int address, int s, int b)
{
    return (address >> b) & ((1 << s) - 1);
}

tCacheLine** init(int s, int e) {
    int S = (1 << s);
    tCacheLine** cache = malloc(S * sizeof(tCacheLine*));
    if (cache == NULL) {
        return NULL;
    }
    for (int i = 0; i < S; i++) {
        cache[i] = malloc(e * sizeof(tCacheLine));
        if (cache[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(cache[j]);
            }
            free(cache);
            return NULL;
        }
    }
    hits = misses = evictions = 0;
    return cache;
}

void free_cache(tCacheLine** cache, int s) {
    int S = (1 << s);
    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }
    free(cache);
}

void L_operate(tCacheLine** cache, int address, int s,int b,int e)
{
    int tag = get_tag(address, s, b);
    int set = get_set(address, s, b);
    if(cache[set][e].valid == 0) // cache miss
    {
        misses ++;
        printf(" miss");
        cache[set][e].valid = 1;
        cache[set][e].tag = tag;
        cache[set][e].lru_tag = ++my_time;
    }
    else 
    {
        if(cache[set][e].tag == tag) //cache hit
        {
            hits ++;
            printf(" hit");
        }
        else // eviction
        {
            misses ++;
            evictions ++;
            printf(" miss eviction");
            int change_e = 0;
            for(int i=0;i<e;i++)
            {
                if(cache[set][i].lru_tag < cache[set][change_e].lru_tag)
                {
                    i = change_e;
                }
            }
            cache[set][e].tag = tag;
            cache[set][e].lru_tag = ++my_time;
        }
    }
}

void S_operate(tCacheLine** cache, int address, int s, int b, int e)
{
    int tag = get_tag(address, s, b);
    int set = get_set(address, s, b);
    if(cache[set][e].valid == 0)
    {
        misses ++;
        printf(" miss");
    }
    else
    {
        if(cache[set][e].tag == tag)
        {
            hits ++;
            printf(" hit");
        }
        else
        {
            misses ++;
            printf(" miss");
        }
    }
}

void M_operate(tCacheLine** cache, int address, int s, int b, int e)
{
    L_operate(cache, address, s, b, e);
    S_operate(cache, address, s, b, e);
}

int main()
{
    printSummary(hits, misses, evictions);
    return 0;
}
