// �����忡 ������ �̱���

#include <iostream>
using namespace std;

// ����ȭ ��ü�� �����մϴ�.
class Mutex
{
public :
	void lock() { cout << "Mutex::lock()" << endl; }
	void unlock() { cout << "Mutex::unlock()" << endl; }
};

// RAII ��ü�� �����մϴ�.
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
			pInstance = new Cursor; // thread A���� ���� �߻� ��, thread B�� unlock �ȵ�.. A�� ���ܸ� catch �ϴ� ������ 
									// stack unwinding...�ǹǷ�, thread B�� ��ٸ� �� => ��, thread B�� ���ؼ� unlock�� �� �ֵ��� �ؾ���
									// RAII (�Ҹ��� ȣ�� �����)
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