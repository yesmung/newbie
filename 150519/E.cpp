// 함수 객체 : 함수처럼 동작하는 객체 (사용자는 선언부를 보기 전까지 객체인지 함수인지 알 수 없다)
#include <iostream>
using namespace std;

struct Adder
{
	int operator()(int a, int b) { return a + b; }
};

struct Subber
{
	int operator()(int a, int b) { return a - b; }
};

void main()
{
	Adder add;
	cout << add(1, 2) << endl; // add.operator()(1, 2);
	
	Subber sub;
	cout << sub(2, 1) << endl;
}

// Adder와 Subber는 같은 타입일까? 다른 타입일까?
// 함수의 시그니처가 같으면, 함수 포인터에 담길 수 있다. 모두가 같은 타입니다. 
// 그렇다면, Adder와 Subber는 같은 타입? No !
// 왜냐하면, Adder와 Subber는 함수가 아니고 객체이기 때문이다.
// 또한, 구조체 태그가 다르면 다른 타입으로 분류한다.

// Smart pointer
// 얕은 복사
// 깊은 복사
// 복사 금지 <- UniqePtr
// 소유권 이전 auto_ptr
// 참조계수 <- sp(stroing ptr), weak_ptr, shared_ptr