// �Լ� ��ü : �Լ�ó�� �����ϴ� ��ü (����ڴ� ����θ� ���� ������ ��ü���� �Լ����� �� �� ����)
#include <iostream>
using namespace std;

struct Adder
{
	int operator()(int a, int b) { return a + b; }
};

struct Subber
{
	int operator()(int a, int b) { return a - b; }
};

void main()
{
	Adder add;
	cout << add(1, 2) << endl; // add.operator()(1, 2);
	
	Subber sub;
	cout << sub(2, 1) << endl;
}

// Adder�� Subber�� ���� Ÿ���ϱ�? �ٸ� Ÿ���ϱ�?
// �Լ��� �ñ״�ó�� ������, �Լ� �����Ϳ� ��� �� �ִ�. ��ΰ� ���� Ÿ�Դϴ�. 
// �׷��ٸ�, Adder�� Subber�� ���� Ÿ��? No !
// �ֳ��ϸ�, Adder�� Subber�� �Լ��� �ƴϰ� ��ü�̱� �����̴�.
// ����, ����ü �±װ� �ٸ��� �ٸ� Ÿ������ �з��Ѵ�.

// Smart pointer
// ���� ����
// ���� ����
// ���� ���� <- UniqePtr
// ������ ���� auto_ptr
// ������� <- sp(stroing ptr), weak_ptr, shared_ptr