
void swap(int arr[], int x, int y) {
    int tmp = arr[x];
    arr[x] = arr[y];
    arr[y] = tmp;
}

void print_array(int arr[], int len) {
    printf("Array [");
    for(int i=0; i<len; i++) {
        if(i==0) { 
            printf("%d", arr[i]);
        } else {
            printf(", %d", arr[i]);
        }
    }
    printf("]\n");
}
