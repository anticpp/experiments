

void heap_adjust(int arr[], int i, int end);

/* Heap sort
 */
void hsort(int arr[], int start, int end) {
    // Adjust tree
    for(int i=(end-1)/2; i>=start; i--) {
        heap_adjust(arr, i, end);
    }

    for(;end>0;end--) {
        swap(arr, 0, end);                  // Swap arr[0] to arr[end]
        heap_adjust(arr, 0, end-1);         // Adjust arr[0...end-1]
    }
}

/* Adjust tree i to Max-Heap
 */
void heap_adjust(int arr[], int i, int end) {
    int next;
    for(;;) {
        next = 0;
        if(2*i+1<=end)                      // Left
            next = 2*i+1;
        if(2*i+2<=end 
            && arr[2*i+1]<arr[2*i+2])       // Right
            next = 2*i+2;
        
        if(next==0)                         // arr[i] is a leaf node
            break;

        if(arr[i]>arr[next])                // arr[i] is smallest, instantly stop
            break;

        swap(arr, i, next);
        i = next;
    }
}
