#include <iostream>
using namespace std;

#if 0
// 참조 계수를 대상체가 관리하도록 코드를 구현해봅시다.
class Car
{
	int mCount;
public :
	// 참조 계수를 관리하기 위한 인터페이스를 제공
	void incStrong() { ++mCount;  }
	void decStrong() { if(--mCount==0) delete this;  }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Car* p1 = new Car; // 객체 생성 후, 참조 계수 증가
	p1->incStrong();
	Car* p2 = p1; // 객체 복사 후, 참조 계수 증가
	p2->incStrong();

	// 객체의 사용이 끝난 경우
	p2->decStrong();	// 참조 계수 감소
	p1->decStrong();	// 참조 계수 감소 후, 소멸
}
#endif


#if 0
template <typename T>
class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong();  }
	~Sptr() { if (pObj) pObj->decStrong(); }
	Sptr(const T& c) : pObj(c.pObj)	{ if (pObj) pObj->incStrong();  }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};


// 참조 계수를 대상체가 관리하도록 코드를 구현해봅시다.
class Car
{
	int mCount;
public:
	// 참조 계수를 관리하기 위한 인터페이스를 제공
	void incStrong() { ++mCount; }
	void decStrong() { if (--mCount == 0) delete this; }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}
#endif

// 참조 계수와 관련된 코드를 자동으로 생성할 수 있도록
// 부모 클래스를 설계합니다.

class RefBase
{
	int mCount;
public:
	RefBase() : mCount(0){}
	virtual ~RefBase(){}
	void incStrong() { ++mCount; }
	void decStrong() { if (--mCount == 0) delete this; }
};

template <typename T>
class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong(); }
	~Sptr() { if (pObj) pObj->decStrong(); }
	Sptr(const T& c) : pObj(c.pObj)	{ if (pObj) pObj->incStrong(); }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

class Car : public RefBase // p57
{
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}