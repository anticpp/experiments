
void cnt_inc_comp();

/*
 * Insert sort
 */
void isort(int arr[], int start, int end) {
    for(int i=start+1; i<=end; i++) {
        int j = start;
        int ii = i; // Original `i` can't be modified. So we keep a copy for `ii`
        int val = arr[i];
        for(; j<ii; j++) {
            cnt_inc_comp();
            if(val<arr[j]) {
                break;
            }
        }
        // Move backward
        for(;j<ii; ii--)
            arr[ii] = arr[i-1];

        // Set value
        arr[j] = val;
    }
}
