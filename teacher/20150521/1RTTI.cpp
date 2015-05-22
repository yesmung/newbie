
#if 0
// RTTI의 구현 요소 1. 클래스의 이름을 저장
//                  2. 클래스의 이름을 읽기

#include <iostream>
#include <string>
using namespace std;

class Animal
{
public:
	virtual ~Animal() {}

	// 이름을 저장하기 위해 전역 변수 사용
	static const string descriptor;

	// 이름을 꺼내오기 위한 인터페이스 제공
	virtual const string& getInterfaceDescriptor() {
		return descriptor;
	}
};
const string Animal::descriptor = "Animal";

class Dog : public Animal
{
public:
	static const string descriptor;
	const string& getInterfaceDescriptor() {
		return descriptor;
	}

};
const string Dog::descriptor = "Dog";

int main()
{
	Animal* pAnimal = new Animal;
	//if (typeid(*pAnimal) == typeid(Animal))
	//	cout << "pAnimal is Animal" << endl;

	if (pAnimal->getInterfaceDescriptor() == Animal::descriptor)
		cout << "pAnimal is Animal" << endl;

	// 아래의 코드가 동작할 수 있도록 코드를 구현해 보세요 ;D
	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "pAnimal is Dog" << endl;
}
#endif



// RTTI의 코드를 자동화하기 위하여 매크로를 도입합니다.

#include <iostream>
#include <string>
using namespace std;

#define String16	string

// RTTI의 선언부 코드
#define DECLARE_META_INTERFACE(INTERFACE)				\
	 static const String16 descriptor;					\
	virtual const String16& getInterfaceDescriptor();

// RTTI의 구현부 코드
#define IMPLEMENT_META_INTERFACE(INTERFACE, NAME)					\
	const String16 INTERFACE::descriptor(NAME);			\
	const String16& INTERFACE::getInterfaceDescriptor() { return descriptor; }

class Animal
{
public:
	DECLARE_META_INTERFACE(Animal)
};
IMPLEMENT_META_INTERFACE(Animal, "Animal")


class Dog : public Animal
{
public:
	DECLARE_META_INTERFACE(Dog)
};
IMPLEMENT_META_INTERFACE(Dog, "Animal")


int main()
{
	Animal* pAnimal = new Animal;
	//if (typeid(*pAnimal) == typeid(Animal))
	//	cout << "pAnimal is Animal" << endl;

	if (pAnimal->getInterfaceDescriptor() == Animal::descriptor)
		cout << "pAnimal is Animal" << endl;

	// 아래의 코드가 동작할 수 있도록 코드를 구현해 보세요 ;D
	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "p2 is Dog" << endl;
}