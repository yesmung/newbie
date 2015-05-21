#include <iostream>
using namespace std;

// RTTI : RunTime Type Information
// 실행 시간에 포인터 변수가 어떤 타입인지를 알아내는 기술
// 조건 : 클래스가 최소한 하나 이상의 가상 함수를 포함하고 있어야 함

#if 0
class Animal
{
public :
	virtual ~Animal() {}
};

class Dog : public Animal
{
public :
	virtual ~Dog() {}
};

int main()
{
	Animal* p1 = new Animal; // 런타임 문법

	// RTTI를 사용하기 위해 컴파일러가 typeid라는 연산자를 제공한다.
	const type_info& t1 = typeid(p1); // typeid(변수명); // typeid는 const를 리턴, 왜냐하면, const가 아닐 경우 vtable 위에 저장된 type 정보를 수정할 위험이 있기 때문이다.
	cout << t1.name() << endl;

	Dog* p2 = new Dog;
	const type_info& t2 = typeid(p2);
	cout << t2.name() << endl;

	// 상속 관계
	// Animal* p3 = static_cast<Animal*>new Dog; // upcasting
	Animal* p3 = new Dog; // 상속 관계에 있는 경우 캐스팅은 생략 가능
	const type_info& t3 = typeid(p3);
	cout << t3.name() << endl;

	Dog* pDog = p3; // 컴파일러는 p3가 Dog인지 알 수 없다. 왜냐하면 상속 관계에 있으므로, 모든 타입은 Animal에 담길 수 있기 때문이다.
					// 이것은 downcasting .. 부모의 타입을 자식의 타입으로 끌어 내림, dog가 animal보다 타입이 크다.
	// pDog = static_cast<Dog*>(p3); // 원래는 되면 안되나, 컴파일러가 이미 관계를 알고 있기에, 자동으로 casting 해줌.
}
#endif

class Animal
{
public:
	virtual ~Animal() {}
};

class Dog : public Animal
{
public:
	virtual ~Dog() {}
};

int main()
{
	Animal* p1 = new Animal; // 런타임 문법
	const type_info& t1 = typeid(p1);
	cout << t1.name() << endl; // Animal*

	Animal a;
	const type_info& t2 = typeid(a); // typeid의 피연산자는 포인터 변수와 일반 객체 변수도 사용 가능(virtual function 있는 객체이어야함)
	cout << t2.name() << endl;

	Animal* pAnimal = new Dog;
	const type_info& t3 = typeid(*pAnimal); // 역참조
	// cout << t3.name() << endl; // Dog
	const type_info& t4 = typeid(Dog); // 타입
	// cout << t4.name() << endl; // Dog

	// if (strcmp(t3.name(), t4.name())) 문자열을 비교할 필요 없다. == 오버로딩 되어 있음
	if (t3 == t4)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	if (typeid(*pAnimal) == typeid(Dog))
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	// 위의 것을 dynamic_cast 로 가능, c++ 제공
	// dynamic_cast를 사용하면 간단하게 안전한 다운캐스팅을 할 수 있다.
	// dynamic_cast를 사용할 경우, 반드시 리턴 타입을 조사해서 사용해야 합니다.
	// 다운캐스팅에 실패한 경우, 널이 반환되기 때문입니다.
	Dog* pDog = dynamic_cast<Dog*>(pAnimal);
	if (pDog)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;
}