// type traits(타입특질, 타입특성)
// 컴파일 타임에 타입의 특성이나 또는 특질을 알아내는 기술

// 주 템플릿은 거짓을 반환
// 부분 전문화는 참을 반환

#if 0
// #include <iostream>
#include <stdio.h>

template<typename T> T Max(T a, T b)
{
	return a > b ? a : b;
}

// is_pointer 구현하기
// 주 템플릿 : 모든 타입은 포인터가 아니라고 가정
template<typename T> struct is_pointer
{
	enum { value = false };
};
// 부분 전문화 : 특정 타입은 포인터라고 가정
template<typename T> struct is_pointer<T*>
{
	enum { value = true };
};

// 컴파일러가 절대 타지 않는 코드(dead code, unreachable code)는 최적화에 의해서 지운다.
template<typename T> void foo(const T& a)
{
	if (is_pointer<T>::value)
		printf("pointer\n");
	else
		printf("not pointer\n");
}

// void foo(int a) { printf("no pointer\n"); }
// void foo(int* a) { printf("pointer\n"); }

/*
Animal* p1 = new Animal; // 런타임 문법
const type_info& t1 = typeid(p1);
*/
int main()
{
	int i = 20;
	int* j = &i;
	foo(i);
	foo(j);
}
#endif

#if 0

// 연습 문제 
#include <stdio.h>

// is_array 구조체를 구현해보세요. + 배열의 길이 
template<typename T> struct is_array
{
	enum { value = false, size };
};

// template<typename T> struct is_array<T[1]> // argument decay 발생 nono, foo에서 &(reference)로 받음,
template<typename T, int N> struct is_array<T[N]>
{
	enum { value = true, size = N };
};
template<typename T>void foo(const T& a)
{
	if (is_array<T>::value)
	{
		printf("array\n");
		printf("size = %d\n", is_array<T>::size);
	}
	else
		printf("not array\n");
}

int main()
{
	int i; 
	foo(i); //no array
	int a[1]; // int[1]
	foo(a); //array
	int b[10];
	foo(b);
}
#endif

// type trait 기술은 이미 표준에서 지원하고 있습니다.
#include <iostream>
using namespace std;

int *p;
int arr[3];

int main()
{
	if (is_pointer<int*>::value) cout << "pointer" << endl;
	if (is_array<int[3]>::value) cout << "array" << endl; // std 제공은 array에서 size 구하는 것은 없다.

	// 생성자가 자명한지 등을 알 수 있다.
}