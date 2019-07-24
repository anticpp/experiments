#include <sys/time.h>
#include <stdlib.h>

// Test data
typedef struct {
    array** arrs;
    int n;
} test_data;

test_data* test_data_create(int n) {
    test_data *td = malloc(sizeof(test_data));
    td->arrs = malloc(sizeof(array*)*n);
    td->n = n;

    for(int i=0; i<n; i++)
        td->arrs[i] = 0;

    return td;
}

/*
 * Fullfill test_data with `n` random arrays, which, each array has a random lenth maximum to `max_array_len`,
 * each value of array is random maximum to `max_array_value`.
 *
 * `test_data`:
 *     [0]   -> array[1, 2, 3, 4...xx]
 *     [1]   -> array[1, 2, 3, 4, 5...xx]
 *     [2]   -> array[1, 2, 3, 4, 5...xx]
 *     ...
 *     [n-1] -> array[1, 2, 3, 4, 5...xx]
 */
void test_data_fullfill(test_data *td, int max_array_len, int max_array_value) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(tv.tv_sec * 1000 
                + tv.tv_usec / 1000); // Milisecond

    for(int i=0; i<td->n; i++) {
        int len = (random() % max_array_len); // Random length
        if(len==0) 
            len = 10; // Default length
        
        array *arr = array_create(len);
        for(int j=0; j<len; j++) {
            arr->a[j] = (random() % max_array_value); // Random value
        }
        td->arrs[i] = arr;
    }
}

void test_data_destroy(test_data *td) {
    for(int i=0; i<td->n; i++) {
        array *arr = td->arrs[i];
        if(arr)
            array_destroy(arr);
    }
    free(td);
}


