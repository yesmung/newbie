
#if 0
// 인라인과 성능
#include <iostream>
#include <algorithm>	// for swap
using namespace std;

// 오름 차순
void ASort(int* arr, int len) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (arr[i] > arr[j])
				swap(arr[i], arr[j]);
		}
	}
}

// 내림 차순
void DSort(int* arr, int len) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (arr[i] < arr[j])
				swap(arr[i], arr[j]);
		}
	}
}

void display(int* arr, int len) {
	for (int i = 0; i < len; i++)
		printf("%d ", arr[i]);
	getchar();
}

void main() {
	int arr[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
	display(arr, 10);
	ASort(arr, 10);
	display(arr, 10);
	DSort(arr, 10);
	display(arr, 10);

}
#endif



// 인라인과 성능
#include <iostream>
#include <algorithm>	// for swap
using namespace std;

void Sort(int* arr, int len, bool(*cmp)(int,int)) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if(cmp(arr[i], arr[j]))	// if (arr[i] > arr[j])
				swap(arr[i], arr[j]);
		}
	}
}

inline bool asc_int(int a, int b) { return (a > b); }
inline bool des_int(int a, int b) { return (a < b); }

void display(int* arr, int len) {
	for (int i = 0; i < len; i++)
		printf("%d ", arr[i]);
	getchar();
}

void main() {
	int arr[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
	display(arr, 10);
	Sort(arr, 10, asc_int);
	display(arr, 10);
	Sort(arr, 10, des_int);
	display(arr, 10);

}