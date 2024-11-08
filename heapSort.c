#include <stdio.h>
#include <stdlib.h>



void heapify(int A[], int d, int i)
{
    // Given the specific parent index extract the associated children indices
    int largest_parent = i;
    int left_child = 2*i + 1;
    int right_child = 2*i + 2;
    
    // If the left child index is not out of array boundaries and the associated left child element is larger than the parent element make it largest
    if(left_child < d && A[left_child] > A[i])
    {
        largest_parent = left_child;
    }

    // If the left child index is not out of array boundaries and the associated left child element is larger than the previous largest
    // element make it largest
    if(right_child < d && A[right_child] > A[largest_parent])
    {
        largest_parent = right_child;
    }

    if(largest_parent != i) // If any swapping occured, recursively perfrom heapifications begining from the swapped child index
    {
        int temp = A[i];
        A[i] = A[largest_parent];
        A[largest_parent] = temp;

        // Recursive heapification call
        heapify(A, d, largest_parent);
    }
}

void printArray(int A[], int d)
{
    printf("Array: ");
    for(int i = 0; i<d; i++)
    {
        if(i == d-1) printf("%d", A[i]);
        else printf("%d, ", A[i]);
    }
    printf("\n");
}


void heapSort(int A[], int d)
{
    // Begin by heapifying the entire array
    for(int i = d/2 -1; i >= 0; i--)
    {
        heapify(A, d, i);
    }

    // Then the associated tree Root gets swapped with the last array element and heapification is repeated for the reduced heap
     for(int i = d-1; i>0; i--)
     {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;

        // Reheapification of reduced heap 
        heapify(A, i, 0);
     }
}





int main()
{
    int A[2] = {1, 2};
    heapSort(A, 2);
    printArray(A, 2);
    return 0;
}