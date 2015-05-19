#if 0
#include <iostream>
using namespace std;

struct Less
{
	inline bool operator()(int a, int b) { return a > b; }
};

struct Greater
{
	inline bool operator()(int a, int b) { return a < b; }
};


// inline bool ACompare(int a, int b) { return a > b; }
// inline bool DCompare(int a, int b) { return a < b; }

// void Sort(int* arr, int len, bool(*Compare)(int, int))
void Sort(int* arr, int len, Less Compare)
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
// void Sort(int* arr, int len, bool(*Compare)(int, int))
void Sort(int* arr, int len, Greater Compare)
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
using namespace std;

struct Less
{
	inline bool operator()(int a, int b) { return a > b; }
};

struct Greater
{
	inline bool operator()(int a, int b) { return a < b; }
};


// inline bool ACompare(int a, int b) { return a > b; }
// inline bool DCompare(int a, int b) { return a < b; }

template<typename T> void Sort(int* arr, int len, T Compare)
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
#include <functional>	// function object : greater, lee 있음

using namespace std;

void display(int* arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", arr[i]);
	}
	getchar();
}

bool asc_int(int a, int b) { return (a < b); }
bool des_int(int a, int b) { return (a > b); }

void main()
{
	int arr[] = { 1, 3, 4, 5, 6, 2, 10, 9, 8, 7 };
	
	// 함수 포인터를 사용할 경우
	// 장점 : 메모리 최적화
	// 단점 : 성능이 떨어짐
	display(arr, 10);

	sort(arr, arr + 10, asc_int);
	display(arr, 10);

	sort(arr, arr + 10, des_int);
	display(arr, 10);

	// 함수 객체를 사용할 경우
	// 장점 : 성능이 좋아짐
	// 단점 : 메모리 효율성이 떨어짐
	less<int> l;
	greater<int> g;

	sort(arr, arr + 10, l);
	display(arr, 10);
	sort(arr, arr + 10, g);
	display(arr, 10);
}