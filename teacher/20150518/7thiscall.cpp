
#if 0
// thiscall
#include <iostream>
using namespace std;

class Int32
{
public:
	int val;
	
	// C++ �Լ� ȣ�� �Ծ�: __thiscall
	// -> �Լ��� ȣ��� ��, ��ü�� �ּҰ� ���޵Ǵ� ���
	void setValue(int v)	// void Int32::setValue(Int32* const this, int v)
	{
		this->val = v;
	}
};

void main()
{
	Int32 i32;
	// cout << sizeof i32 << endl;	// ?

	i32.setValue(10);	// Int32::setValue(&i32, 10);
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

public:
	// �Ϲ� ��� �Լ�
	// �Ϲ� ����� ���� ����
	// ���� ����� ���� ����
	void member_func()	// void member_func(Clazz* const this)
	{
		this->member = 0;	// OK
		static_member = 0;	// OK

		member_func();	// this->member_func();
		static_func();	// OK
	}

	// ���� ��� �Լ�: C ����� �Ϲ� �Լ��� �����մϴ�.
	static void static_func()	// void static_func()
	{
		// member = 0;		// ERROR, this->member = 0;
		static_member = 0;	// OK

		member_func();		// ERROR, this->member_func();
		static_func();		// OK
	}
};
int Clazz::static_member = 0;

// static Ű����� ��ü�� ���Ե��� �ʴ� ���� �ɺ��� ������ ��, ���

void main()
{
	Clazz obj;
	obj.member_func();

	Clazz::static_func();	// Clazz.static_func();
}