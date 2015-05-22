
#if 0
// Decay
#include <iostream>
using namespace std;

void init_int(int a)   { a = 0; }
void init_arr(int a[]) { a[0] = 0; }

void main()
{
	int i = 10;
	int a[10] = { 20, };	// int[1] 

	init_int(i);	// init_int(10);
	cout << i << endl;

	// 포인터 변수의 선언: 대상체의 타입 * 변수명;
	int* pInt = &i;

	getchar();

	init_arr(a);	// init_int(0x12FF60);
	cout << a[0] << endl;

	// int(* pArr)[1] = a;
	int* pArr = a;	// decay: 배열의 타입이 배열 전체에서 첫 번째 원소의 타입으로 축소 또는 퇴화

	cout << sizeof a << endl;	// sizeof 연산자는 decay가 발생하지 않습니다.
}
#endif


#if 0
// Argument Decay
#include <iostream>
using namespace std;


// 값(T)을 인자로 받을 경우: 배열과 함수는 Decay가 발생
template<typename T> void type_check_val(T t) {
	cout << typeid(t).name() << endl;
}

// 참조(T&)를 인자로 받을 경우: 정확한 타입으로 전달됨
template<typename T> void type_check_ref(T& t) {
	cout << typeid(t).name() << endl;
}


void main()
{
	int i = 0;
	type_check_val(i);	// int
	type_check_ref(i);	// int

	int arr[3];
	type_check_val(arr);	// int *
	type_check_ref(arr);	// int [3]

	type_check_val("hello");	// const char*
	type_check_ref("hello");	// const char[6]

	type_check_val(main);		// 1
	type_check_ref(main);		// 2


}
#endif


#include <stdio.h>

template<typename T> void foo(T a, T b) { printf("called foo(T)\n"); }
template<typename T> void goo(T& a, T& b) { printf("called goo(T&)\n"); }
void goo(const char* a, const char *b) { printf("called goo(char*)\n"); }

// 컴파일러가 일반 함수와 함수 템플릿이 오버로딩 된 경우, 찾는 순서
// 1. 일반 함수
// 2. 함수 템플릿

void main()
{
	foo("kkk", "hhhh");	// void foo(const char*, const char*);

	// 아래의 함수가 호출되도록 코드를 수정해 보세요 ;D
	goo("kkk", "hhhh");
	goo("kkfdasfak", "fadsfasfdsafdasfashhhh");
}