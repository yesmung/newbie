
#if 1 
// 변환의 위험성
#include <iostream>
using namespace std;

class Complex
{
public:
	int r, i;
	Complex(int a = 0, int b = 0) : r(a), i(b) {}
	
	// 어떤 클래스가 변환 연산자를 사용할것이라면 가급적 변환 연산자 보다는
	// 변환 인터페이스를 제공하는 것이 더 나을 수 있다.
	explicit operator int() { return r; }
	// int asReal() { return r; }
};

void foo(int i) { cout << "called foo(" << i << ")" << endl; }

int main()
{
	double d = 3.14;
	int i = (int)d;	// standard converision(기본 변환)
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
// 변환의 위험성 2
#include <iostream>
using namespace std;

class Int32
{
public:
	int val;

	// 변환 생성자는 인자가 하나인 생성자는 모두 변환 생성자가 될 수 있습니다.
	// 변환 생성자는 컴파일러에 의해 호출될 수 있습니다. (암시적 변환)
	// 인자가 하나인 생성자를 변환 생성자로서 사용을 금지하려면 explicit
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