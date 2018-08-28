#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stack.c"
#include "queue.c"

#define MUSTBE(cond, msg) \
        if(!(cond)) {\
            printf("abort capture: %s\n", (msg));\
            abort();\
        }\

#define NUM(c) ((c)-'0')

int compare_op(char c1, char c2) {
    if(c1=='*' || c1=='/') {
        if(c2=='+' || c2=='-') 
            return 1;
        else
            return 0;
    }

    if(c1=='+' || c1=='-') {
        if(c2=='*' || c2=='/')
            return -1;
        else
            return 0;
    }
    return 0;// Should never be here
}

int convert_to_rpn(const char *in, int len, char *out) {
    char c, cc;

    struct stack *stk = stack_create(len+100, sizeof(char));
    struct queue *que = queue_create(len+100, sizeof(char));
    for(int i=0; i<len; i++) {
        c = in[i];   

        // If digital
        // Push to queue tail
        if(isdigit(c)) {
            MUSTBE(queue_push_tail(que, &c), "push stack c fail");
            continue;
        } else if(c=='(') { // Just push to stack
            MUSTBE(stack_push(stk, &c), "stack push c fail");
        } else if(c==')') { // Pop all '(...)', and push to queue
            while(stack_pop(stk, &cc)) {
                if(cc=='(') {
                    break;
                }
                MUSTBE(queue_push_tail(que, &cc), "push queue cc fail");
            }
        } else { // Pop all lower-or-equal priority, and push to queue
            while(stack_pop(stk, &cc)) {
                if(cc=='(') {
                    MUSTBE(stack_push(stk, &cc), "push stack cc fail");
                    break;
                }
                if( compare_op(c, cc)>0) {
                    MUSTBE(stack_push(stk, &cc), "push stack cc fail");
                    break;
                }
            
                MUSTBE(queue_push_tail(que, &cc), "push queue cc fail");
            }
            MUSTBE(stack_push(stk, &c), "push stack fail");
        }
    }

    // Pop all op, and push to queue
    while(stack_pop(stk, &c)) {
        MUSTBE(queue_push_tail(que, &c), "push queue tail fail");
    }
    
    // Pop from queue to out
    int i=0;
    while(queue_pop_head(que, &c)) {
        out[i++] = c;
    }

    stack_destroy(stk);
    queue_destroy(que);
    return i;
}

// Caculate revert polish notation
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
            MUSTBE(stack_push(stk, &v0), "push v0 fail");
            continue;
        }

        // Else operator
        // Pop stack 2 numbers and calculate
        MUSTBE(stack_pop(stk, &v0), "pop v0 fail");
        MUSTBE(stack_pop(stk, &v1), "pop v1 fail");

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
        MUSTBE(stack_push(stk, &v1), "push new number fail");
    }

    MUSTBE(stack_pop(stk, &v0), "pop last value fail");
    stack_destroy(stk);
    return v0;
}


