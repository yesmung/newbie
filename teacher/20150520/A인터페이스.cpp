
// 인터페이스 이야기
#include <iostream>
using namespace std;

// public class Dog extends Animal implements crying, eating, sleeping {}

#define interface struct
// 모든 핸드폰이 지켜야 할 규칙을 클래스로 구현합니다.
// C++ 에서는 인터페이스를 식별하기 위한 키워드를 제공하지 않습니다.
// 그래서 접두어로 I(Interface)를 붙이는 관례가 있습니다.
interface IPhone	// 안드로이드는 인터페이스로 클래스를 사용합니다.
{
	// 순수 가상 함수는 자식에게 어떠한 기능을 강제로 구현하도록
	// 하기 위한 문법적인 장치입니다.
	virtual void call(const char*) = 0;
};

// 이제 핸드폰을 설계하는 사람은 Phone을 상속받기로 약속합니다.
class AnyCall : public IPhone
{
public:
	void call(const char* n) { printf("calling %s with Anycall\n", n); }
};

class Cyon : public IPhone
{
public:
	void call(const char* n) { printf("calling %s with Cyon\n", n); }
};

class Person
{
public:
	void use_phone(IPhone* c, const char* n) { c->call(n); }
	// void use_phone(AnyCall* c, const char* n) { c->call(n); }

};

void main()
{
	Person p;

	Cyon c;
	p.use_phone(&c, "000-0000-0000");

	AnyCall a;
	p.use_phone(&a, "000-0000-0000");

}