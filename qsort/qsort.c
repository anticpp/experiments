#include <stdio.h>
#include <assert.h>

int cnt_comp = 0;

void qsort(int arr[], int start, int end) {
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
        qsort(arr, start, i-1);

    // Right
    if(end-i>0)
        qsort(arr, i+1, end);
}

int main(int argc, const char *argv[]) {
    //int arr[] = {9, 1, 4, 7, 3, 2, 1};
    //int arr[] = {9, 1, 4, 13, 3, 2, 1, 18, 29, 12, 56, 3, 2, 90, 13};
    int arr[] = {9, 1, 4, 13, 3, 2, 1, 18, 29, 12, 56, 3, 2, 90, 12};
    int len = sizeof(arr)/sizeof(int);
    qsort(arr, 0, len-1);
 
    printf("Array N: %d\n", len);
    printf("Compare count: %d\n", cnt_comp);
    for(int i=0; i<len; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

