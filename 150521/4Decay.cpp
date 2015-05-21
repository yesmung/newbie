#if 0
// Decay
#include <iostream>
using namespace std;

void init_int(int a) { a = 0; }
void init_arr(int a[]) { a[0] = 0; }

void main()
{
	int i = 10;
	int a[10] = { 20 }; // int[1] 타입 : 배열에도 타입이 존재함

	init_int(i);			// init_int(10);
	cout << i << endl;

	// 포인터 변수의 선언 : 대상체의 타입 * 변수명
	int* pInt = &i;

	getchar();

	init_arr(a);			// init_int(0x12FF60)
	cout << a[0] << endl;

	// int* pArr[1] = a;		// ERROR, 상수에다가 상수를 담을 수 없다. 즉, *가 먼저 해석되도록 바꿔야 한다.
	// int(*pArr)[1] = a;		// ERROR,
	int* pArr = a;				// decay : 배열의 타입이 배열 전체에서 첫 번째 원소의 타입으로 축소 또는 퇴화된 것을 의미

	cout << sizeof a << endl;   // 배열의 이름은 첫번째 원소를 가리키는 포인터, 즉, 4바이트가 나와야 하지만, sizeof 연산자는 decay가 발생하지 않는다.
	cout << sizeof pArr << endl;
}
#endif

#if 0
// Argument decay (template arg에 decay가 발생한다.)
#include <iostream>
using namespace std;

// 값(T)을 인자로 받을 경우 : 배열과 함수는 decay가 발생
template<typename T> void type_check_val(T t)
{
	cout << typeid(T).name() << endl;
}

// 참조(T&)를 인자로 받을 경우, 정확한 타입으로 전달됨 
template<typename T> void type_check_ref(T& t)
{
	cout << typeid(t).name() << endl;
}

void main()
{
	int i = 0;
	type_check_val(i); // ?
	type_check_val<int>(i); // ?
	type_check_ref(i);	// ?
	
	int arr[3];
	type_check_val(arr);	// int *
	type_check_ref(arr);	// int [3]

	// char * s = " hello"  : 상수 포인터, 즉, 이런 타입을 함수 등으로 받을 때에는,const를 항상 붙여야함
	// char a[] = "hello" : 배열
	type_check_val("hello");	// const char *
	type_check_ref("hello");	// const char [6]

	type_check_val(main);	// 함수 포인터
	type_check_ref(main);	// 정확히 함수 타입으로 봄 (함수주소 & 붙여야 얻을 수 있다)

	//int
	//int
	//int
	//int *
	//int[3]
	//char const *
	//char const[6]
	//int(__cdecl*)(void)
	//int __cdecl(void)
}
#endif

#if 1
#include <stdio.h>

template<typename T> void foo(T a, T b) { printf("called foo(T)\n"); }
template<typename T> void goo(T& a, T& b) { printf("called goo(T&)\n"); }
void goo(const char* a, const char *b) { printf("called goo(char*)\n"); }

// 컴파일러가 일반 함수와 함수 템플릿이 오버로딩 된 경우, 찾는 순서
// 1. 일반 함수
// 2. 함수 템플릿

void main()
{
	foo("kkk", "hhhh"); // void foo(const char*, const char*) //signature
    goo("kkk", "hhhh"); // void goo(const 
	goo("kkk111", "hhhh222222"); // void goo(const 
}
#endif