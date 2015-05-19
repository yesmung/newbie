#include <iostream>
#include <algorithm>
using namespace std;

#if 0
void ASort(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (arr[i] > arr[j])
				swap(arr[i], arr[j]);
		}
	}
}

void DSort(int* arr, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (arr[i] < arr[j])
				swap(arr[i], arr[j]);
		}
	}
}
#endif

inline bool ACompare(int a, int b) { return a > b; }
inline bool DCompare(int a, int b) { return a < b; }

void Sort(int* arr, int len, bool(*Compare)(int,int))
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			// if (arr[i] > arr[j])
			if (Compare(arr[i], arr[j]))
				swap(arr[i], arr[j]);
		}
	}
}

void display(int* arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", arr[i]);
	}
	getchar();
}

void main()
{
	int arr[] = { 1, 3, 4, 5, 6, 2, 10, 9, 8, 7 };
	display(arr, 10);
	Sort(arr, 10, ACompare);
	display(arr, 10);
	Sort(arr, 10, DCompare);
	display(arr, 10);
}

