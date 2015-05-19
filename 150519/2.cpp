// 싱글톤 이야기
#include <iostream>
using namespace std;

class Cursor
{
	// 1. 사용자로부터 객체의 임의 생성을 막기 위하여 생성자를 private 영역에 정의해야 한다.
	Cursor() {}
	static Cursor* pInstance;
	// Cursor(const Cursor& c) {}
	// Cursor& operator=(const Cursor& c) {}
	
	// 내부 생성 문제 해결
	// Cursor(const Cursor& c);
	// Cursor& operator=(const Cursor& c);

	// C++11에서 새로운 문법 제공 (타인이 보기에 이것은 SINGLETON임을 알 수 있다)
	Cursor(const Cursor& c) = delete;
	Cursor& operator=(const Cursor& c) = delete;

public :
	// 2. 유일한 객체를 반환하기 위한 정적 인터페이스를 제공해야 한다.
	// 즉, namespace 안에 갖혀 있는 전역 함수이다.
	static Cursor& getInstance() // 포인터리턴하면, 다른 곳에서 delete의 위험성이 있다. 그래서 참조리턴
	{
		// static Cursor cursor; // getInstance scope에서만 접근 가능한 전역 변수
		// return cursor;
		
		// static Cursor* pInstance; 에 맞게 구현

		if (pInstance == 0)
		{
			pInstance = new Cursor;
		}
		return *pInstance;
	}
	
	// 내부에서 생성 가능..문제임
	// 컴파일 타임에는 막을 수 없지만, 링킹타임에 막을 수 있다. 즉, 링킹..바인딩할 때, 에러를 발생시키면 되겠다.
	// 즉, 바인딩에 대한 하는 코드를 없애자... 
	// 다시 말해, private에 정의한 것들을 선언만 해버리자.
	//void foo()
	//{
	//	Cursor c;
	//	Cursor c2 = c;
	//	Cursor c3;
	//	c3 = c;
	//}
};
Cursor* Cursor::pInstance = 0;


int main()
{
	// Cursor c1;
	// Cursor c2;
    // 여기서는 android singleton pattern을 적용한다. - 고프, heap에 만듦.
	Cursor& c1 = Cursor::getInstance(); // Cursor.getInstance()
	Cursor& c2 = Cursor::getInstance(); 

	// 현재 코드의 구멍!!!!
	// Cursor c3 = c1;
	// cout << "&c3 = " << &c3 << endl;

	// 객체는
	// 선언과 동시 초기화 -> 복사 생성사 호출됨
	// 선언 후 초기화 -> 대입 연산자가 호출됨
	// 즉, 복사, 대입 생성자가 호출되지 않도록 막아야 됨 -> private 영역에 두어야 함!!!

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;

	// c1.foo();
}

// Singleton 을 만드는 방법 (자동화)
// 1. META CODE (macro) 
// 2. CRTP (templete)
// => 3.cpp