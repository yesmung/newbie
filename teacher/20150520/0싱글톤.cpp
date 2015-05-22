
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
			pInstance = new TYPE;	// 생성자 호출
		return *pInstance;
	}
};
template<typename TYPE> Mutex Singleton<TYPE>::slock;
template<typename TYPE> TYPE* Singleton<TYPE>::pInstance = 0;


class Cursor : public Singleton<Cursor>	// CRTP
{
	// 싱글톤 클래스를 상속받는 자식에게 두 가지 규칙이 발생
	// Cursor() : Singleton() {}
	// 1. 자식의 생성자는 private 으로 정의해야 합니다.
	// 2. 부모 클래스와 친구를 맺어야 합니다.
private: 
	Cursor() {}
	friend class Singleton<Cursor>;
};

//void main()
//{
//	Cursor c1;	// 기본 생성자가 호출
//	Cursor c2;
//
//	cout << "&c1 = " << &c1 << endl;
//	cout << "&c2 = " << &c2 << endl;
//}

void main()
{
	// Cursor c;							// ERROR
	Cursor& c1 = Cursor::getInstance();	// OK
	Cursor& c2 = Cursor::getInstance();	// OK

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c1 << endl;
}
