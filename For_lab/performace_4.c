#include <stdio.h>

int partition(int arr[], int start, int end) {
    int pvt = arr[start];
    int i = start;
    int j = end;
    int temp;

    while (i < j) {
        while (arr[i] <= pvt && i < end) {
            i++;
        }

        while (arr[j] > pvt && j > start) {
            j--;
        }

        if (i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    arr[start] = arr[j];
    arr[j] = pvt;

    return j;
}

void quicksort(int arr[], int start, int end) {
    if (start < end) {
        int loc = partition(arr, start, end);
        quicksort(arr, start, loc - 1);
        quicksort(arr, loc + 1, end);
    }
}

int main() {
    int n;
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    quicksort(arr, 0, n - 1);
    printf("sorted array: ");

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}