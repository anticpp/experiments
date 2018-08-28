#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct queue {
    void *q;
    int nsize;
    int cap;
    int head;
    int tail;
};

struct queue *queue_create(int cap, int nsize) {
    struct queue *q = malloc(sizeof(struct queue)+cap*nsize);
    if(!q) {
        return NULL;
    }
    q->q = (void *)(q+1);
    q->nsize = nsize;
    q->cap = cap;
    q->head = 0;
    q->tail = 0;
    return q;
}

int queue_push_tail(struct queue *q, void *p) {
    if(q->tail>=q->cap) {
        return 0;
    }

    memcpy(&(q->q[q->tail*q->nsize]), p, q->nsize);
    q->tail++;
    return 1;
}

int queue_pop_head(struct queue *q, void *p) {
    if(q->head>=q->tail) {
        return 0;
    }

    memcpy(p, &(q->q[q->head*q->nsize]), q->nsize);
    q->head++;
    return 1;
}

void queue_destroy(struct queue *q) {
    free(q);
}

