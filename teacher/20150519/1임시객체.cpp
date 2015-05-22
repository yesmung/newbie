
#if 0
// 임시 객체 이야기
#include <iostream>
using namespace std;

class Int32
{
public:
	int val;

	Int32(int v = 0) : val(v) { printf("Int32(%d): %p\n", v, this); }
	~Int32() { printf("~Int32(%d): %p\n", val, this); }

	Int32(const Int32& o) : val(o.val) {
		printf("Int32(Int32(%d)): %p\n", val, this);
	}

	Int32 operator+(const Int32& o) {
		//Int32 temp(val + o.val);
		//return temp;
		return Int32(val + o.val);	// RVO(Return Value Optimazation
	}
};

void main()
{
	Int32 a = 10;
	Int32 b = 20;
	Int32 c;

	getchar();
	c = a + b;	// c = a.operator+(b);
	getchar();

	cout << c.val << endl;
}


//void main()
//{
//	//Int32();
//	//getchar();
//
//	int a = 10;
//	int b = 20;
//	int c;
//
//	c = a + b;
//	cout << c << endl;
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

Int32  getInt32A() { return i; }
Int32& getInt32B() { return i; }

int main()
{
	getInt32A().val = 0;
	cout << i.val << endl;	// ?

	getchar();

	getInt32B().val = 0;
	cout << i.val << endl;
}