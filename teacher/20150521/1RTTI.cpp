
#if 0
// RTTI�� ���� ��� 1. Ŭ������ �̸��� ����
//                  2. Ŭ������ �̸��� �б�

#include <iostream>
#include <string>
using namespace std;

class Animal
{
public:
	virtual ~Animal() {}

	// �̸��� �����ϱ� ���� ���� ���� ���
	static const string descriptor;

	// �̸��� �������� ���� �������̽� ����
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

	// �Ʒ��� �ڵ尡 ������ �� �ֵ��� �ڵ带 ������ ������ ;D
	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "pAnimal is Dog" << endl;
}
#endif



// RTTI�� �ڵ带 �ڵ�ȭ�ϱ� ���Ͽ� ��ũ�θ� �����մϴ�.

#include <iostream>
#include <string>
using namespace std;

#define String16	string

// RTTI�� ����� �ڵ�
#define DECLARE_META_INTERFACE(INTERFACE)				\
	 static const String16 descriptor;					\
	virtual const String16& getInterfaceDescriptor();

// RTTI�� ������ �ڵ�
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

	// �Ʒ��� �ڵ尡 ������ �� �ֵ��� �ڵ带 ������ ������ ;D
	Animal* p2 = new Dog;
	if (p2->getInterfaceDescriptor() == Dog::descriptor)
		cout << "p2 is Dog" << endl;
}