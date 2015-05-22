
#if 0
// �ζ��ΰ� ����
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

	// �Լ� �����͸� ����� ���
	// ����: �޸� ����ȭ
	// ����: ������ ������
	display(arr, 10);
	sort(arr, arr + 10, asc_int);
	display(arr, 10);
	sort(arr, arr + 10, des_int);

	// �Լ� ��ü�� ����� ���
	// ����: ������ ������
	// ����: �޸� ȿ������ ������
	less<int> l;
	greater<int> g;

	display(arr, 10);
	sort(arr, arr + 10, l);
	display(arr, 10);
	sort(arr, arr + 10, g);
	display(arr, 10);
}