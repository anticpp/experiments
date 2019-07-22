#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

int cnt_comp = 0;

void mqsort(int arr[], int start, int end) {
    int i = start;
    int j = end;
    int sentinel = arr[j];
    while (i<j) {
        while(i<j && arr[i]<=sentinel) { i++; cnt_comp++; }
        arr[j] = arr[i];

        while(i<j && arr[j]>=sentinel) { j--; cnt_comp++; }
        arr[i] = arr[j];
    }
    assert(i==j);
    arr[i] = sentinel;

    // Left
    if(i-start>0)
        mqsort(arr, start, i-1);

    // Right
    if(end-i>0)
        mqsort(arr, i+1, end);
}

const int max_array_value = 100;
const int max_array_len = 30;
const int sample_num = 10;

typedef struct {
    int *a;
    int len;
} array;

array* array_create(int len) {
    array *arr = (array *)malloc(sizeof(array)+sizeof(int)*len);
    arr->a = (int *)(arr+1);
    arr->len = len;
    return arr;
}

void array_destroy(array *arr) {
    free(arr);
}

int main(int argc, const char *argv[]) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec * 1000 
                + tv.tv_usec / 1000); // milisecond

    // Create samples
    array *samples[sample_num];
    for(int i=0; i<sample_num; i++) {
        int len = (random() % max_array_len);
        if(len==0) 
            len = 10;
        
        array *arr = array_create(len);
        samples[i] = arr;
        for(int j=0; j<len; j++) {
            arr->a[j] = (random() % max_array_value);
        }
    }

    for(int i=0; i<sample_num; i++) {
        array *arr = samples[i];
        printf("Array Len: %d\n", arr->len);

        cnt_comp = 0;
        mqsort(arr->a, 0, arr->len-1);
 
        printf("Compare: %d\n", cnt_comp);
        for(int i=0; i<arr->len; i++) {
            printf("%d, ", arr->a[i]);
        }
        printf("\n");
    }
}

