// 멤버 함수 포인터 이야기
#include <iostream>
using namespace std;

void func() { printf("called func()"); }

class Clazz
{
public :
	void member_func() { printf("called member_func()\n"); } // void member_func(Clazz* const this)
	static void static_func() { printf("called static_func()\n"); }
};


int main()
{
	void(*fp1)() = func;
	void(*fp2)() = &Clazz::static_func; // Clazz라는 namespace에 있다.
	// & 안붙여도 되지만, 원칙은 붙여야 된다.
	// 클래스 안쪽에 있는 멤버함수이기때문에, 암시적 변환은 안되나
	// c언어와의 하위호환성 때문에 변경되는 것이다.
	
	fp2();

	// 멤버 함수 포인터의 선언 방법
	// void(*fp3)() = &Clazz::member_func; ERROR
	// 리턴타입(클래스명::*변수명)([매개변수,...]);

	void(Clazz::*fp3)() = &Clazz::member_func; // 멤버 함수는 암시적으로 포인터로 해석되지 않기 때문에
												// 명시적으로 &연산자를 사용해야 한다.
	
	// 멤버 함수 포인터를 사용하여 함수를 호출할 경우 : .*, ->* 연산자를 사용
	// .* : 왼쪽의 피연산자가 객체인 경우
	// ->* : 왼쪽의 피연산자가 포인터인 경우
	Clazz obj; // 스택
	// obj.fp3() ERROR
	// fp3(&obj); ERROR, 객체의 주소는 범용레지스터로 들어감
	(obj.*fp3)(); // .* 연산자 제공

	// 피연산자가 포인터인 경우(힙 영역)
	Clazz* pObj = new Clazz;
	// (pObj.*fp3)();
	(pObj->*fp3)();
}

/*
int main()
{
	// 포인터 변수의 선언 방법 : 타입 * 변수명
	// 함수 포인터 변수의 선언은 함수의 타입이 아닌, 시그니처를 사용해서 선언해야 한다.
	// 함수의 시그니처 : 리턴타입 + 매개변수
	// 시그니처 * 변수명

	// void() * fp;
	// void * fp() = func; // ERROR, 함수 포인터를 선언한 것이 아니라, 함수를 선언한 것이다...
	// 컴파일러가 해석할 때, 심볼의 뒤부터 해석하기 때문에, ()를 해석하면, 함수라고 생각하게 된다.
	// 따라서, 아래와 같이 함수 포인터를 선언할 수 있다.
	// 기호의 우선순위 역전을 위해, (*)를 사용
	
	// 1. 함수 포인터 변수의 선언 : 리턴타입(*변수명)([매개변수,...]);
	void(*fp)(); 

	// 2. 함수의 이름이 함수의 포인터로 해석됨
	// call by address : array, function
	fp = func;

	// 3. 함수를 호출하기 위한 연산자를 사용
	fp();
}
*/