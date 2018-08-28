#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rpn.c"

int calc(const char *str, int len) {
    int result;
    int rpn_len;

    char *rpn = malloc(len+1);
    rpn_len = convert_to_rpn(str, len, rpn);
    result = rpn_calc(rpn, rpn_len);
    free(rpn);

    return result;
}

struct test_case {
    char *in;
    int expect;
};

int main(int argc, const char *argv[])
{
    struct test_case cases[] = {
        {"1+2+3+4", 10}, 
        {"(3+4)*5-6", 29},
        {"(4-3)*5-6", -1},
        {"2*(5-3)+5", 9},
        {"2*(9-3*2)+5*2", 16},
        {"2+(9-(3+3)/2)*2+5*2", 24}
    };  

    for(int i=0; i<sizeof(cases)/sizeof(struct test_case); i++) {
        struct test_case *c = &cases[i];

        int v = calc(c->in, strlen(c->in));
        if(v!=c->expect) {
            printf("[FAIL] case[%d] '%s', value(%d)!=expect(%d)\n"
                            , i, c->in, v, c->expect);
        } else {
            printf("[OK] case[%d] '%s'\n", i, c->in);
        }
    }
    
    return 0;
}
