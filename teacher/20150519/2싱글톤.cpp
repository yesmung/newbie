
// 싱글톤 이야기
#include <iostream>
using namespace std;

class Cursor
{
	// 1. 사용자로부터 객체의 임의 생성을 막기 위하여 생성자를
	// private 영역에 정의해야 합니다.
	Cursor() {}

	// 3. 복사와 대입을 금지하기 위해 복사 생성자와 대입 연산자 함수를
	// private 영역에 선언해야 합니다.
	Cursor(const Cursor&);			// = delete;
	void operator=(const Cursor&);	// = delete;
	static Cursor* pInstance;
	
public:
	// 2. 유일한 객체를 반환하기 위한 정적 인터페이스를 제공해야
	// 합니다.
	static Cursor& getInstance()
	{
		if (pInstance == 0)
			pInstance = new Cursor;
		return *pInstance;
	}
};
Cursor* Cursor::pInstance = 0;

int main()
{
	Cursor& c1 = Cursor::getInstance();	// Cursor.getInstance();
	Cursor& c2 = Cursor::getInstance();
	
	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;

	//Cursor c3 = c1;
	//cout << "&c3 = " << &c3 << endl;
}