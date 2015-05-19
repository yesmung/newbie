#include <iostream>
using namespace std;

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
// 위의 코드는 프레임워크가 제공하는 코드입니다.
//--------------------------------------------------------------------------
// 아래 코드는 사용자가 구현하는 코드입니다.

// template은 타입이 아니다, 즉 아래에서 Singleton은 템플릿이다.
template<typename TYPE>
class Singleton
{
protected : // 자식은 생성자를 호출할 수 있도록 하기 위해 protected 생성자를 사용
	Singleton(){};

private :
	Singleton(const Singleton&);
	void operator=(const Singleton&);

	static Mutex slock;
	static TYPE* pInstance;

public :
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
};

void main()
{
	// Cursor& c = Cursor::getInstance(); // static Singleton& getInstance() 일 경우, 에러
	Cursor& c1 = Cursor::getInstance();
	Cursor& c2 = Cursor::getInstance();

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;
}