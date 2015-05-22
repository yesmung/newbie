
#if 0
// 바인딩
#include <iostream>
using namespace std;

class Animal {
public:
	virtual void cry() { // void Animal::cry(Animal* const this)
		cout << "Animal::cry()" << endl;
	}
};

class Dog : public Animal {
public:
	void cry() {	// void Dog::cry(Dog* const this)
		cout << "Dog::cry()" << endl;
	}
};

int main() {
	Animal* p1 = new Animal;
	p1->cry();	// cry(p1);

	getchar();
	Animal* p2 = new Dog;
	p2->cry();	// cry(p2);
}
#endif

#if 0
// 가상 함수 테이블의 존재
#include <iostream>
using namespace std;

class Parent
{
	int val;
public:
	virtual ~Parent() {}
};

void main()
{
	Parent p;
	cout << sizeof p << endl;	// ?
}
#endif


// 추상 클래스
#include <iostream>
using namespace std;

// 추상 클래스란: 순수 가상 함수를 한 개 이상 가지고 있는 클래스를 의미
class Parent
{
public:
	// 순수 가상 함수: 함수의 몸체가 정의되지 않은 함수
	virtual void foo() = 0;
};

// 자식이 부모의 순수 가상 함수를 구현하지 않으면 자신도
// 추상 클래스가 됩니다.
class Child : public Parent
{
};

void main()
{
	Parent p;	// 추상 클래스는 객체를 생성할 수 없다.
	Child  c;
}
