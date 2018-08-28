#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rpn.c"

struct test_case1 {
    char *in;
    char *expect;
};

struct test_case2 {
    char *in;
    int expect;
};

int main(int argc, const char *argv[])
{
#if 1
    printf("[TEST Convert RPN]\n");
    struct test_case1 cases1[] = {
        {"1+2+3+4", "12+3+4+"},
        {"(3+4)*5-6", "34+5*6-"},
        {"(4-3)*5-6", "43-5*6-"},
        {"2*(5-3)+5", "253-*5+"},
        {"2*(9-3*2)+5*2", "2932*-*52*+"},
        {"2+(9-(3+3)/2)*2+5*2", "2933+2/-2*+52*+"}
    }; 
    for(int i=0; i<sizeof(cases1)/sizeof(struct test_case1); i++) {
        struct test_case1 *c = &cases1[i];
        
        char *out = malloc(strlen(c->in)+1);
        memset(out, 0x00, strlen(c->in)+1);
        convert_to_rpn(c->in, strlen(c->in), out);
        if(memcmp(out, c->expect, strlen(c->expect))!=0) {
            printf("[FAIL] case[%d] '%s', out('%s')!=expect('%s')\n"
                            , i, c->in, out, c->expect);
        } else {
            printf("[OK] case[%d] '%s'\n", i, c->in);
        }
        free(out);
    }
#endif
#if 1
    // Revert polish notation
    printf("[TEST Revert polish notation]\n");
    struct test_case2 cases2[] = {
        {"12+3+4+", 10}, // 1+2+3+4
        {"34+5*6-", 29}, // (3+4)*5-6
        {"43-5*6-", -1}, // (4-3)*5-6
        {"253-*5+", 9}, //  2*(5-3)+5
        {"2932*-*52*+", 16}, // 2*(9-3*2)+5*2
        {"2933+2/-2*+52*+", 24} // 2+(9-(3+3)/2)*2+5*2
    };  

    for(int i=0; i<sizeof(cases2)/sizeof(struct test_case2); i++) {
        struct test_case2 *c = &cases2[i];

        int v = rpn_calc(c->in, strlen(c->in));
        if(v!=c->expect) {
            printf("[FAIL] case[%d] '%s', value(%d)!=expect(%d)\n"
                            , i, c->in, v, c->expect);
        } else {
            printf("[OK] case[%d] '%s'\n", i, c->in);
        }
    }
    
#endif
    return 0;
}
