#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>
#include "qsort.c"
#include "array.c"
#include "count.c"

const int DEFAULT_SAMPLE_NUM = 10;
const int DEFAULT_MAX_ARRAY_LEN = 30;
const int DEFAULT_MAX_ARRAY_VAL = 100;

typedef array* sample;

/*
 * Return:
 *     [0]->array[1, 2, 3, 4...xx]
 *     [1]->array[1, 2, 3, 4, 5...xx]
 *     [2]->array[1, 2, 3, 4, 5...xx]
 *     ...
 *     [sample_num]->array[1, 2, 3, 4, 5...xx]
 *
 * sample_num:           One sample is an array, `sample_num` samples to be created.
 * max_array_len:        Maximun length of each array.
 * max_array_value:      Maximum of array value.
 */
sample* create_random_samples(int sample_num, int max_array_len, int max_array_value) {
    // srandom
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec * 1000 
                + tv.tv_usec / 1000); // milisecond

    sample *s = malloc(sizeof(sample)*sample_num);
    for(int i=0; i<sample_num; i++) {
        int len = (random() % max_array_len);
        if(len==0) 
            len = 10; // Deffault length
        
        array *arr = array_create(len);
        s[i] = arr;
        for(int j=0; j<len; j++) {
            arr->a[j] = (random() % max_array_value);
        }
    }
    return s;
}

void destroy_samples(sample *s, int num) {
    for(int i=0; i<num; i++) {
        array *arr = s[i];
        array_destroy(arr);
    }
    free(s);
}

int main(int argc, const char *argv[]) {
    int pflag = 0;
    int sn = DEFAULT_SAMPLE_NUM;
    int mal = DEFAULT_MAX_ARRAY_LEN;
    int mav = DEFAULT_MAX_ARRAY_VAL;
    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-p")==0) {
            pflag = 1;
        } else if(strcmp(argv[i], "-sn")==0) {
            sn = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "-mal")==0) {
            mal = atoi(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "-mav")==0) {
            mav = atoi(argv[i+1]);
            i++;
        } else {
            printf("Usage: %s [options]\n\n", argv[0]);
            printf("    -p        Print array data.\n");
            printf("    -sn int   Sample number. Default %d.\n", DEFAULT_SAMPLE_NUM);
            printf("    -mal int  Max array len. Default %d.\n", DEFAULT_MAX_ARRAY_LEN);
            printf("    -mav int  Max array value. Default %d.\n", DEFAULT_MAX_ARRAY_VAL);
            printf("\n");
            return 0;
        }
    }

    printf("Samples: %d, Max array len: %d, Max array value: %d.\n", sn, mal, mav);

    sample *s = create_random_samples(sn, mal, mav);
    for(int i=0; i<sn; i++) {
        cnt_reset();

        array *arr = s[i];
        if(pflag)
            array_print(arr);

        mqsort(arr->a, 0, arr->len-1);

        if(pflag)
            array_print(arr);
        printf("N: %d, C: %d\n", arr->len, cnt_get_comp());
    }

    destroy_samples(s, sn);
    return 0;
}
