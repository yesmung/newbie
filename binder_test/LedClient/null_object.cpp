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
		this ? onAsFoo() : 0; // ���α׷��� ���� �ʴ´�. 
	}
	// a = 0; // �̷� ����, this->a = 0 �̱� �����̴�.
};

void main()
{
	Clazz* p = 0;
	p->foo(); // �̰� ��! ��, ���� ������ ��, �״°� �ƴϰ�, ����� �����Ϸ��� ��, �״´�.
	
	// null ��ü�� ȣ���ϴ� ���� ǥ���� �ƴϳ�,
	// �����Ϸ��� ������
	// ����, �����ڴ� �̸� ��ȸ�ϴ� �ڵ带 �ۼ��ؾ���
}
