/* Bubble sort
 */
void bsort(int arr[], int start, int end) {
    int low = start;
    int high = end;
    while(low<high) {
        for(int i=low; i<high; i++) {
            cnt_inc_comp();
            if(arr[i]>arr[i+1]) {
				swap(arr, i, i+1);
            }
        }
        high--;
    }
}


