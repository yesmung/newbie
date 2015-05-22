
// 싱글톤 정책을 자동화해봅시다. :D
// 2.cpp 코드를 복사해오세요
#include <iostream>
using namespace std;

// 복사와 대입을 금지하는 매크로
#define MAKE_NO_COPY(CLASSNAME)					\
	private:									\
		CLASSNAME(const CLASSNAME&);			\
		void operator=(const CLASSNAME&);

// 싱글톤 정책을 자동화하기 위한 매크로
#define DECLARE_SINGLETON(CLASSNAME)				\
		MAKE_NO_COPY(CLASSNAME)						\
	private: CLASSNAME() {}							\
		 static CLASSNAME* pInstance;				\
	public:	static CLASSNAME& getInstance();

#define IMPLEMENT_SIGNLETON(CLASSNAME)				\
	CLASSNAME* CLASSNAME::pInstance = 0;			\
	CLASSNAME& CLASSNAME::getInstance() {			\
		if (pInstance == 0)							\
			pInstance = new CLASSNAME;				\
		return *pInstance; }						

// 위의 코드는 라이브러리 또는 프레임워크에서 제공하는 코드입니다.
//--------------------------------------------------------------
// 아래는 사용자가 구현하는 코드입니다.

//class Cursor
//{
//	MAKE_NO_COPY(Cursor)
//	Cursor() {}
//	static Cursor* pInstance;
//public:
//	static Cursor& getInstance();
//};
//
//Cursor* Cursor::pInstance = 0;
//Cursor& Cursor::getInstance()
//{
//	if (pInstance == 0)
//		pInstance = new Cursor;
//	return *pInstance;
//}

class Cursor
{
	DECLARE_SINGLETON(Cursor)
};
IMPLEMENT_SIGNLETON(Cursor)

int main()
{
	Cursor& c1 = Cursor::getInstance();	// Cursor.getInstance();
	Cursor& c2 = Cursor::getInstance();
	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;
}