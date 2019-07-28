/* Select sort
 */
void ssort(int arr[], int start, int end) {
    for(int i=start; i<end; i++) {
        for(int j=i+1; j<=end; j++) {
            cnt_inc_comp();
            if(arr[i]>arr[j]) {
                swap(arr, i, j);
            }
        }
    }
}
