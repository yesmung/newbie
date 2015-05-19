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
// 위의 코드는 프레임워크가 제공하는 코드입니다.
//--------------------------------------------------------------------------
// 아래 코드는 사용자가 구현하는 코드입니다.

template<typename TYPE>
class Singleton
{
protected:
	Singleton(){};

private:
	Singleton(const Singleton&);
	void operator=(const Singleton&);

	static Mutex slock;
	static TYPE* pInstance;

public:
	static TYPE& getInstance() {
		if (pInstance == 0) {
			pInstance = new TYPE;
		}
		return *pInstance;
	}
};

template<typename TYPE>Mutex Singleton<TYPE>::slock;
template<typename TYPE>TYPE* Singleton<TYPE>::pInstance = 0;

class Cursor : public Singleton<Cursor>
{
private :

};

void main()
{
	// 싱글톤을 상속으로 만들었을 때.... 문제, 결국 다른 객체가 만들어진다.
	Cursor c1;
	Cursor c2; 

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;
}