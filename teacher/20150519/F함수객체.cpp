
#if 0
// 인라인과 성능
#include <iostream>
#include <algorithm>	// for swap
using namespace std;

struct Less { inline bool operator()(int a, int b) { return a > b; } };
struct Greater { inline bool operator()(int a, int b) { return a < b; } };

void Sort(int* arr, int len, Less cmp) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (cmp(arr[i], arr[j]))
				swap(arr[i], arr[j]);
		}
	}
}
void Sort(int* arr, int len, Greater cmp) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (cmp(arr[i], arr[j]))
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

	Less l;
	Sort(arr, 10, l);
	display(arr, 10);

	Greater g;
	Sort(arr, 10, g);
	display(arr, 10);

}

#endif



#if 0
#include <iostream>
#include <algorithm>	// for swap
using namespace std;

struct Less { inline bool operator()(int a, int b) { return a > b; } };
struct Greater { inline bool operator()(int a, int b) { return a < b; } };

template<typename T>
void Sort(int* arr, int len, T cmp) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (cmp(arr[i], arr[j]))
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

	Less l;
	Sort(arr, 10, l);
	display(arr, 10);

	Greater g;
	Sort(arr, 10, g);
	display(arr, 10);

}

#endif

#include <iostream>
#include <algorithm>	// sort
#include <functional>	// function object: greater, less
using namespace std;

void display(int* arr, int len) {
	for (int i = 0; i < len; i++)
		printf("%d ", arr[i]);
	getchar();
}

bool asc_int(int a, int b) { return (a < b); }
bool des_int(int a, int b) { return (a > b); }

void main()
{
	int arr[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };

	// 함수 포인터를 사용할 경우
	// 장점: 메모리 최적화
	// 단점: 성능이 떨어짐
	display(arr, 10);
	sort(arr, arr + 10, asc_int);
	display(arr, 10);
	sort(arr, arr + 10, des_int);

	// 함수 객체를 사용할 경우
	// 장점: 성능이 좋아짐
	// 단점: 메모리 효율성이 떨어짐
	less<int> l;
	greater<int> g;

	display(arr, 10);
	sort(arr, arr + 10, l);
	display(arr, 10);
	sort(arr, arr + 10, g);
	display(arr, 10);
}