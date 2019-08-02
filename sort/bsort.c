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

/* Bidirectional bubble sort
 */
void bbsort(int arr[], int start, int end) {
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

        for(int i=high; i>low; i--) {
            cnt_inc_comp();
            if(arr[i-1]>arr[i]) {
                swap(arr, i-1, i);
            }
        }
        low++;
    }
}


