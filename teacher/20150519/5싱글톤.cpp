
// 스레드에 안전한 싱글톤
#include <iostream>
using namespace std;

// 3.cpp에서 사용했던 Cursor클래스를 복사해오세요.


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
protected:			// 자식은 생성자를 호출할 수 있도록 하기 위해 protected
	Singleton() {}	// 생성자로 정의해야 합니다.

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

// 위의 코드는 프레임워크가 제공하는 코드입니다.
//--------------------------------------------------------------------
// 아래의 코드는 사용자가 구현하는 코드입니다.

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
