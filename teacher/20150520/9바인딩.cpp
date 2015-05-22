
#if 0
// ���ε�
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
// ���� �Լ� ���̺��� ����
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


// �߻� Ŭ����
#include <iostream>
using namespace std;

// �߻� Ŭ������: ���� ���� �Լ��� �� �� �̻� ������ �ִ� Ŭ������ �ǹ�
class Parent
{
public:
	// ���� ���� �Լ�: �Լ��� ��ü�� ���ǵ��� ���� �Լ�
	virtual void foo() = 0;
};

// �ڽ��� �θ��� ���� ���� �Լ��� �������� ������ �ڽŵ�
// �߻� Ŭ������ �˴ϴ�.
class Child : public Parent
{
};

void main()
{
	Parent p;	// �߻� Ŭ������ ��ü�� ������ �� ����.
	Child  c;
}
