






#if 0
// type traits
#include <iostream>
using namespace std;

template<typename T> T Max(T a, T b)
{
	if (T is pointer)
		return *a > *b ? *a : *b;
	else
		return a > b ? a : b;
}

int main()
{
	int a = 10, b = 20;
	Max(&a, &b);
}
#endif


// type traits: 타입 특질 또는 타입 특성
// 컴파일 타임에 타입의 특성이나 또는 특질을 알아내는 기술

#if 0
// 주 템플릿은 거짓을 반환
// 부분 전문화는 참을 반환

#include <stdio.h>
// is_pointer 클래스 또는 구조체를 구현해 보세여 ;D

// 주 템플릿: 모든 타입은 포인터가 아니라고 가정
template<typename T> struct is_pointer {
	enum { value = false };
};

// 부분 전문화: 특정 타입은 포인터라고 가정
template<typename T> struct is_pointer < T* > {
	enum { value = true };
};

template<typename T> void foo(const T& a)
{
	if (is_pointer<T>::value)
		printf("pointer\n");
	else
		printf("no pointer\n");
}

// 위의 코드는 아래의 코드로 변경됩니다.
// void foo(int a) { printf("no pointer\n"); }
// void foo(int* a) { printf("pointer\n"); }




void main()
{
	int  i; foo(i);	// no pointer
	int* p; foo(p);	// pointer
}

#endif


#if 0
// 연습 문제 ;D
#include <stdio.h>

// is_array 구조체를 구현해보세요 :D
template<typename T> struct is_array {
	enum { size = -1 };
	enum { value = false };
};

template<typename T, int N> struct is_array < T[N] > {
	enum { size = N };
	enum { value = true };
};

// 배열의 길이도 출력해보세여 ;D
template<typename T> void foo(const T& a)
{
	if (is_array<T>::value)
		printf("array: %d\n", is_array<T>::size);
	else
		printf("no array\n");
}

int main()
{
	int i;		foo(i);	// no array
	int a[5];	foo(a);	// array


}

#endif


// type traits 기술은 이미 표준에서 지원하고 있습니다.
#include <iostream>
using namespace std;

int *p;
int arr[3];

int main()
{
	if (is_pointer<int*>::value) cout << "pointer" << endl;
	if (is_array<int[3]>::value) cout << "array" << endl;
}