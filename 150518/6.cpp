#if 0
// 변환의 위험성
#include <iostream>
using namespace std;

class Complex
{
public :
	int r, i;
	Complex(int a = 0, int b = 0) : r(a), i(b) {}

	// 어떤 클래스가 변환 연산자를 사용할 것이라면, 가급적 변환 연산자 보다는
	// 변환 인터페이스를 제공하는 것이 더 나을 수 있다.
	// 	operator int() { return r; }
	int asReal() { return r;  }
};

void foo(int i) { cout << "called foo(" << i << ")" << endl; }

int main()
{
	double d = 3.14;
	int i = d; // standard conversion(기본 변환)
	// cout << i << endl;
	foo(i); // OK

	Complex c(1, 1);		// 1 + 1i;
	//int real = (int)c;		// int(c); -> c.operator int(); 연산자 오버로딩
	int real = c.asReal();
	// cout << real << endl;

	// foo(c); // ??? 이것은 버그다. 정상적으로 수행되지만, 의도하지 않았다.
	// 아래와 같이 asReal 이라는 인터페이스를 정의하고 사용하자. C++11은 solution 이 있다.
	foo(c.asReal());
}
#endif

// 변환의 위험성2
#include <iostream>
using namespace std;

class Int32
{
public :
	int val;

	// 변환 생성자는 인자가 하나인 생성자는 모두 변환 생성자가 될 수 있다.
	// 변환 생성자는 컴파일러에 의해 호출될 수 있다. (암시적 변환)
	// 인자가 하나인 생성자를 변환 생성자로서 사용을 금지하려면, explicit 키워드를 사용한다.

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
	foo(n); // ? 이것은 버그다... 되면 안된다... foo(Int32(n)) 되는 것이다. 의도한 것은 아니다. 그렇다면 어떻게 이 문제를 해결할 수 있는가? => explicit 키워드를 통해 해결이 가능하다.
}