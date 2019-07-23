#include <stdlib.h>
#include <string.h>

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

array* array_copy(array *arr) {
    array *n = array_create(arr->len);
    memcpy(n->a, arr->a, arr->len * sizeof(arr->len));
    return n;
}

void array_destroy(array *arr) {
    free(arr);
}

void array_print(array *arr) {
    printf("[");
    for(int i=0; i<arr->len; i++) {
        if(i==arr->len-1) 
            printf("%d", arr->a[i]);
        else 
            printf("%d, ", arr->a[i]);
     }
     printf("]\n");
}
