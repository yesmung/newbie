// 바인딩

#include <iostream>
using namespace std;

#if 0
class Animal
{
public :
	// void Animal::cry(Animal* const this)
	virtual void cry() { cout << "Animal::cry()" << endl; }
};

class Dog : public Animal
{
public :
	// void Dog::cry(Dog* const this)
	void cry() { cout << "Dog::cry()" << endl; }
};

int main()
{
	Animal* p1 = new Animal;
	p1->cry(); // cry(p1)

	getchar();

	Animal* p2 = new Dog; // 컴파일러는 타입만 본다. 즉, animal 타입.. 업캐스팅을 해도 부모의 함수가 호출됨
	// 왜 이렇게 설계를 했나? 기본적인 메커니즘이 c를 따라감..c는 정적 바인딩이 only... 따라서 virtual 에 대한 개념이 없어..
	// 제조사마다 가상함수를 구현하는 방법이 다르긴함
	p2->cry(); // cry(p2)
}
#endif

#if 0
// 가상 함수 테이블의 존재 (sizeof 값이 늘어나~)
class Parent
{
	int val;
public :
	virtual ~Parent() {}

};

void main()
{
	Parent p;
	cout << sizeof p << endl;
}
#endif

// 추상 클래스

// 추상 클래스란 : 순수 가상 함수를 한 개 이상 가지고 있는 클래스를 의미
// java는 abstract 키워드로 명확히 구분한다.
// foo는 선언만 되어 있기 때문에, 객체 생성이 안된다.
class Parent
{
public :
	// 순수 가상 함수 : 함수의 몸체가 정의되지 않은 함수
	virtual void foo() = 0; // pure virtual function(순수 가상 함수)
};

// 자식이 부모의 순수 가상 함수를 구현하지 않으면 자신도 추상 클래스가 됩니다.
class Child : public Parent
{
public : 

};

void main()
{
	Parent p;
}

