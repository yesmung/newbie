// ���ε�

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

	Animal* p2 = new Dog; // �����Ϸ��� Ÿ�Ը� ����. ��, animal Ÿ��.. ��ĳ������ �ص� �θ��� �Լ��� ȣ���
	// �� �̷��� ���踦 �߳�? �⺻���� ��Ŀ������ c�� ����..c�� ���� ���ε��� only... ���� virtual �� ���� ������ ����..
	// �����縶�� �����Լ��� �����ϴ� ����� �ٸ�����
	p2->cry(); // cry(p2)
}
#endif

#if 0
// ���� �Լ� ���̺��� ���� (sizeof ���� �þ~)
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

// �߻� Ŭ����

// �߻� Ŭ������ : ���� ���� �Լ��� �� �� �̻� ������ �ִ� Ŭ������ �ǹ�
// java�� abstract Ű����� ��Ȯ�� �����Ѵ�.
// foo�� ���� �Ǿ� �ֱ� ������, ��ü ������ �ȵȴ�.
class Parent
{
public :
	// ���� ���� �Լ� : �Լ��� ��ü�� ���ǵ��� ���� �Լ�
	virtual void foo() = 0; // pure virtual function(���� ���� �Լ�)
};

// �ڽ��� �θ��� ���� ���� �Լ��� �������� ������ �ڽŵ� �߻� Ŭ������ �˴ϴ�.
class Child : public Parent
{
public : 

};

void main()
{
	Parent p;
}

