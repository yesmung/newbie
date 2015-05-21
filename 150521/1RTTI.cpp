// RTTI�� ���� ���
// 1. Ŭ������ �̸��� ����
// 2. Ŭ������ �̸��� �б�
#if 0
#include <iostream>
#include <string>
using namespace std;


class Animal
{
public :
	// �̸��� �����ϱ� ���� ���� ���� ���
	static const string descriptor;

	// �̸��� �������� ���� �������̽� ����
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

// RTTI�� �ڵ带 �ڵ�ȭ�ϱ� ���Ͽ� ��ũ�θ� �����մϴ�.
// RTTI�� ������ �ڵ�, ��ũ���� �������ڴ� ��ū�� ���ڿ�ȭ�Ѵ�.

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