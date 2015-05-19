#if 0
// 임시 객체 이야기
// 언제 생성되고, 언제 사라지는지 정확히 알아야 한다.
#include <iostream>
using namespace std;

class Int32
{
public :
	int val;

	Int32(int v = 0) : val(v) 
	{ 
		// cout << "Int32()" << endl; 
		printf("Int32(%d) - %p\n", v, this);
	}
	~Int32() 
	{ 
		// cout << "~Int32()" << endl; 
		printf("~Int32(%d) - %p\n", val, this);
	}

	Int32(const Int32& o) : val(o.val)
	{
		printf("Int32(Int32(%d)) - %p\n", val, this);
	}

	Int32 operator+(const Int32& o)
	{
		cout << "In operator()" << endl;
		// 이런 구현이면, temp라는 개체가 생성되면서, 복사 생성자가 호출된다.
		// Int32 temp(val + o.val);
		// return temp;

		// RVO (Return Value Optimization)
		// 객체가 하나 더 생성되는 것을 막을 수 있다.
		// 함수 내에서 단순한 작업(ex 리턴)이면, 이렇게 임시 객체를 사용하자.
		// ex. Smart pointer 에서 활용 가능하다. (strong pointer, weak pointer)
		return Int32(val + o.val);
	}
};

void main()
{
	Int32 a = 10;
	Int32 b = 20;
	Int32 c;

	getchar();
	c = a + b; // c = a.operator+(b);
	getchar();

	cout << c.val << endl;
}

//void main()
//{
	// Int32();
	// getchar();

	//int a = 10;
	//int b = 20;
	//int c;
	//c = a + b;  // 덧셈이 가능한 것은 이미 a, b가 스택에 잡혔다는 의미
	//			// a+b의 결과(리턴값)는 어디에 저장되는가?
	//cout << c << endl;
//}
#endif

// 임시 객체와 참조 반환
#include <iostream>
using namespace std;

struct Int32
{
	int val;
};

Int32 i = { 10 };
Int32 getInt32A() { return i; }
Int32& getInt32B() { return i;  }

int main()
{
	getInt32A().val = 0; // 임시객체이다. 전역변수 i와는 다른 객체이다.
	getInt32B().val = 20;
	cout << i.val << endl; // ?
	return 0;
}