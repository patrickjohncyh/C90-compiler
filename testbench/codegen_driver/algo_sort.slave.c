/* Code is based on code from : "https://www.geeksforgeeks.org/top-algorithms-and-data-structures-for-competitive-programming/" */

int binarySearch(int arr[], int l, int r, int x){
   if (r >= l){
        int mid = l + (r - l)/2;
        if (arr[mid] == x)  return mid;
        if (arr[mid] > x) return binarySearch(arr, l, mid-1, x);
        return binarySearch(arr, mid+1, r, x);
   }
    return -1;
}

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}
 
int partition (int arr[], int low, int high){
    int pivot = arr[high];   
    int i = (low - 1); 
    int j;
    for ( j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high){
    int pi;
    if (low < high)
    {
        pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
 
