/* Select sort
 */
void ssort(int arr[], int start, int end) {
    for(int i=start; i<end; i++) {
        // Find `min`
        int min = i;
        for(int j=i+1; j<=end; j++) {
            cnt_inc_comp();
            if(arr[j]<arr[min]) {
                min = j;
            }
        }

        // Swap
        if(min!=i) {
            swap(arr, i, min);
        }
    }
}
