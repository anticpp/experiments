#include <assert.h>

void cnt_inc_comp();

void mqsort(int arr[], int start, int end) {
    int i = start;
    int j = end;
    int sentinel = arr[j];
    while (i<j) {
        while(i<j && arr[i]<=sentinel) { i++; cnt_inc_comp(); }
        arr[j] = arr[i];

        while(i<j && arr[j]>=sentinel) { j--; cnt_inc_comp(); }
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

