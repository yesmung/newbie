
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
			pInstance = new TYPE;
		return *pInstance;
	}
};
template<typename TYPE> Mutex Singleton<TYPE>::slock;
template<typename TYPE> TYPE* Singleton<TYPE>::pInstance = 0;

class Cursor : public Singleton<Cursor>
{
};

void main()
{
	Cursor& c1 = Cursor::getInstance();
	Cursor& c2 = Cursor::getInstance();
	

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;
}
