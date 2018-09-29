#include "utility.c"

// Bidirectional bubble sort
void bidirect_bubble_sort(int arr[], int len) {
	int low=0;
	int high=len-1;
	while(low<high) {
		for(int i=low; i<high;i++) {
			if(arr[i]>arr[i+1]) {
				swap(arr, i, i+1);
			}
		}
		high--;

		for(int j=high-1; j>low; j--) {
			if(arr[j]<arr[j-1]) {
				swap(arr, j, j-1);
			}
		}
		low++;
	}
}


