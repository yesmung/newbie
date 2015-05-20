
#include <iostream>
using namespace std;

class Mutex
{
public:
	void lock() { cout << "Mutex::lock()" << endl; }
	void unlock() { cout << "Mutex::unlock()" << endl; }
};

class Autolock
{
	Mutex& mLock;
public:
	Autolock(Mutex& m) : mLock(m) { mLock.lock(); }
	~Autolock() { mLock.unlock(); }
};

template<typename TYPE>
class Singleton
{
protected:
	Singleton() {}

private:
	Singleton(const Singleton&);
	void operator=(const Singleton&);

	static Mutex slock;
	static TYPE* pInstance;

public:
	static TYPE& getInstance() {
		if (pInstance == 0)
		{
			pInstance = new TYPE; // ��ü memory allocation -> ������ ȣ��
		}
		return *pInstance;
	}
};
template<typename TYPE> Mutex Singleton<TYPE>::slock;
template<typename TYPE> TYPE* Singleton<TYPE>::pInstance = 0;


class Cursor : public Singleton<Cursor>	// CRTP
{
	// �̱��� Ŭ������ ��ӹ޴� �ڽĿ��� �� ���� ��Ģ�� �߻�
	// Cursor() : Singleton() {}
	// 1. �ڽ��� �����ڴ� private ���� �����ؾ� �մϴ�.
	// 2. �θ� Ŭ������ ģ���� �ξ�� �Ѵ�. (friend)
private:
	Cursor() {}
	friend class Singleton<Cursor>;
};

//void main()
//{
//	Cursor c1;	// �⺻ �����ڰ� ȣ��
//	Cursor c2;
//
//	cout << "&c1 = " << &c1 << endl;
//	cout << "&c2 = " << &c2 << endl;
//}


void main()
{
	// Cursor c;	// ERROR
	Cursor& c1 = Cursor::getInstance();	// OK
	Cursor& c2 = Cursor::getInstance();	// OK
	
	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;
}
