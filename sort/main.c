#include <stdio.h>
#include <stdlib.h>
#include "bubble.c"

int arr1[] = {5, 4, 3, 2, 1};
//int arr1[] = {10, 1, 8, 3, 7};
int len1 = sizeof(arr1) / sizeof(int);

int main(int argc, const char *argv[])
{
    printf("unsorted:\n");
    print_array(arr1, len1);
    bidirect_bubble_sort(arr1, len1);
    printf("sorted:\n");
    print_array(arr1, len1);
    return 0;
}
