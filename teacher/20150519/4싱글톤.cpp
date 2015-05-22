
// 스레드에 안전한 싱글톤
#include <iostream>
using namespace std;

// 3.cpp에서 사용했던 Cursor클래스를 복사해오세요.


// 동기화 객체를 설계합니다.
class Mutex
{
public:
	void lock() { cout << "Mutex::lock()" << endl; }
	void unlock() { cout << "Mutex::unlock()" << endl; }
};

// RAII 객체를 설계합니다.
class Autolock
{
	Mutex& mLock;
public:
	Autolock(Mutex& m) : mLock(m) { mLock.lock(); }
	~Autolock() { mLock.unlock(); }
};

class Cursor
{
	Cursor(const Cursor&);
	Cursor& operator=(const Cursor&);
	Cursor() {}

	static Mutex slock;
	static Cursor* pInstance;
public:
	static Cursor& getInstance()
	{
		Autolock l(slock);
		// slock.lock();
		//-------------------------
		if (pInstance == 0)
			pInstance = new Cursor;
		//-------------------------
		// slock.unlock();

		return *pInstance;
	}
};
Cursor* Cursor::pInstance = 0;
Mutex Cursor::slock;

void main()
{
	Cursor& c = Cursor::getInstance();
}
