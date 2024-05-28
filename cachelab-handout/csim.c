#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct{
    int tag;
    int valid;
    int lru_tag;
}tCacheLine;

int hits, misses, evictions;

int get_tag(unsigned long address, int s, int b)
{
    return address >> (s + b);
}

int get_set(unsigned long address, int s, int b)
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

void access(tCacheLine** cache, unsigned long address,int s, int b, int e)
{
    int tag = get_tag(address, s, b);
    int set = get_set(address, s, b);

    for(int i = 0; i < e; i++)
    {
        if(cache[set][i].valid == 1 && cache[set][i].tag == tag)
        {
            hits++;
            cache[set][i].lru_tag = 0;
            return;
        }
    } // cache hit

    misses++;

    for(int i = 0;i < e; i++)
    {
        if(cache[set][i].valid == 0)
        {
            cache[set][i].valid = 1;
            cache[set][i].tag = tag;
            cache[set][i].lru_tag = 0;
            return;
        }
    }

    evictions++;

    int lru_now = 0;
    for(int i = 0; i < e; i++)
    {
        if(cache[set][i].lru_tag > cache[set][lru_now].lru_tag)
        {
            lru_now = i;
        } 
    }
    cache[set][lru_now].tag = tag;
    cache[set][lru_now].lru_tag = 0;
}

void update_lru(tCacheLine** cache,int s,int e)
{
    int S = (1 << s);
    for(int i = 0; i < S; i++)
    {
        for(int j = 0; j < e; j++)
        {
            if(cache[i][j].valid == 1)
            {
                cache[i][j].lru_tag++;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int s, e, b;
    FILE *fp;
    char *filePath = (char*)malloc(100);

    int opt = 0;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) 
    {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                break;

            case 'E':
                e = atoi(optarg);
                break;

            case 'b':
                b = atoi(optarg);
                break;

            case 't':
                strcpy(filePath, optarg);
                break;
        }
    }

    tCacheLine** cache = init(s, e);

    fp = fopen(filePath, "r");

    char operation;
    unsigned long address = 0;
    int size = 0;
    while(fscanf(fp," %c %lx,%d", &operation, &address, &size) > 0)
    {
        switch(operation)
        {
            case 'L':
                access(cache, address, s, b, e);
                break;
            case 'S':
                access(cache, address, s, b, e);
                break;
            case 'M':
                access(cache, address, s, b, e);
                access(cache, address, s, b, e);
                break;
        }
        update_lru(cache, s, e);
    }

    fclose(fp);

    free_cache(cache, s);
    free(filePath);

    printSummary(hits, misses, evictions);
    return 0;
}
