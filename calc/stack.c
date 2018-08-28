#include <string.h>

struct stack {
    void *s;
    int nsize;
    int cap;
    int top;
};

struct stack *stack_create(int cap, int nsize) {
    struct stack *s = malloc(sizeof(struct stack)+cap*nsize);
    if(!s) {
        return NULL;
    }
    s->s = (void *)(s+1);
    s->nsize = nsize;
    s->cap = cap;
    s->top = 0;
    return s;
}

void stack_destroy(struct stack *s) {
    free(s);
}

int stack_push(struct stack *s, void *p) {
    if(s->top>=s->cap) {
        return 0;
    }
    memcpy(&(s->s[s->top*s->nsize]), p, s->nsize);
    s->top++;
    return 1;
}

int stack_pop(struct stack *s, void *p) {
    if(s->top==0) {
        return 0;
    }
    s->top--;
    memcpy(p, &(s->s[s->top*s->nsize]), s->nsize);
    return 1;
}
