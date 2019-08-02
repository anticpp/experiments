#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include "utility.c"
#include "count.c"
#include "array.c"
#include "test_data.c"

#include "isort.c"
#include "ssort.c"
#include "qsort.c"
#include "bsort.c"

const int DEFAULT_TEST_NUM = 10;
const int DEFAULT_MAX_ARRAY_LEN = 50000;
const int DEFAULT_MAX_ARRAY_VAL = 100000;

// Sort function
typedef void (*sortfunc)(int*, int, int);

typedef struct {
    const char *name;
    sortfunc f;
} sort;

sort sorts[] = {
        {"isort", isort}, 
        {"ssort", ssort}, 
        {"bsort", bsort},
        {"bbsort", bbsort},
        {"qsort", mqsort}
};
int sorts_n = sizeof(sorts)/sizeof(sort);

// Main
int main(int argc, const char *argv[]) {
    int pflag = 0; // Print flag
    int cflag = 0; // Check flag
    int tn = DEFAULT_TEST_NUM;
    int mal = DEFAULT_MAX_ARRAY_LEN;
    int mav = DEFAULT_MAX_ARRAY_VAL;
    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-p")==0) {
            pflag = 1;
        } else if(strcmp(argv[i], "-c")==0) {
            cflag = 1;
        } else if(strcmp(argv[i], "-tn")==0) {
            tn = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "-mal")==0) {
            mal = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "-mav")==0) {
            mav = atoi(argv[i+1]);
            i++;
        } else {
            printf("Usage: %s [options]\n\n", argv[0]);
            printf("    -p        Print array data before and after sorted. Useful for handy debug.\n");
            printf("    -c        Check if correct order for sorted data.\n");
            printf("    -tn int   Test data number. Default %d.\n", DEFAULT_TEST_NUM);
            printf("    -mal int  Max array len. Default %d.\n", DEFAULT_MAX_ARRAY_LEN);
            printf("    -mav int  Max array value. Default %d.\n", DEFAULT_MAX_ARRAY_VAL);
            printf("\n");
            return 0;
        }
    }

    printf("Tests(n): %d, Max array len: %d, Max array value: %d.\n", tn, mal, mav);
    if(cflag) {
        printf("Sort check: On.\n\n");
    } else {
        printf("Sort check: Off.\n\n");
    }

    test_data *td = test_data_create(tn);
    test_data_fullfill(td, mal, mav);
    for(int i=0; i<td->n; i++) {
        array *arr = td->arrs[i];

        if(pflag)
            array_print(arr);

        printf("Array Len: %d\n", arr->len);
        printf("%-15s %-15s %-15s\n", "Algorithm", "Compare", "Elapse(ms)");
        for(int j=0; j<sorts_n; j++) {
            sort st = sorts[j];
            array *narr = array_copy(arr);

            // Reset counter
            cnt_reset();

            time_t begin_us = now_us(); // Begin time
            st.f(narr->a, 0, narr->len-1);
            time_t elapse_us = now_us() - begin_us; // Elapse

            // Check sort result
            if( cflag && !array_is_asc(narr) )
                printf("Error: Sort check fail for '%s'\n", st.name);
            
            printf("%-15s %-15d %ld.%-10ld\n", st.name, cnt_get_comp(), elapse_us/1000, elapse_us%1000);
            if(pflag) {
                printf("Sorted: ");
                array_print(narr);
            }

            array_destroy(narr);
        }
        printf("\n");
    }

    test_data_destroy(td);
    return 0;
}
