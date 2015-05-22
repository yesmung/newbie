
#if 0
// thiscall
#include <iostream>
using namespace std;

class Int32
{
public:
	int val;
	
	// C++ 함수 호출 규약: __thiscall
	// -> 함수가 호출될 때, 객체의 주소가 전달되는 방식
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

// 정적 멤버와 일반 멤버
#include <iostream>
using namespace std;

class Clazz
{
	int member;
	static int static_member;	// Clazz라는 네임스페이스 안에 갖힌 전역 변수

public:
	// 일반 멤버 함수
	// 일반 멤버에 접근 가능
	// 정적 멤버에 접근 가능
	void member_func()	// void member_func(Clazz* const this)
	{
		this->member = 0;	// OK
		static_member = 0;	// OK

		member_func();	// this->member_func();
		static_func();	// OK
	}

	// 정적 멤버 함수: C 언어의 일반 함수와 동일합니다.
	static void static_func()	// void static_func()
	{
		// member = 0;		// ERROR, this->member = 0;
		static_member = 0;	// OK

		member_func();		// ERROR, this->member_func();
		static_func();		// OK
	}
};
int Clazz::static_member = 0;

// static 키워드는 객체의 포함되지 않는 전역 심볼을 생성할 때, 사용

void main()
{
	Clazz obj;
	obj.member_func();

	Clazz::static_func();	// Clazz.static_func();
}