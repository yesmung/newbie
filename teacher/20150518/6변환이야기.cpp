
#if 1 
// ��ȯ�� ���輺
#include <iostream>
using namespace std;

class Complex
{
public:
	int r, i;
	Complex(int a = 0, int b = 0) : r(a), i(b) {}
	
	// � Ŭ������ ��ȯ �����ڸ� ����Ұ��̶�� ������ ��ȯ ������ ���ٴ�
	// ��ȯ �������̽��� �����ϴ� ���� �� ���� �� �ִ�.
	explicit operator int() { return r; }
	// int asReal() { return r; }
};

void foo(int i) { cout << "called foo(" << i << ")" << endl; }

int main()
{
	double d = 3.14;
	int i = (int)d;	// standard converision(�⺻ ��ȯ)
	// cout << i << endl;
	foo(i);	// OK

	Complex c(1, 1);	// 1 + 1i;
	int real = c;		// int(c); -> c.operator int();
	// int real = c.asReal();
	// cout << real << endl;
	foo(c);	// ????
}

#endif


#if 0
// ��ȯ�� ���輺 2
#include <iostream>
using namespace std;

class Int32
{
public:
	int val;

	// ��ȯ �����ڴ� ���ڰ� �ϳ��� �����ڴ� ��� ��ȯ �����ڰ� �� �� �ֽ��ϴ�.
	// ��ȯ �����ڴ� �����Ϸ��� ���� ȣ��� �� �ֽ��ϴ�. (�Ͻ��� ��ȯ)
	// ���ڰ� �ϳ��� �����ڸ� ��ȯ �����ڷμ� ����� �����Ϸ��� explicit
	explicit Int32(int i = 0) : val(i) {}
};

void foo(Int32 i) { cout << "called foo(" << i.val << ")" << endl; }

void main()
{
	Int32 i(20);
	// cout << i.val << endl;
	foo(i);	// OK

	int n = 30;
	foo(n);	// foo(Int32(n));
}
#endif