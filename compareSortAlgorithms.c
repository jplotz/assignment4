#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Jacob Plotz
 * ja347685
 * Torosdagli
 * CS1
 * Assignment 4 3/21/23
 */

int extraMemoryAllocated;

// helper swap function
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

// helper merge function for mergeSort
void merge(int *finalarray, int finalarraylen, int *arr1, int len1, int *arr2, int len2)
{
    int pos1 = 0;
    int pos2 = 0;
    int smallestpos, smallestval;
    for (int i = 0; i < finalarraylen; i++)
    {
        // simply add the elements of the other array
        if (pos1 >= len1)
        {
            finalarray[i] = arr2[pos2++];
        }
        else if (pos2 >= len2)
        {
            finalarray[i] = arr1[pos1++];
        }
        else if (arr1[pos1] <= arr2[pos2])
        {
            smallestval = arr1[pos1];
            smallestpos = pos1;
            pos1++;
            finalarray[i] = smallestval;
        }
        else if (arr2[pos2] <= arr1[pos1])
        {
            smallestval = arr2[pos2];
            smallestpos = pos2;
            pos2++;
            finalarray[i] = smallestval;
        }
    }
}



// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int left, int right)
{
    if (left == right) // only one element, base case
        return;

    int mid = (left + right) / 2;

    // create left half
    int leftLen = mid-left+1;
    int *leftSide = malloc(sizeof(int) * leftLen);
    extraMemoryAllocated += sizeof(int)*leftLen;

    // create right half
    int rightLen = right-mid;
    int *rightSide = malloc(sizeof(int) * rightLen);
    extraMemoryAllocated += sizeof(int)*rightLen;

    memcpy(leftSide, pData, leftLen*sizeof(int));
    memcpy(rightSide, pData+mid+1, rightLen*sizeof(int));

    mergeSort(leftSide, 0, leftLen-1);
    mergeSort(rightSide, 0, rightLen-1);
    merge(pData, right+1, leftSide, leftLen, rightSide, rightLen);

    free(leftSide);
    free(rightSide);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
    for (int right = 1; right < n; right++)
    {
        for (int left = right-1; left >= 0; left--)
        {
            if (pData[left+1] < pData[left])
            {
                swap(&pData[left+1], &pData[left]);
            }
        }
    }    
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int sorted = 0;
    while (!sorted)
    {
        sorted = 1;
        for (int i = 0; i < n-1; i++)
        {
            if (pData[i] > pData[i+1])
            {
                swap(&pData[i], &pData[i+1]);
                sorted = 0;
            }
        }    
    }
}


// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int minVal, minIdx;
    for (int i = 0; i < n; i++)
    {
        minVal = pData[i];
        minIdx = i;
        for (int j = i+1; j < n; j++)
        {
            if (pData[j] < minVal)
            {
                minVal = pData[j];
                minIdx = j;
            }
        }

        if (minVal != pData[i])
        {
            swap(&pData[i], &pData[minIdx]);
        }
    }    
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
    int n, ret;

	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
        for (int i = 0; i < dataSz; i++)
        {
            ret = fscanf(inFile, "%d", &n);
            (*ppData)[i] = n;
            if (ret == EOF)
                break;
        }
	}

    fclose(inFile);
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
