#if 0
// thiscall (c++의 함수 호출 규약), __cdec에 대해 알아보자
#include <iostream>
using namespace std;

class Int32
{
public :
	int val;
	// C++ 함수 호출 규약 : __thiscall
	// -> 함수가 호출될 때, 객체의 주소가 전달되는 방식
	void setValue(int v) // void Int32::setValue(Int32* const this, int v)
	{ 
		val = v;		 // this->val = v;
	}
};

void main()
{
	Int32 i32;
	// cout << sizeof i32 << endl; ?

	i32.setValue(10);			// Int32::setValue(&i32, 10) ,, 객체주소가 넘어가면서 호출됨
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
								// auto variable(지역변수)가 아니기에, int Clazz::static_member = 0와 같은 정의가 필요
	
public :
	// 일반 멤버 함수
	// 일반 멤버에 접근 가능
	// 정적 멤버에 접근 가능
	void member_func()
	{
		member = 0;		// OK
		// this->member = 0; // OK
		static_member = 0; // OK
		// member_func(); // this->member_func()
		static_func(); // OK
	}

	// 정적 멤버 함수 -> C언어의 일반 함수와 동일하다.★
	static void static_func() // void static_func();
	{
		// member = 0;			// OK ? // ERROR, this->member=0
		// static_member = 0;  // OK
		// member_func(); // ERROR, this가 없다
		// static_func(); // OK
	}
};
int Clazz::static_member = 0;

// static 키워는 객체에 포함되지 않은 전역 심볼을 생성할 때, 사용한다.

void main()
{
	Clazz obj;
	obj.member_func();

	// Clazz::static_func(); // Clazz.static_func() 는 java
}