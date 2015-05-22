#include <iostream>
using namespace std;

class Clazz
{
	int a;
protected :
	void onAsFoo() { cout << "foo" << endl; }
public :
	// void foo() { cout << "foo" << endl; } // void foo(Clazz* const this)
	void foo() {
		this ? onAsFoo() : 0; // 프로그램은 죽지 않는다. 
	}
	// a = 0; // 이럼 죽음, this->a = 0 이기 때문이다.
};

void main()
{
	Clazz* p = 0;
	p->foo(); // 이게 됨! 즉, 널을 참조할 때, 죽는게 아니고, 멤버를 변경하려할 때, 죽는다.
	
	// null 객체를 호출하는 것은 표준은 아니나,
	// 컴파일러는 묵인함
	// 따라서, 개발자는 이를 우회하는 코드를 작성해야함
}
