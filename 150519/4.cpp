// 스레드에 안전한 싱글톤

#include <iostream>
using namespace std;

// 동기화 객체를 설계합니다.
class Mutex
{
public :
	void lock() { cout << "Mutex::lock()" << endl; }
	void unlock() { cout << "Mutex::unlock()" << endl; }
};

// RAII 객체를 설계합니다.
class Autolock
{
	Mutex& mLock;

public :
	Autolock(Mutex& m) : mLock(m) { mLock.lock(); }
	~Autolock() { mLock.unlock(); }
};

class Cursor
{
	Cursor() {}
	Cursor(const Cursor& c);
	Cursor& operator=(const Cursor& c);
	static Cursor* pInstance;
	static Mutex slock;

public:
	static Cursor& getInstance()
	{
		// slock.lock();
		Autolock l(slock);
		//-----------------------------
		if (pInstance == 0)
		{
			pInstance = new Cursor; // thread A에서 예외 발생 시, thread B는 unlock 안됨.. A의 예외를 catch 하는 곳까지 
									// stack unwinding...되므로, thread B는 기다릴 뿐 => 즉, thread B에 대해서 unlock할 수 있도록 해야함
									// RAII (소멸자 호출 보장됨)
		}
		//-----------------------------
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