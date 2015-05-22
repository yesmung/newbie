
// �����忡 ������ �̱���
#include <iostream>
using namespace std;

// 3.cpp���� ����ߴ� CursorŬ������ �����ؿ�����.


// ����ȭ ��ü�� �����մϴ�.
class Mutex
{
public:
	void lock() { cout << "Mutex::lock()" << endl; }
	void unlock() { cout << "Mutex::unlock()" << endl; }
};

// RAII ��ü�� �����մϴ�.
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
