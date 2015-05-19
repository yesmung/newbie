#if 0
// �ӽ� ��ü �̾߱�
// ���� �����ǰ�, ���� ��������� ��Ȯ�� �˾ƾ� �Ѵ�.
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
		// �̷� �����̸�, temp��� ��ü�� �����Ǹ鼭, ���� �����ڰ� ȣ��ȴ�.
		// Int32 temp(val + o.val);
		// return temp;

		// RVO (Return Value Optimization)
		// ��ü�� �ϳ� �� �����Ǵ� ���� ���� �� �ִ�.
		// �Լ� ������ �ܼ��� �۾�(ex ����)�̸�, �̷��� �ӽ� ��ü�� �������.
		// ex. Smart pointer ���� Ȱ�� �����ϴ�. (strong pointer, weak pointer)
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
	//c = a + b;  // ������ ������ ���� �̹� a, b�� ���ÿ� �����ٴ� �ǹ�
	//			// a+b�� ���(���ϰ�)�� ��� ����Ǵ°�?
	//cout << c << endl;
//}
#endif

// �ӽ� ��ü�� ���� ��ȯ
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
	getInt32A().val = 0; // �ӽð�ü�̴�. �������� i�ʹ� �ٸ� ��ü�̴�.
	getInt32B().val = 20;
	cout << i.val << endl; // ?
	return 0;
}