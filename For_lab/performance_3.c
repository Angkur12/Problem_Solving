#include <stdio.h>

void merge(int arr[], int lb, int mid, int ub) {
    int temp[ub - lb + 1];

    int i = lb;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= ub) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= ub) {
        temp[k] = arr[j];
        j++;
        k++;
    }

    for (i = lb, k = 0; i <= ub; i++, k++) {
        arr[i] = temp[k];
    }
}

void mergeSort(int arr[], int lb, int ub) {
    if (lb < ub) {
        int mid = lb + (ub - lb) / 2;
        mergeSort(arr, lb, mid);
        mergeSort(arr, mid + 1, ub);
        merge(arr, lb, mid, ub);
    }
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Array size must be positive.\n");
        return 1;
    }

    int arr[n];
    printf("Enter %d elements:\n", n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    mergeSort(arr, 0, n - 1);
    printf("Sorted array: ");

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}