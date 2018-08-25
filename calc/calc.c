#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM(c) ((c)-'0')

int calc(const char *in, int len) {
    if(len==1) {
        return NUM(in[0]);
    }

    int start = 0;
    int end = len;

    char op;
    int v0, v1;
    int index = start;
    v0 = NUM(in[index++]);
    while(index<end) {
        op = in[index++];

        if(in[index]=='(') { // Case '(...)'
            int pos = index+1;
            for(; pos<len; pos++) {
                if(in[pos]==')') {
                    break;
                }
            }
            if(pos>=len) 
                abort();// Should never hapen
            
            // Copy '(...)', exclude '(' & ')'
            int nlen = pos-index-1;
            char *nbuf = malloc(nlen);
            memcpy(nbuf, in+index+1, nlen);
            v1 = calc(nbuf, nlen);

            // Skip '(....)'
            index = pos+1; 
        } else if(in[index+1]=='*' || in[index+1]=='/') { // Case high priority op '*' or '/'
            int nlen = len-index;
            char *nbuf = malloc(nlen);
            memcpy(nbuf, in+index, nlen);
            v1 = calc(nbuf, nlen);
            
            // Skip to the end
            index = len; 
        } else {
            v1 = NUM(in[index++]);
        }

        switch(op) {
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
        }
    }
    return v0;
}

struct test_case {
    char *in;
    int expect;
};

int main(int argc, const char *argv[])
{
    struct test_case cases[] = {
        {"8", 8},
        {"1+2+3+4", 10},
        {"1+2+8-5", 6},
        {"1+2*3", 7},
        {"1+2*3+5/3", 8},
        {"1+2*3+5/3+1+2*3", 15},
        {"1+2*(3+4)/7+1", 4}
    };  

    for(int i=0; i<sizeof(cases)/sizeof(struct test_case); i++) {
        struct test_case *c = &cases[i];

        int v = calc(c->in, strlen(c->in));
        if(v!=c->expect) {
            printf("[FAIL] case[%d] '%s', value(%d)!=expect(%d)\n"
                            , i, c->in, v, c->expect);
        } else {
            printf("[OK] case[%d]\n", i);
        }
    }

    return 0;
}
