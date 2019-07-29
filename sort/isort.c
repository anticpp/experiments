/* Insert sort
 */
void isort(int arr[], int start, int end) {
    for(int i=start+1; i<=end; i++) {
        int v = arr[i];
        int j = i-1;
        for(; j>=start; j--) {
            cnt_inc_comp();
            if(v<arr[j]) {
                arr[j+1] = arr[j];
            } else {
                break;
            }
        }
        arr[j+1] = v;
    }
}
