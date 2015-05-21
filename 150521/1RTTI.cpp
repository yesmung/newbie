// RTTI의 구현 요소
// 1. 클래스의 이름을 저장
// 2. 클래스의 이름을 읽기
#if 0
#include <iostream>
#include <string>
using namespace std;


class Animal
{
public :
	// 이름을 저장하기 위해 전역 변수 사용
	static const string descriptor;

	// 이름을 꺼내오기 위한 인터페이스 제공
	virtual const string& getInterfaceDescriptor() {
		return descriptor;
	}

	virtual ~Animal(){}
};
const string Animal::descriptor = "Animal";

class Dog : public Animal
{
public :
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

	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;
}
#endif

// RTTI의 코드를 자동화하기 위하여 매크로를 도입합니다.
// RTTI의 구현부 코드, 매크로의 샵연산자는 토큰을 문자열화한다.

#include <iostream>
#include <string>
using namespace std;


#define DECLARE_META_INTERFACE(INTERFACE)					\
	static const string descriptor;							\
	virtual const string& getInterfaceDescriptor();

#define IMPLEMENT_META_INTERFACE(INTERFACE, NAME)		\
	const string INTERFACE::descriptor(NAME);\
	const string& INTERFACE::getInterfaceDescriptor() { return INTERFACE::descriptor; }

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

	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;
}