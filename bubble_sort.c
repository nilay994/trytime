#include <stdio.h> 
#include<stdlib.h>  
#include<time.h>  
 
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort2(int arr[], int n)
{
    int i, j, comp_cnt, swap_cnt;
    comp_cnt = 0;
    swap_cnt = 0;

    for (i = 0; i < n - 1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++) {
            comp_cnt = comp_cnt + 1;
            if (arr[j] > arr[j + 1]) {
                swap_cnt++;
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    printf("cnt: %d, swap cnt: %d\n", comp_cnt, swap_cnt);
}
 
void bubbleSort1(int arr[], int n)
{
    // Loop below based on the premise that the largest
    // will be pushed to the end in the first iteration itself
    // Hence in the next loop, skip the last element
    // In the one after, skip the last two elements, and so on...
    int comp_cnt = 0;
    int swap_cnt = 0;
    int iter = 0;
    while (1) {
        int i = 0;
        int swapped = 0;
        for (i = 0; i < (n - 1 - iter); i++) {
            comp_cnt++;
            if (arr[i] > arr[i+1]) {
                swap_cnt++;
                swap(&arr[i], &arr[i+1]);
                swapped = 1;
            }
        }
        if (swapped == 0) {
            printf("cnt: %d, swap_cnt: %d\n", comp_cnt, swap_cnt);
            break;
        }
        iter = iter + 1; 
    }
}

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int arr[100], arr2[100];
    for (int ds = 0; ds < 100; ds++) {
        arr[ds] = rand() % (100) + 1;
        arr2[ds] = arr[ds];
    }
    int n = 100;
    bubbleSort2(arr, n);
    printf("Sorted array: \n");
    printArray(arr, n);

    bubbleSort1(arr2, n);
    printf("Sorted array: \n");
    printArray(arr2, n);
    return 0;
}