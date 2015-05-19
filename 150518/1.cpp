#include <iostream>
using namespace std;

// �Լ� �����ε�

// �Լ� �����ε��� ���� : C++ �����Ϸ��� �Լ��� �̸��� ����(name mangling)�ϱ� ����
// �̸��� �����ϴ� ��� : �Լ��� �̸� + �Ķ���� ����
// name mangling : C/C++ ���̺귯�� ������ �⺻

// ������� �ڵ� ���� ���
// windows : cl.exe ���ϸ�.cpp -FAs (/FAs)	-> ���ϸ�.asm
// linux : g++ -S ���ϸ�.cpp					-> ���ϸ�.s

int square(int x) { return x*x; }
double square(double x) { return x*x; }

void main()
{
	int a = square(3);
	double b = square(3.3);

	cout << a << endl;
	cout << b << endl;
}