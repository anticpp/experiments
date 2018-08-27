#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MUSTBE(cond, msg) \
        if(!(cond)) {\
            printf("abort capture: %s\n", (msg));\
            abort();\
        }\


#define NUM(c) ((c)-'0')

// Simple stack
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
        return -1;
    }
    memcpy(&(s->s[s->top*s->nsize]), p, s->nsize);
    s->top++;
    return 0;
}

int stack_pop(struct stack *s, void *p) {
    if(s->top==0) {
        return -1;
    }
    s->top--;
    memcpy(p, &(s->s[s->top*s->nsize]), s->nsize);
    return 0;
}

// Polish notation
int pn_calc(const char *str, int len) {
    int v0, v1;
    char c;

    struct stack *stk = stack_create(len+100, sizeof(int));
    for(int i=len-1; i>=0; i--) {
        c = str[i]; 

        // If digital
        // Push stack
        if(isdigit(c))  {
            v0 = NUM(c);
            MUSTBE(stack_push(stk, &v0)==0, "push v0 fail");
            continue;
        }

        // Else operator
        // Pop stack 2 numbers and calculate
        MUSTBE(stack_pop(stk, &v0)==0, "pop v0 fail");
        MUSTBE(stack_pop(stk, &v1)==0, "pop v1 fail");

        switch(c) {
            case '+': 
                v0 += v1;
                break;
            case '-': 
                v0 -= v1;
                break;
            case '*': 
                v0 *= v1;
                break;
            case '/': 
                v0 /= v1;
                break;
            default:
                printf("Unknown operator: %c\n", c);
                abort();
        }

        // Push new number
        MUSTBE(stack_push(stk, &v0)==0, "push new number fail");
    }

    MUSTBE(stack_pop(stk, &v0)==0, "pop last value fail");
    stack_destroy(stk);
    return v0;
}

// Revert polish notation
int rpn_calc(const char *str, int len) {
    int v0, v1;
    char c;

    struct stack *stk = stack_create(len+100, sizeof(int));
    for(int i=0; i<len; i++) {
        c = str[i];

        // If digital
        // Push stack
        if(isdigit(c)) {
            v0 = NUM(c);
            MUSTBE(stack_push(stk, &v0)==0, "push v0 fail");
            continue;
        }

        // Else operator
        // Pop stack 2 numbers and calculate
        MUSTBE(stack_pop(stk, &v0)==0, "pop v0 fail");
        MUSTBE(stack_pop(stk, &v1)==0, "pop v1 fail");

        switch(c) {
            case '+': 
                v1 += v0;
                break;
            case '-': 
                v1 -= v0;
                break;
            case '*': 
                v1 *= v0;
                break;
            case '/': 
                v1 /= v0;
                break;
            default:
                printf("Unknown operator: %c\n", c);
                abort();
        }

        // Push new number
        MUSTBE(stack_push(stk, &v1)==0, "push new number fail");
    }

    MUSTBE(stack_pop(stk, &v0)==0, "pop last value fail");
    stack_destroy(stk);
    return v0;
}

struct test_case {
    char *in;
    int expect;
};

int main(int argc, const char *argv[])
{
    // Revert polish notation
    printf("[TEST Revert polish notation]\n");
    struct test_case cases1[] = {
        {"34+5*6-", 29}, // (3+4)×5-6
        {"43-5*6-", -1} // (4-3)×5-6
    };  

    for(int i=0; i<sizeof(cases1)/sizeof(struct test_case); i++) {
        struct test_case *c = &cases1[i];

        int v = rpn_calc(c->in, strlen(c->in));
        if(v!=c->expect) {
            printf("[FAIL] case[%d] '%s', value(%d)!=expect(%d)\n"
                            , i, c->in, v, c->expect);
        } else {
            printf("[OK] case[%d]\n", i);
        }
    }
    
    // Polish notation
    printf("[TEST Polish notation]\n");
    struct test_case cases2[] = {
        {"-*+3456", 29}, // (3+4)×5-6
        {"-*-4356", -1} // (4-3)×5-6
    };  

    for(int i=0; i<sizeof(cases2)/sizeof(struct test_case); i++) {
        struct test_case *c = &cases2[i];

        int v = pn_calc(c->in, strlen(c->in));
        if(v!=c->expect) {
            printf("[FAIL] case[%d] '%s', value(%d)!=expect(%d)\n"
                            , i, c->in, v, c->expect);
        } else {
            printf("[OK] case[%d]\n", i);
        }
    }

    return 0;
}
