#if 0
// thiscall (c++�� �Լ� ȣ�� �Ծ�), __cdec�� ���� �˾ƺ���
#include <iostream>
using namespace std;

class Int32
{
public :
	int val;
	// C++ �Լ� ȣ�� �Ծ� : __thiscall
	// -> �Լ��� ȣ��� ��, ��ü�� �ּҰ� ���޵Ǵ� ���
	void setValue(int v) // void Int32::setValue(Int32* const this, int v)
	{ 
		val = v;		 // this->val = v;
	}
};

void main()
{
	Int32 i32;
	// cout << sizeof i32 << endl; ?

	i32.setValue(10);			// Int32::setValue(&i32, 10) ,, ��ü�ּҰ� �Ѿ�鼭 ȣ���
	cout << i32.val << endl;
}
#endif

// ���� ����� �Ϲ� ���
#include <iostream>
using namespace std;

class Clazz
{
	int member;
	static int static_member;	// Clazz��� ���ӽ����̽� �ȿ� ���� ���� ����
								// auto variable(��������)�� �ƴϱ⿡, int Clazz::static_member = 0�� ���� ���ǰ� �ʿ�
	
public :
	// �Ϲ� ��� �Լ�
	// �Ϲ� ����� ���� ����
	// ���� ����� ���� ����
	void member_func()
	{
		member = 0;		// OK
		// this->member = 0; // OK
		static_member = 0; // OK
		// member_func(); // this->member_func()
		static_func(); // OK
	}

	// ���� ��� �Լ� -> C����� �Ϲ� �Լ��� �����ϴ�.��
	static void static_func() // void static_func();
	{
		// member = 0;			// OK ? // ERROR, this->member=0
		// static_member = 0;  // OK
		// member_func(); // ERROR, this�� ����
		// static_func(); // OK
	}
};
int Clazz::static_member = 0;

// static Ű���� ��ü�� ���Ե��� ���� ���� �ɺ��� ������ ��, ����Ѵ�.

void main()
{
	Clazz obj;
	obj.member_func();

	// Clazz::static_func(); // Clazz.static_func() �� java
}