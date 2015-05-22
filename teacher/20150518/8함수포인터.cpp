
// 멤버 함수 포인터 이야기
#include <iostream>
using namespace std;

void func() { printf("calle func()\n"); }

class Clazz
{
public:
	void member_func()	// void member_func(Clazz* const this)
	{
		printf("called member_func()\n");
	}
	
	static void static_func() { printf("called static_func()\n"); }
};

int main()
{
	void(*fp1)() = func;
	void(*fp2)() = &Clazz::static_func;

	// 멤버 함수 포인터의 선언 방법
	// 리턴타입(클래스명::*변수명)([매개변수,...]);
	void(Clazz::*fp3)() = &Clazz::member_func;	// 멤버 함수는 암시적으로 포인터로 해석되지 않기
												// 때문에 명시적으로 &연산자를 사용해야 합니다.

	// 멤버 함수 포인터를 사용하여 함수를 호출할 경우: .*, ->* 연산자를 사용
	// .*: 왼쪽의 피연산자가 객체인 경우
	// ->*: 왼쪽의 피연산자가 포인터인 경우
	Clazz obj;
	// obj.fp3();
	// fp3(&obj);	push 0x12ff60
	(obj.*fp3)();

	Clazz* pObj = new Clazz;
	(pObj->*fp3)();
}



//int main()
//{
//	// 포인터 변수의 선언 방법: 타입 * 변수명;
//	// 함수 포인터 변수의 선언은 함수의 타입이 아닌 시그니처를 사용해서 선언해야 합니다.
//	// void * fp();	// 함수를 선언
//
//	// 1. 함수 포인터 변수의 선언: 리턴타입(*변수명)([매개변수, ...]);
//	void(*fp)();
//
//	// 2. 함수의 이름이 함수의 포인터로 해석됨
//	fp = func;
//
//	// 3. 함수 호출 연산자를 사용
//	fp();
//
//}