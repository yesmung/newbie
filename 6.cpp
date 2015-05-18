#if 0
// ��ȯ�� ���輺
#include <iostream>
using namespace std;

class Complex
{
public :
	int r, i;
	Complex(int a = 0, int b = 0) : r(a), i(b) {}

	// � Ŭ������ ��ȯ �����ڸ� ����� ���̶��, ������ ��ȯ ������ ���ٴ�
	// ��ȯ �������̽��� �����ϴ� ���� �� ���� �� �ִ�.
	// 	operator int() { return r; }
	int asReal() { return r;  }
};

void foo(int i) { cout << "called foo(" << i << ")" << endl; }

int main()
{
	double d = 3.14;
	int i = d; // standard conversion(�⺻ ��ȯ)
	// cout << i << endl;
	foo(i); // OK

	Complex c(1, 1);		// 1 + 1i;
	//int real = (int)c;		// int(c); -> c.operator int(); ������ �����ε�
	int real = c.asReal();
	// cout << real << endl;

	// foo(c); // ??? �̰��� ���״�. ���������� ���������, �ǵ����� �ʾҴ�.
	// �Ʒ��� ���� asReal �̶�� �������̽��� �����ϰ� �������. C++11�� solution �� �ִ�.
	foo(c.asReal());
}
#endif

// ��ȯ�� ���輺2
#include <iostream>
using namespace std;

class Int32
{
public :
	int val;

	// ��ȯ �����ڴ� ���ڰ� �ϳ��� �����ڴ� ��� ��ȯ �����ڰ� �� �� �ִ�.
	// ��ȯ �����ڴ� �����Ϸ��� ���� ȣ��� �� �ִ�. (�Ͻ��� ��ȯ)
	// ���ڰ� �ϳ��� �����ڸ� ��ȯ �����ڷμ� ����� �����Ϸ���, explicit Ű���带 ����Ѵ�.

	// Int32(int i = 0) : val(i) {}
	explicit Int32(int i = 0) : val(i) {}
};

void foo(Int32 i) 
{
	cout << "called foo(" << i.val << ")" << endl; 
}

void main()
{
	Int32 i(20);
	cout << i.val << endl;
	foo(i); // OK

	int n = 30;
	foo(n); // ? �̰��� ���״�... �Ǹ� �ȵȴ�... foo(Int32(n)) �Ǵ� ���̴�. �ǵ��� ���� �ƴϴ�. �׷��ٸ� ��� �� ������ �ذ��� �� �ִ°�? => explicit Ű���带 ���� �ذ��� �����ϴ�.
}